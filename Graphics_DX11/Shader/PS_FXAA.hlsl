#include "ConstantBuffer.hlsli"

#define EDGE_THRESHOLD_MIN 0.0312
#define EDGE_THRESHOLD_MAX 0.125
#define MAX_SEARCH_ITERATIONS 12
#define SUBPIXEL_QUALITY 0.75

struct VS_OUT
{
    float4 pos : SV_POSITION;
    float4 color : COLOR;
    float2 uv : TEXCOORD;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float3 world : POSITION;
};

Texture2D Texture_Target : register(t0);

SamplerState Sam_Clamp : register(s0);    

float RGBToLuma(float4 rgb)
{
   return sqrt(dot(rgb.xyz, float3(0.299, 0.587, 0.114)));
}

float4 PS_FXAA(VS_OUT input) : SV_TARGET
{
    float QUALITY[MAX_SEARCH_ITERATIONS] =
    {
        1.f, 1.f, 1.5f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f
    };
    
    float4 colorCenter = Texture_Target.Sample(Sam_Clamp, input.uv);
    
    float lumaCenter = RGBToLuma(colorCenter);

    float2 up = float2(0.f, -_textureInfo.w);
    float2 down = float2(0.f, _textureInfo.w);
    float2 left = float2(-_textureInfo.z, 0.f);
    float2 right = float2(_textureInfo.z, 0.f);
    
    float lumaDown = RGBToLuma(Texture_Target.Sample(Sam_Clamp, input.uv + down));
    float lumaUp = RGBToLuma(Texture_Target.Sample(Sam_Clamp, input.uv + up));
    float lumaLeft = RGBToLuma(Texture_Target.Sample(Sam_Clamp, input.uv + left));
    float lumaRight = RGBToLuma(Texture_Target.Sample(Sam_Clamp, input.uv + right));
    
    float lumaMin = min(lumaCenter, min(min(lumaDown, lumaUp), min(lumaLeft, lumaRight)));
    float lumaMax = max(lumaCenter, max(max(lumaDown, lumaUp), max(lumaLeft, lumaRight)));
    
    float lumaRange = lumaMax - lumaMin;
    
    // Luma의 차이가 임계값보다 크다면 밝기의 변화가 큰 곳 => 가장자리 O
    // 차이가 임계값보다 작다면, 또는 정말 어두운 영역에 있면 시행하지 않는다. => 가장자리 X
    if (lumaRange < max(EDGE_THRESHOLD_MIN, lumaMax * EDGE_THRESHOLD_MAX))
        return colorCenter;
    
    float lumaDownLeft = RGBToLuma(Texture_Target.Sample(Sam_Clamp, input.uv + down + left));
    float lumaUpRight = RGBToLuma(Texture_Target.Sample(Sam_Clamp, input.uv + up + right));
    float lumaUpLeft = RGBToLuma(Texture_Target.Sample(Sam_Clamp, input.uv + up + left));
    float lumaDownRight = RGBToLuma(Texture_Target.Sample(Sam_Clamp, input.uv + down + right));
    
    // 4개의 가장자리의 lumas를 조합 (같은 값으로 추후 계산을 위해 중간 변수로 사용)
    float lumaDownUp = lumaDown + lumaUp;
    float lumaLeftRight = lumaLeft + lumaRight;

    // 코너도 동일하다 ..!
    float lumaLeftCorners = lumaDownLeft + lumaUpLeft;
    float lumaDownCorners = lumaDownLeft + lumaDownRight;
    float lumaRightCorners = lumaDownRight + lumaUpRight;
    float lumaUpCorners = lumaUpRight + lumaUpLeft;
    
    // 수평과 수직 축을 따르는 변화도의 추정치 계산
    // 수평: | (upleft - left) - (left - downleft) | +2 *  | (up - center) - (center - down) | + | (upright - right) - (right - downright) |
    float edgeHorizontal = abs(-2.0 * lumaLeft + lumaLeftCorners) + abs(-2.0 * lumaCenter + lumaDownUp) * 2.0 + abs(-2.0 * lumaRight + lumaRightCorners);
    
    // 수직: |(upright - up) - (up - upleft)| + 2 * |(right - center) - (center - left)| + |(downright - down) - (down - downleft)|
    float edgeVertical = abs(-2.0 * lumaUp + lumaUpCorners) + abs(-2.0 * lumaCenter + lumaLeftRight) * 2.0 + abs(-2.0 * lumaDown + lumaDownCorners);
    
    // 로컬 가장자리가 수평인가? 수직인가?
    bool isHorizontal = (edgeHorizontal >= edgeVertical);
    
    // 로컬 가장자리의 반대방향으로 2개의 이웃 텍셀을 선택함
    float luma1 = isHorizontal ? lumaDown : lumaRight;          // UV 증가
    float luma2 = isHorizontal ? lumaUp : lumaLeft;             // UV 감소

    // 이 방향으로 변화도를 계산
    float gradient1 = luma1 - lumaCenter;   // UV 증가 방향으로의 변화도
    float gradient2 = luma2 - lumaCenter;   // UV 감소 방향으로의 변화도

    // 어느 방향이 변화가 더 가파른가?
    // 증가 방향으로 더 가파르면 (아랫쪽 또는 오른쪽) true, 감소 방향으로 더 가파르면 (위쪽 또는 왼쪽) false
    bool is1Steepest = abs(gradient1) >= abs(gradient2);

    // 해당 방향의 변화도, 정규화
    float gradientScaled = 0.25f * max(abs(gradient1), abs(gradient2));
    
    // 가장자리의 방향에 따라서 스텝 크기(한 픽셀)를 선택
    // 가장자리가 수평이면 당연히 수직 방향으로 이동해서 참고할 것이다.
    float stepLength = isHorizontal ? _textureInfo.w : _textureInfo.x;

    // 올바른 방향의 평균 luma
    float lumaLocalAverage = 0.0;

    // 증가 방향으로 더 가파르다.
    if (is1Steepest)
    {
        lumaLocalAverage = 0.5f * (luma1 + lumaCenter);
    }
    // 감소 방향으로 더 가파르다.
    else
    {
        // 따라서 방향을 바꿈 (Step을 UV가 감소되는 방향으로 이동해야한다.)
        stepLength = -stepLength;
        lumaLocalAverage = 0.5f * (luma2 + lumaCenter);
    }

    // UV 를 올바른 방향으로 반픽셀 이동시킴
    float2 currentUv = input.uv;
    
    // 수평 가장자리면 위쪽을 참고합시다 ..!
    if (isHorizontal)
    {
        currentUv.y += stepLength * 0.5f;
    }
    else
    {
        currentUv.x += stepLength * 0.5f;
    }
    
  // 오른쪽 방향으로 offset (각 반복 스탭에 대해) 을 계산.
    float2 offset = isHorizontal ? float2(_textureInfo.z, 0.0f) : float2(0.0, _textureInfo.w);

    // 가장자리의 각 측면을 직교하여 탐색하기 위해서 UV를 계산. QUALITY 는 스탭을 더 빠르게 함.
    float2 uv1 = currentUv - offset;
    float2 uv2 = currentUv + offset;

    // 탐색하는 픽셀의 양쪽 끝에서 lumas를 읽고, delta 를 계산하고 로컬 평균 luma에 기록
    float lumaEnd1 = RGBToLuma(Texture_Target.Sample(Sam_Clamp, uv1));
    float lumaEnd2 = RGBToLuma(Texture_Target.Sample(Sam_Clamp, uv2));
    lumaEnd1 -= lumaLocalAverage;
    lumaEnd2 -= lumaLocalAverage;

    // 현재 끝점에서 luma delta가 로컬 변화도 보다 크면, 우리는 가장자리의 측면에 도달한 것임
    bool reached1 = abs(lumaEnd1) >= gradientScaled;
    bool reached2 = abs(lumaEnd2) >= gradientScaled;
    bool reachedBoth = reached1 && reached2;

    // 측면에 도달하지 못했다면, 우리는 계속해서 이 방향으로 탐색함.
    if (!reached1)
    {
        uv1 -= offset;
    }
    
    if (!reached2)
    {
        uv2 += offset;
    }
    
    if (!reachedBoth)
    {
        // 만약 측면에 도달하지 않았다면, 우리는 이 방향으로 계속 탐색, 
        for (int i = 1; i < MAX_SEARCH_ITERATIONS; i++)
        {
            // 가변 품질로 진행 (역주 : 스탭을 반복에 따라 조정하겠다는 의미)
            // 필요하다면, 첫번재 방향의 luma를 읽음, delta 계산
            if (!reached1)
            {
                lumaEnd1 = RGBToLuma(Texture_Target.Sample(Sam_Clamp, uv1));
                lumaEnd1 -= lumaLocalAverage;
            }
        
            // 필요하다면, 반대 방향의 luma를 읽음, delta 계산
            if (!reached2)
            {
                lumaEnd2 = RGBToLuma(Texture_Target.Sample(Sam_Clamp, uv2));
                lumaEnd2 -= lumaLocalAverage;
            }
                
            // 만약 현재 끝점의 luma delta가 로컬 변화도 보다 크다면, 우리는 가장자리 측면에 도달한 것임
            reached1 = abs(lumaEnd1) >= gradientScaled;
            reached2 = abs(lumaEnd2) >= gradientScaled;
            reachedBoth = reached1 && reached2;

            if (!reached1)
                uv1 -= offset * QUALITY[i];
            
            if (!reached2)
                uv2 += offset * QUALITY[i];
            
            // 두 측면에 도착했다면, 탐색을 중단
            if (reachedBoth)
            {
                break;
            }
        }
    }
    
    // 양 끝 가장자리까지의 거리를 계산
    float distance1 = isHorizontal ? (input.uv.x - uv1.x) : (input.uv.y - uv1.y);
    float distance2 = isHorizontal ? (uv2.x - input.uv.x) : (uv2.y - input.uv.y);
    
    // 어떤 방향의 가장자리의 끝이 더 가깝나?
    bool isDirection1 = distance1 < distance2;
    float distanceFinal = min(distance1, distance2);

    // 가장자리의 길이
    float edgeThickness = (distance1 + distance2);

    // UV offset: 가장자리의 측면까지 가장 가까운 방향으로 읽음
    float pixelOffset = (-distanceFinal / edgeThickness) + 0.5f;
    
    // 중앙 luma가 로컬 평균보다 더 작나?
    bool isLumaCenterSmaller = lumaCenter < lumaLocalAverage;

    // 만약 중앙 luma가 그것의 이웃보다 더 작다면, 양 끝의 delta luma가 양수여야 합니다. (같은 변형)
    // (가장자리의 측면과 더 가까운 방향으로)
    bool correctVariation = ((isDirection1 ? lumaEnd1 : lumaEnd2) < 0.0) != isLumaCenterSmaller;

    // 만약 luma 차이가 잘못된 경우, offset 을 적용하지 않음
    float finalOffset = correctVariation ? pixelOffset : 0.0;
    
    // Sub-pixel shifting
    // 3x3 이웃에 대한 luma의 전체 가중 평균
    float lumaAverage = (1.0f / 12.0f) * (2.0f * (lumaDownUp + lumaLeftRight) + lumaLeftCorners + lumaRightCorners);

    // 3x3 이웃의 루마 범위에 대한, 전역 평균과 중앙 luma 간의 delta의 비율
    float subPixelOffset1 = clamp(abs(lumaAverage - lumaCenter) / lumaRange, 0.0f, 1.0f);
    float subPixelOffset2 = (-2.0 * subPixelOffset1 + 3.0) * subPixelOffset1 * subPixelOffset1;

    // 이 delta에 기반한 sub-pixel의 offset 계산
    float subPixelOffsetFinal = subPixelOffset2 * subPixelOffset2 * SUBPIXEL_QUALITY;

    // 두 offset 중 더 큰것을 고름
    finalOffset = max(finalOffset, subPixelOffsetFinal);
    
    float2 finalUv = input.uv;
    
    if (isHorizontal)
    {
        finalUv.y += finalOffset * stepLength;
    }
    else
    {
        finalUv.x += finalOffset * stepLength;
    }

    return float4(Texture_Target.Sample(Sam_Clamp, finalUv).xyz, 1.f);
}