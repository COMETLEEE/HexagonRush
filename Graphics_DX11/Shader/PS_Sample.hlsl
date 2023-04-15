struct VS_IN
{
    float3 pos : POSITION;
    float2 uv : TEXCOORD;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float4 color : COLOR;
};

struct VS_OUT
{
    float4 pos : SV_POSITION;
    float4 color : COLOR;
    float2 uv : TEXCOORD;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float3 world : POSITION;
};

Texture2D Texture_Sample : register(t0);

SamplerState Sam_Clamp : register(s0);      // »ùÇÃ·¯ (Clamp)

float4 PS_Sample(VS_OUT input) : SV_TARGET
{
    float4 color = input.color;
    
    color = Texture_Sample.Sample(Sam_Clamp, input.uv);
    
    color.a = 1.f;
    
    return color;
}