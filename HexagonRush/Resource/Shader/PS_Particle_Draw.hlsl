struct GS_OUT_PLANE
{
    float4 _pos : SV_Position;          // Clip Space Position
    
    float4 _color : COLOR;              // 생명 주기에 따른 알파값 테스팅 ..
    
    float2 _uv : TEXCOORD;
};

// 타이머를 통해서 뭔가 컨트롤할 수 있을 것 같아.
cbuffer CB_PARTICLE_TIMER : register(b1)
{
    float _deltaTime;
    
    float _totalTime;
    
    float2 _pad;
}

// 색상의 경향을 줄 수 있다.
cbuffer CB_PARTICLE_COLOR : register(b2)
{
    float4 _color;
}

// 파티클 텍스처 (랜덤 컬러 ? 정도면 예쁘지 않을까 .. 싶다 ..)
Texture2D Tex_Color : register(t0);

// Depth
Texture2D Tex_DMRAO : register(t1);

SamplerState Sam_Clamp : register(s0);

float4 PS_Particle_Draw(GS_OUT_PLANE input) : SV_TARGET
{
    // 텍스처의 색깔을 그대로 보존하지만, input의 시간에 따라 조절되는 
    // oppacity로 그려지는 것을 컨트롤한다.
    // 그냥 넣는게 예뻐서 감마 연산 안하고 넣음
    return Tex_Color.Sample(Sam_Clamp, input._uv) * input._color * _color;
}