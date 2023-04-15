static const float fAlpha = 1.f;

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

Texture2D Texture_Target : register(t0);

SamplerState Sam_Clamp : register(s0);

float4 PS_Vignetting(VS_OUT input) : SV_TARGET
{
    float2 uv = input.uv - 0.5f;
    
    float dist = length(uv);
    
    float radius = 0.7f;
    
    float softness = 0.4f;
    
    // 거리가 반지름 - soft ~ 반지름 사이에 있으면 Vignetting을 걸겠다.
    float vignette = smoothstep(radius, radius - softness, dist);           // 반지름 - softness 아래이면 0, 반지름 이상이면 1
        
    return Texture_Target.Sample(Sam_Clamp, input.uv) * vignette;
}