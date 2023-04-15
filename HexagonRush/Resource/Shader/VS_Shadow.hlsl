#include "ConstantBuffer.hlsli"

#ifdef USE_SKINNING
    struct VS_IN
    {
        float3 pos : POSITION;
        float2 uv : TEXCOORD;
        float3 normal : NORMAL;
        float3 tangent : TANGENT;
        float4 color : COLOR;

        float4 weight1 : WEIGHT;
        float4 weight2 : WEIGHTS;
        float4 weight3 : WEIGHTSS;

        uint4 boneIndex1 : BONEINDEX;
        uint4 boneIndex2 : BONEINDEXES;
        uint4 boneIndex3 : BONEINDEXESS;
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
    float3 posW = float3(0.0f, 0.0f, 0.0f);
    
    float totalWeight = (float) 0.f;
    
     for (int k = 0; k < 4; k++)
    {
        totalWeight += input.weight1[k];
        totalWeight += input.weight2[k];
        totalWeight += input.weight3[k];
    }
    
    float totalWeightInv = 1.f / totalWeight;
    
    for (int i = 0; i < 4; i++)
    {
        posW += input.weight1[i] * mul(float4(input.pos, 1.0f), _boneMatrix[input.boneIndex1[i]]).xyz * totalWeightInv;
    }
        
    for (int j = 0; j < 4; j++)
    {
        posW += input.weight2[j] * mul(float4(input.pos, 1.0f), _boneMatrix[input.boneIndex2[j]]).xyz * totalWeightInv;
    }
 
    for (int z = 0; z <4; z++)
    {
        posW += input.weight3[z] * mul(float4(input.pos, 1.0f), _boneMatrix[input.boneIndex3[z]]).xyz * totalWeightInv;
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