#include "ConstantBuffer.hlsli"

// Albedo
Texture2D Tex_Albedo: register(t0);

// Normal
Texture2D Tex_Normal : register(t1);

// Metallic
Texture2D Tex_Metallic : register(t2);

// Roughness
Texture2D Tex_Roughness : register(t3);

// Ambient Occlusion
Texture2D Tex_AmbientOcclusion : register(t4);

// Emissive Map
Texture2D Tex_Emissive : register(t5);

// Sampler
SamplerState Sam_Clamp : register(s0);

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
    float4 color : COLOR;
    float2 uv : TEXCOORD;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float3 world : POSITION;
    
    uint objectID : OBJECTID;
    float metallic : METALLIC;
    float roughness : ROUGHNESS;
    float4 emissive : EMISSIVE;
};

struct PS_OUT
{
    float4 DMRAO : SV_Target0;      // D, M, R, AO (Depth, Metallic, Roughness, Ambient Occlusion)
    
    float4 Normal : SV_Target1;
    
    float4 Position : SV_Target2;
    
    float4 Albedo : SV_Target3;
    
    float4 Emissive : SV_Target4;   // 자체 발광 !
    
    uint ObjectID : SV_Target5;
};

VS_OUT VS_Main(VS_IN input)
{
    VS_OUT output = (VS_OUT) 0;
    
#ifdef USE_SKINNING
    float3 posW = float3(0.0f, 0.0f, 0.0f);
    float3 normalW = float3(0.0f, 0.0f, 0.0f);
    
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
        normalW += input.weight1[i] * mul(input.normal, (float3x3) _boneMatrix[input.boneIndex1[i]]) * totalWeightInv;
    }
        
    for (int j = 0; j < 4; j++)
    {
        posW += input.weight2[j] * mul(float4(input.pos, 1.0f), _boneMatrix[input.boneIndex2[j]]).xyz * totalWeightInv;
        normalW += input.weight2[j] * mul(input.normal, (float3x3) _boneMatrix[input.boneIndex2[j]]) * totalWeightInv;
    }
 
    for (int z = 0; z < 4; z++)
    {
        posW += input.weight3[z] * mul(float4(input.pos, 1.0f), _boneMatrix[input.boneIndex3[z]]).xyz * totalWeightInv;
        normalW += input.weight3[z] * mul(input.normal, (float3x3) _boneMatrix[input.boneIndex3[z]]) * totalWeightInv;
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
    
    output.metallic = _material._metallic;
    
    output.roughness = _material._roughness;
    
    output.emissive = _material._emissive;
    
    return output;
}

// PBR Model
PS_OUT PS_Main(VS_OUT input)
{
    float4 color = input.color;
    
    float metallic = input.metallic;
    
    float roughness = input.roughness;
    
    float ambientOcclusion = 1.f;
    
    float4 emissive = input.emissive;
    
#ifdef USE_ALBEDO
    // Gamma -> Linear
    color = pow(Tex_Albedo.Sample(Sam_Clamp, input.uv), 2.2f);
    // color = Tex_Diffuse.Sample(Sam_Clamp, input.uv);
#endif
    
#ifdef USE_NORMAL
    float3 newNormal = Tex_Normal.Sample(Sam_Clamp, input.uv);
    
    // Normal Vector를 R8G8B8A8에서 0 ~ 1 (RGB) 사이로 저장하기 위해 가공해서 들어옴
    newNormal = newNormal * 2.0f - 1.f;
    
    float3 binormal = cross(input.normal, input.tangent);
    
    float3x3 TangentToWorld = float3x3(input.tangent, binormal, input.normal);
    
    newNormal = mul(newNormal, TangentToWorld);
    
    input.normal = normalize(newNormal);
#endif
    
#ifdef USE_METALLIC
    metallic = Tex_Metallic.Sample(Sam_Clamp, input.uv);
#endif
    
#ifdef USE_ROUGHNESS
    roughness = Tex_Roughness.Sample(Sam_Clamp, input.uv);
#endif

#ifdef USE_AMBIENTOCCLUSION
    ambientOcclusion = Tex_AmbientOcclusion.Sample(Sam_Clamp, input.uv);
#endif                                                                                                               
    
#ifdef USE_EMISSIVE
    emissive = Tex_Emissive.Sample(Sam_Clamp, input.uv);
#endif
    
    
#ifdef USE_RIMLIGHT
    float rim = (float) 0.f;
    
    rim = 1.f - saturate(dot(input.normal, normalize(_cameraInfo._cameraWorldPos - input.world)));
    
    rim = pow(rim, 5.f);
    
    // 림 컬러
    emissive += float4(rim, rim, rim, 1.f) * float4(1.f, 0.f, 0.f, 1.f);
#endif
    
    PS_OUT output = (PS_OUT) 0;
    
    output.DMRAO = float4(input.pos.z, metallic, roughness, ambientOcclusion);
    
    output.Position = float4(input.world, 1.f);
    
    output.Normal = float4(input.normal, 1.f);

    output.Albedo = color;
    
    output.ObjectID = input.objectID;
    
    output.Emissive = emissive;
    
    return output;
}