#include "ConstantBuffer.hlsli"

Texture2D Tex_Diffuse : register(t0);

Texture2D Tex_Normal : register(t1);

SamplerState sam_0 : register(s0);

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
    float4 color : COLOR;
    float2 uv : TEXCOORD;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float3 world : POSITION;
    
    uint objectID : OBJECTID;
};

struct PS_OUT
{
    // 메모리 낭비지만 .. Debug Quad의 유연함을 위해서 따로 뽑는다.
    float4 Depth : SV_Target0;
    
    float4 Normal : SV_Target1;
    
    float4 Position : SV_Target2;
    
    float4 Albedo : SV_Target3;
    
    uint ObjectID : SV_Target4;
};

VS_OUT VS_Main(VS_IN input)
{
    VS_OUT output = (VS_OUT) 0;
    
#ifdef USE_SKINNING
    float weights[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
    
    weights[0] = input.weight.x;
    weights[1] = input.weight.y;
    weights[2] = input.weight.z;
    weights[3] = 1.0f - weights[0] - weights[1] - weights[2];

    float3 posW = float3(0.0f, 0.0f, 0.0f);
    float3 normalW = float3(0.0f, 0.0f, 0.0f);

    for (int i = 0; i < 4; i++)
    {
        posW += weights[i] * mul(float4(input.pos, 1.0f), _boneMatrix[input.boneIndex[i]]).xyz;
        normalW += weights[i] * mul(input.normal, (float3x3)_boneMatrix[input.boneIndex[i]]);
    }
    
    output.pos = mul(float4(posW, 1.f), _worldViewProj);
    
    output.world = mul(float4(posW, 1.f), _world);
    
    output.normal = normalize(mul(normalW, (float3x3)_worldInvTranspose));
 #else
    output.pos = mul(float4(input.pos, 1.0f), _worldViewProj);
    
    output.world = mul(float4(input.pos, 1.0f), _world).xyz;
    
    output.normal = normalize(mul(input.normal, (float3x3) _worldInvTranspose));
 #endif
    
    output.color = input.color;
    
    output.uv = input.uv;

    output.tangent = mul(input.tangent, (float3x3) _world).xyz;
    
    output.objectID = _objectID;
    
    return output;
}

PS_OUT PS_Main(VS_OUT input)
{
    float4 color = input.color;
    
#ifdef USE_DIFFUSE
    // Gamma -> Linear
    color = pow(Tex_Diffuse.Sample(sam_0, input.uv), 2.2f);
   // color = Tex_Diffuse.Sample(sam_0, input.uv);
#endif
    
#ifdef USE_NORMAL
    float3 newNormal = Tex_Normal.Sample(sam_0, input.uv);
    
    // Normal Vector를 R8G8B8A8에서 0 ~ 1 (RGB) 사이로 저장하기 위해 가공해서 들어옴
    newNormal = newNormal * 2.0f - 1.f;
    
    float3 binormal = cross(input.normal, input.tangent);
    
    float3x3 TangentToWorld = float3x3(input.tangent, binormal, input.normal);
    
    newNormal = mul(newNormal, TangentToWorld);
    
    input.normal = normalize(newNormal);
#endif
    
    PS_OUT output = (PS_OUT) 0;
    
    output.Depth = float4(input.pos.zzz, 1.f);
    
    output.Position = float4(input.world, 1.f);
    
    output.Normal = float4(input.normal, 1.f);

    output.Albedo = color;
    
    output.ObjectID = input.objectID;
    
    return output;
}