struct VS_OUT
{
    float4 pos : SV_POSITION;
};

float4 PS_Shadow(VS_OUT input) : SV_Target
{   
    // 깊이를 한 장의 Texture로 출력 (아직까지는 할 일이 따로 없다. 깊이값만 쓰고 렌더 타겟을 안 써서 ..)
    return float4(input.pos.zzz, 1.f);
}