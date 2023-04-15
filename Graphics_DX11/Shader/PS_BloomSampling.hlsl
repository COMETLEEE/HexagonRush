#include "ConstantBuffer.hlsli"

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

float Bloom_Curve(float intensity, float threshold)
{
    float result = intensity;
    
    intensity *= 2.0f;
    
#ifdef BLOOMCURVE_METHOD_1
    result = intensity * 0.05 + max(0, intensity - threshold) * 0.5; // default threshold = 1.26
#endif

#ifdef BLOOMCURVE_METHOD_2
    result = intensity * intensity / 3.2;
#endif

#ifdef BLOOMCURVE_METHOD_3
    result = max(0, intensity - threshold); // default threshold = 1.0
    result *= result;
#endif 

    return result * 0.5f;
}

// 블룸이 될 곳을 찾는다.
float4 PS_BloomSampling(VS_OUT input) : SV_Target
{
    float4 color = Texture_Target.Sample(Sam_Clamp, input.uv);
    
    float intensity = dot(color.rgb, float3(0.3f, 0.3f, 0.3f));
    
    float bloom_Intensity = Bloom_Curve(intensity, 1.26f);

    // 강도가 0으로 나오면 나중에 문제 생길 수 있으니 ..
    if (intensity == (float) 0.f)
        return float4(0.f, 0.f, 0.f, 0.f);
    
    return color * bloom_Intensity / intensity;
}