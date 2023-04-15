#include "ConstantBuffer.hlsli"

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
    float2 uv : TEXCOORD1;
    float3 toFarPlane : TEXCOORD0;
};

VS_OUT VS_SSAO(VS_IN input)
{
    VS_OUT output;
    
    output.pos = float4(input.pos, 1.0f).xyww;
    
    output.uv = input.uv;
    
    int indexFromUV = (int) input.uv.x + 2 * (int) input.uv.y;
    
    output.toFarPlane = _ssaoInfo._viewFrustumCorners[indexFromUV].xyz;
    
    return output;
}