struct VS_OUT
{
    float4 posH : SV_Position;
    float3 posL : POSITON;
    float4 posW : POSITION2;
    
    uint objectID : OBJECTID;
};

struct PS_OUT
{
    // 메모리 낭비지만 .. Debug Quad의 유연함을 위해서 따로 뽑는다.
    float4 Depth : SV_Target0;
    
    float4 Normal : SV_Target1;
    
    float4 Position : SV_Target2;
    
    float4 Albedo : SV_Target3;
    
    float4 Emissive : SV_Target4;
    
    uint ObjectID : SV_Target5;
};

TextureCube Tex_CubeMap : register(t0);

SamplerState Sam_Clamp : register(s0);

PS_OUT PS_SkyBox(VS_OUT input)
{
    PS_OUT output = (PS_OUT) 0;
        
    output.Position = input.posW;
    
    // Gamma -> Linear
    output.Albedo = pow(Tex_CubeMap.Sample(Sam_Clamp, input.posL), 2.2f);
    
    output.ObjectID.x = input.objectID;
  
    output.Depth = float4(input.posH.zzz, 1.f);
    
    output.Emissive = float4(0.f, 0.f, 0.f, 1.f);
    
    return output;
}