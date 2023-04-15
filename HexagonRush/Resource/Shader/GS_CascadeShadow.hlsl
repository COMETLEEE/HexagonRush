#define MAX_CASCADE_COUNT 3

cbuffer CB_DIRLIGHTVIEWPROJ : register(b1)
{
    float4x4 _lightViewProjList[MAX_CASCADE_COUNT];
    
    float4 _cascadeEnds;
};

struct PS_OUT_TEX_ARRAY
{
    float4 pos : SV_POSITION;
    
    uint RTIndex : SV_RenderTargetArrayIndex;
};

struct VS_OUT
{
    float4 world : POSITION;
};

// 드로우 콜을 줄이기 위해서 기하 쉐이더를 응용해본당.
#ifdef USE_TRIANGLE
[maxvertexcount (3 * MAX_CASCADE_COUNT)]
void GS_CascadeShadow(triangle VS_OUT input[3],
    inout TriangleStream<PS_OUT_TEX_ARRAY> TriStream)
{
    PS_OUT_TEX_ARRAY output[3];
    
    for (uint i = 0; i < MAX_CASCADE_COUNT; i++)
    {
        for (uint j = 0; j < 3; j++)
        {
            output[j].pos = mul(input[j].world, _lightViewProjList[i]);

            output[j].RTIndex = i;
            
            TriStream.Append(output[j]);
        }
        
        TriStream.RestartStrip();
    }
}
#endif

#ifdef USE_LINE
[maxvertexcount(2 * MAX_CASCADE_COUNT)]
void GS_CascadeShadow(line VS_OUT input[2],
    inout LineStream<PS_OUT_TEX_ARRAY> lineStream)
{
    PS_OUT_TEX_ARRAY output[2];
    
    for (uint i = 0; i < MAX_CASCADE_COUNT; i++)
    {
        for (uint j = 0; j < 2; j++)
        {
            output[j].pos = mul(input[j].world, _lightViewProjList[i]);

            output[j].RTIndex = i;
            
            lineStream.Append(output[j]);
        }
        
        lineStream.RestartStrip();
    }
}
#endif