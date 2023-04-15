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
    float4 posH : SV_Position;
    float3 posL : POSITON;
    float4 posW : POSITION2;
    
    uint objectID : OBJECTID;
};

VS_OUT VS_SkyBox(VS_IN input)
{
    VS_OUT output;
    
    // 스카이 박스, 큐브가 가장 먼 깊이에 있도록 조작
    output.posH = mul(float4(input.pos, 1.0f), _worldViewProj).xyww;
    
    output.posL = input.pos;
    
    output.posW = mul(float4(input.pos, 1.0f), _world);
    
    output.objectID = _objectID;
    
    return output;
}