#include "ConstantBuffer.hlsli"

struct PS_OUT_TEX_ARRAY
{
    float4 pos : SV_POSITION;
    
    uint RTIndex : SV_RenderTargetArrayIndex;
};

float4 PS_CascadeShadow(PS_OUT_TEX_ARRAY input)
{   
    // 없음. 나중에 생길 수도 있으니 파일을 만들어 둡니당.
}