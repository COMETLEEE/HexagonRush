#include "ConstantBuffer.hlsli"

// Dist : World - View Space Z => Depth로 하면 쓸데 없이 거의 뒷부분으로 넘어감 .. 텍스쳐 낭비도 생기고
uint CalcCascadeIndex(float Dist)
{
    uint index = MAX_CASCADE_COUNT - 1;

    for (uint i = 0; i < MAX_CASCADE_COUNT; i++)
    {
        if (Dist <= _cascadeEnds[i])
        {
            index = i;
            break;
        }
    }
    
    return index;
}

float CalcShadowFactor(SamplerComparisonState samShadow, Texture2D shadowMap, float3 shadowPosNDC)
{
    float depth = shadowPosNDC.z;
    
    // 쉐도우 맵의 텍셀 크기
    const float dx = _textureInfo.z;
    
    const float dy = _textureInfo.w;
    
    float percentLit = 0.0f;
    
    const float2 offsets[9] =
    {
        float2(-dx, -dy), float2(0.0f, -dy), float2(dx, -dy),
        float2(-dx, 0.0f), float2(0.0f, 0.0f), float2(dx, 0.0f),
        float2(-dx, +dy), float2(0.0f, +dy), float2(dx, +dy)
    };

    [unroll]
    for (int i = 0; i < 9; i++)
    {
        percentLit += shadowMap.SampleCmpLevelZero(samShadow, shadowPosNDC.xy + offsets[i], depth).r;
    }
    
    return percentLit /= 9.0f;
}

// Dist : World - View Space Z
float CalcCascadeShadowColor(SamplerComparisonState samShadow, Texture2DArray cascadeShadow, float4 PosWorld, float Dist)
{
    uint index = CalcCascadeIndex(Dist);
        
    float4 PosShadowSpace = mul(PosWorld, _lightViewProjList[index]);
    
    float4 texCoord = PosShadowSpace / PosShadowSpace.w;
    
    // NDC -> UV Space
    texCoord.y = -texCoord.y;
    
    texCoord = texCoord * 0.5f + 0.5f;
    
    if ((saturate(texCoord.x) != texCoord.x) || (saturate(texCoord.y) != texCoord.y))
    {
        return 1.f;
    }
    
    float compare_depth = PosShadowSpace.z;
   
    float percentLit = 0.0f;

    int2 offsets[9] =
    {
        -1, -1, 0, -1, 1, -1,
        -1, 0, 0, 0, 1, 0,
        -1, 1, 0, 1, 1, 1
    };
    
    [unroll]
    for (int i = 0; i < 9; i++)
    {
        percentLit += cascadeShadow.SampleCmpLevelZero(samShadow, float3(texCoord.xy, index), compare_depth, offsets[i]).r;
    }
    
    return percentLit /= 9.0f;
}