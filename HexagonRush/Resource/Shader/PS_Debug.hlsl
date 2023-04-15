#define OUT_BUFFER_COUNT 6

static const float yStep = 0.2f;

static const float xStep = 1.f / OUT_BUFFER_COUNT;

static const float InvYStep = 1.f / yStep;

struct VS_OUT
{
    float4 pos : SV_POSITION;
    float4 color : COLOR;
    float2 uv : TEXCOORD;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float3 world : POSITION;
};

Texture2D DMRAO : register(t0);                 // Depth, Metallic, Roughness, Ambient Occlusion

Texture2D Normal : register(t1);                // Normal

Texture2D Position : register(t2);              // Position

Texture2D Albedo : register(t3);                // Albedo Color

Texture2D Emissive : register(t4);              // Emissive

SamplerState Sam_Clamp : register(s0);      

// 그냥 G-Buffer들 잘 나오는지 확인하기 위한 용도
float4 PS_Debug(VS_OUT input) : SV_TARGET
{
    float2 InUV = input.uv;
    
    // 1. Depth
    if ((InUV.x >= 0.f) && (InUV.x <= xStep))
    {
        return float4(DMRAO.Sample(Sam_Clamp, input.uv * float2(OUT_BUFFER_COUNT, 1.f)).xxx, 1.f);
    }
    // 2. Metallic
    else if ((InUV.x >= 1 * xStep) && (InUV.x <= 2 * xStep))
    {
        return float4(DMRAO.Sample(Sam_Clamp, (input.uv - float2(xStep, 0.f)) * float2(OUT_BUFFER_COUNT, 1.f)).yyy, 1.f);
    }
    // 3. Roughness
    else if (((InUV.x >= 2 * xStep) && (InUV.x <= 3 * xStep)))
    {
        return float4(DMRAO.Sample(Sam_Clamp, (input.uv - float2(2 * xStep, 0.f)) * float2(OUT_BUFFER_COUNT, 1.f)).zzz, 1.f);
    }
    // 4. Normal
    else if (((InUV.x >= 3 * xStep) && (InUV.x <= 4 * xStep)))
    {
        return float4(Normal.Sample(Sam_Clamp, (input.uv - float2(3 * xStep, 0.f)) * float2(OUT_BUFFER_COUNT, 1.f)).xyz, 1.f);
    }
    // 5. Albedo
    else if (((InUV.x >= 4 * xStep) && (InUV.x <= 5 * xStep)))
    {
        return float4(pow(Albedo.Sample(Sam_Clamp, (input.uv - float2(4 * xStep, 0.f)) * float2(OUT_BUFFER_COUNT, 1.f)).xyz, 1 / 2.2f), 1.f);
    }
    // 6. Emissive
    else if (((InUV.x >= 5 * xStep) && (InUV.x <= 6 * xStep)))
    {
        return float4(Emissive.Sample(Sam_Clamp, (input.uv - float2(5 * xStep, 0.f)) * float2(OUT_BUFFER_COUNT, 1.f)).xyz, 1.f);
    }
    
    return float4(0.f, 0.f, 0.f, 1.f);
}