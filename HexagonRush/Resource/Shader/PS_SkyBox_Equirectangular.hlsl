static const float PI = 3.141592;

static const float TwoPI = 2 * PI;

struct VS_OUT
{
    float4 posH : SV_Position;
    float3 posL : POSITON;
    float4 posW : POSITION2;
    
    uint objectID : OBJECTID;
};

struct PS_OUT
{
    float4 DMRAO : SV_Target0;
    
    float4 Normal : SV_Target1;
    
    float4 Position : SV_Target2;
    
    float4 Albedo : SV_Target3;
    
    float4 Emissive : SV_Target4;
    
    uint ObjectID : SV_Target5;
};

Texture2D Tex_Equirectangular : register(t0);

SamplerState Sam_Clamp : register(s0);

PS_OUT PS_SkyBox_Equirectangular(VS_OUT input)
{
    PS_OUT output = (PS_OUT) 0;
        
    output.Position = input.posW;
    
    // »ùÇÃ¸µ ¹æÇâ
    float3 dir = normalize(input.posL);
    
    float longitude = atan2(dir.x, dir.z) * (180.f / PI);
    
    float latitude = asin(dir.y) * (180.f / PI);
    
    output.Albedo = pow(Tex_Equirectangular.SampleLevel(Sam_Clamp, float2(longitude, latitude) / float2(360.f, -180.f) + 0.5, 0), 2.2f);
    
    output.ObjectID.x = input.objectID;
    
    output.DMRAO = float4(input.posH.zzz, 1.f);
    
    output.Emissive = float4(0.f, 0.f, 0.f, 1.f);
    
    return output;
}