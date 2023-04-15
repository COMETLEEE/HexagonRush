cbuffer CB_COLOR : register(b1)
{
    float4 _color;
};

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

SamplerState Sam_Clamp : register(s0);

// ���� �ؽ�ó�� ���İ����� ���� ���ø��ϰ� 
// ����� ���� ���� ������ �����Ѵ�.
float4 PS_Sample_Color(VS_OUT input) : SV_TARGET
{
    float4 color = (float4) 0;
    
    color = Texture_Sample.Sample(Sam_Clamp, input.uv);
        
    color = float4(color.x * _color.x, color.y * _color.y,
        color.z * _color.z, color.w * _color.w);
    
    return color;
}