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

VS_OUT VS_Quad(VS_IN input)
{
    VS_OUT output;
    
    // Quad : 가장 앞으로 나와야 한다.
    output.pos = float4(input.pos, 1.0f).xyww;
    
    output.uv = input.uv;

    return output;
}