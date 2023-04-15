#include "ConstantBuffer.hlsli"

struct VS_OUT
{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD1;
    float3 toFarPlane : TEXCOORD0;
};

Texture2D Texture_Depth : register(t0);

Texture2D Texture_Normal : register(t1);

Texture2D Texture_RandomVectorMap : register(t2);

SamplerState Sam_Clamp : register(s0);      // 샘플러 (Wide Wrap)

SamplerState Sam_Wrap : register(s1);

// 무작위 표본점이 타겟 픽셀 포인트를 얼마나 가리는지 깊이 차이 Term을 계산한다.
float CalculateOcclusion(float distZ)
{
    // 만일 표본점의 Depth가 더 뒤에 있다면 가릴 수 없다. (distZ 가 음수인 경우)
    // 충분히 가까운 경우에도 가릴 수 없다. => Epsilon 이상은 앞에 있어야 가린다고 판정한다.
    // 물론 멀리 있어도 가릴 수 없다.
    float occlusion = 0.f;
    
    if (distZ > _ssaoInfo._surfaceEpsilon)
    {
        float fadeLength = _ssaoInfo._occlusionFadeEnd - _ssaoInfo._occlusionFadeStart;
        
        // distZ가 Start에 가까워 질수록 차폐도는 1, End에 가까워 질수록 0으로 선형 감소한다.
        occlusion = saturate((_ssaoInfo._occlusionFadeEnd - distZ) / fadeLength);
    }
    
    return occlusion;
}

float4 PS_SSAO(VS_OUT input) : SV_TARGET
{
    // 현재 픽셀의 World Space Normal
    float3 normal = Texture_Normal.Sample(Sam_Clamp, input.uv).xyz;
    
    // View Space Normal로 변환
    normal = mul(normal, (float3x3) _cameraInfo._viewMatrix);
    
    // 현재 픽셀의 깊이
    float depth = Texture_Depth.Sample(Sam_Clamp, input.uv).x;
        
    float viewPosZ = _cameraInfo._projMatrix._43 / (depth - _cameraInfo._projMatrix._33);
    
    // 현재 픽셀의 View-Space 위치를 뽑아낸다.
    // Origin - FarPlane Vector
    float3 targetPoint = (viewPosZ / input.toFarPlane.z) * input.toFarPlane;
    
    // (0, 1) => (-1, 1)
    float3 randomVector = 2.0f * Texture_RandomVectorMap.Sample(Sam_Wrap, input.uv).xyz - 1.f;
    
    float occlusionSum = 0.0f;
    
    [unroll]
    for (uint i = 0 ; i < _ssaoInfo._sampleCount ; i++)
    {
        // 미리 만들어둔 상수 오프셋 벡터들은 당연히 고르게 분포되어 있음.
        // 하나의 랜덤 벡터에 대해서 반사시키면 역시 고르게 분포된 무작위 벡터들이 만들어진다.        
        float3 offset = reflect(_ssaoInfo._offsetVectors[i].xyz, randomVector);
        
        // p 앞에 있는 녀석들에 대해서만 신경쓴다.
        float flip = sign(dot(offset, normal));
        
        // 차폐 반지름 이내의 무작위 점 q를 잡는다.
        float3 q = targetPoint + flip * _ssaoInfo._occlusionRadius * offset;
                
        // 무작위 점 q의 투영 텍스처 좌표를 찾자.
        float4 projTexCoord = mul(float4(q, 1.f), _cameraInfo._projMatrix);

        float2 projUV = projTexCoord.xy / projTexCoord.w;
        
        projUV.y = -projUV.y;
    
        projUV = projUV * 0.5f + 0.5f;
        
        // 무작위 표본점 q로부터 잠재적인 차폐점 r을 찾아라.
        float rDepth = Texture_Depth.Sample(Sam_Clamp, projUV.xy).x;
        
        float rViewPosZ = _cameraInfo._projMatrix._43 / (rDepth - _cameraInfo._projMatrix._33);
        
        float3 r = (rViewPosZ / q.z) * q;
        
        // 점 r이 TargetPoint를 차폐하는지 찾자.
        float distZ = targetPoint.z - r.z;
        float dp = max(dot(normal, normalize(r - targetPoint)), 0.0f);
        float occlusion = dp * CalculateOcclusion(distZ);
        
        occlusionSum += occlusion;
    }
    
    occlusionSum /= _ssaoInfo._sampleCount;
        
    return saturate(pow(1.f - occlusionSum, 8.f));
}