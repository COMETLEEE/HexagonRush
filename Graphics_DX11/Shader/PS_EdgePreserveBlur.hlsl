#include "ConstantBuffer.hlsli"

#define BLUR_SAMPLE_COUNT 5

static const float g_Weights[BLUR_SAMPLE_COUNT] =
{
        0.1f,
        0.2f,
        0.4f,
        0.2f,
        0.1f,
};

static const float g_Indices[BLUR_SAMPLE_COUNT] = { -2, -1, 0, +1, +2 };

struct VS_OUT
{
    float4 pos : SV_POSITION;
    float4 color : COLOR;
    float2 uv : TEXCOORD;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float3 world : POSITION;
};

Texture2D Texture_Depth : register(t0);

Texture2D Texture_Normal : register(t1);

Texture2D Texture_Target : register(t2);

SamplerState Sam_Clamp : register(s0);    

float4 PS_EdgePreserveBlur_X(VS_OUT input) : SV_TARGET
{
    float2 dir = float2(1.0f, 0.f);
    
    float2 step = float2(dir.x * _textureInfo.z, dir.y * _textureInfo.w);
    
    // G Buffer는 텍셀 사이즈가 다릅니다.
    float2 gStep = float2(dir.x * _screenInfo.z, dir.y * _screenInfo.w);
    
    // 가운데. 타겟 픽셀의 색은 보존한다.
    float4 color = Texture_Target.Sample(Sam_Clamp, input.uv) * 
                g_Weights[(int) (BLUR_SAMPLE_COUNT / 2) + (BLUR_SAMPLE_COUNT % 2)];

    float totalWeight = (float) g_Weights[(int) (BLUR_SAMPLE_COUNT / 2) + (BLUR_SAMPLE_COUNT % 2)];
    
    float3 targetPixelNormal = Texture_Normal.Sample(Sam_Clamp, input.uv).xyz;
    
    float targetPixelDepth = Texture_Depth.Sample(Sam_Clamp, input.uv).x;
    
    for (int i = 0; i < BLUR_SAMPLE_COUNT; i++)
    {
        if (i == 2)
            continue;
        
        float2 neighborGStep = input.uv + g_Indices[i] * gStep;
        
        float3 neighborNormal = Texture_Normal.Sample(Sam_Clamp, neighborGStep);
        
        float neighborDepth = Texture_Depth.Sample(Sam_Clamp, neighborGStep);
        
        if (dot(neighborNormal, targetPixelNormal) >= 0.8f && abs(neighborDepth - targetPixelDepth) <= 0.2f)
        {
            float2 neighborStep = input.uv + g_Indices[i] * step;

            color += Texture_Target.Sample(Sam_Clamp, neighborStep) * g_Weights[i];
            
            totalWeight += g_Weights[i];
        }
    }
    
    return float4(color.xyz / totalWeight, 1.f);
}

float4 PS_EdgePreserveBlur_Y(VS_OUT input) : SV_TARGET
{
    float2 dir = float2(0.f, 1.f);
    
    float2 step = float2(dir.x * _textureInfo.z, dir.y * _textureInfo.w);
    
    // G Buffer는 텍셀 사이즈가 다릅니다.
    float2 gStep = float2(dir.x * _screenInfo.z, dir.y * _screenInfo.w);
    
    // 가운데. 타겟 픽셀의 색은 보존한다.
    float4 color = Texture_Target.Sample(Sam_Clamp, input.uv) *
                g_Weights[(int) (BLUR_SAMPLE_COUNT / 2) + (BLUR_SAMPLE_COUNT % 2)];
    
    float totalWeight = (float) g_Weights[(int) (BLUR_SAMPLE_COUNT / 2) + (BLUR_SAMPLE_COUNT % 2)];
    
    float3 targetPixelNormal = Texture_Normal.Sample(Sam_Clamp, input.uv).xyz;
    
    float targetPixelDepth = Texture_Depth.Sample(Sam_Clamp, input.uv).x;

    for (int i = 0; i < BLUR_SAMPLE_COUNT; i++)
    {
        if (i == 2)
            continue;
        
        float2 neighborGStep = input.uv + g_Indices[i] * gStep;
        
        float3 neighborNormal = Texture_Normal.Sample(Sam_Clamp, neighborGStep);
        
        float neighborDepth = Texture_Depth.Sample(Sam_Clamp, neighborGStep);
        
        if (dot(neighborNormal, targetPixelNormal) >= 0.8f && abs(neighborDepth - targetPixelDepth) <= 0.2f)
        {
            float2 neighborStep = input.uv + g_Indices[i] * step;
            
            color += Texture_Target.Sample(Sam_Clamp, neighborStep) * g_Weights[i];
            
            totalWeight += g_Weights[i];
        }
    }
   
    return float4(color.xyz / totalWeight, 1.f);
}