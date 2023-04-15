struct GS_OUT_PLANE
{
    float4 _pos : SV_Position;          // Clip Space Position
    
    float4 _color : COLOR;              // ���� �ֱ⿡ ���� ���İ� �׽��� ..
    
    float2 _uv : TEXCOORD;
};

// Ÿ�̸Ӹ� ���ؼ� ���� ��Ʈ���� �� ���� �� ����.
cbuffer CB_PARTICLE_TIMER : register(b1)
{
    float _deltaTime;
    
    float _totalTime;
    
    float2 _pad;
}

// ������ ������ �� �� �ִ�.
cbuffer CB_PARTICLE_COLOR : register(b2)
{
    float4 _color;
}

// ��ƼŬ �ؽ�ó (���� �÷� ? ������ ������ ������ .. �ʹ� ..)
Texture2D Tex_Color : register(t0);

// Depth
Texture2D Tex_DMRAO : register(t1);

SamplerState Sam_Clamp : register(s0);

float4 PS_Particle_Draw(GS_OUT_PLANE input) : SV_TARGET
{
    // �ؽ�ó�� ������ �״�� ����������, input�� �ð��� ���� �����Ǵ� 
    // oppacity�� �׷����� ���� ��Ʈ���Ѵ�.
    // �׳� �ִ°� ������ ���� ���� ���ϰ� ����
    return Tex_Color.Sample(Sam_Clamp, input._uv) * input._color * _color;
}