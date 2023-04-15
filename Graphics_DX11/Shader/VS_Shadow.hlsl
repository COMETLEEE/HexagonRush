#include "ConstantBuffer.hlsli"

#ifdef USE_SKINNING
    struct VS_IN
    {
        float3 pos : POSITION;
        float2 uv : TEXCOORD;
        float3 normal : NORMAL;
        float3 tangent : TANGENT;
        float4 color : COLOR;

        float4 weight : WEIGHT;
        uint4 boneIndex : BONEINDEX;
    };
#else
struct VS_IN
{
    float3 pos : POSITION;
    float2 uv : TEXCOORD;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float4 color : COLOR;
};
#endif

struct VS_OUT
{
    float4 pos : SV_POSITION;
};

VS_OUT VS_Shadow(VS_IN input)
{
    VS_OUT output;

#ifdef USE_SKINNING
    float weights[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
    
    weights[0] = input.weight.x;
    weights[1] = input.weight.y;
    weights[2] = input.weight.z;
    weights[3] = 1.0f - weights[0] - weights[1] - weights[2];

    float3 posW = float3(0.0f, 0.0f, 0.0f);

    for (int i = 0; i < 4; i++)
    {
        posW += weights[i] * mul(float4(input.pos, 1.0f), _boneMatrix[input.boneIndex[i]]).xyz;
    }
    
    output.pos = mul(float4(posW, 1.f), _world);
  
    output.pos = mul(output.pos, _lightViewProj);
#else
    // To World Space
    output.pos = mul(float4(input.pos, 1.0f), _world);
    
    // To Light View Proj Space
    // TODO : 다양한 빛, Light View Proj으로부터 Shadow Map을 만들어내야 합니다.
    output.pos = mul(output.pos, _lightViewProj);
#endif

    return output;
}