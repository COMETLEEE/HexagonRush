cbuffer CB_PARTICLE_DRAW : register(b1)
{
    // ��� ���ӵ� (��ƼŬ�� �������� ��Ʈ���Ѵ�.)
    float3 _accel;
    
    // ����, ���� ��ʸ� ���ؼ� ����Ѵ� .. ���� ..
    float _lifeSpan;
};

struct VS_IN_PARTICLE
{
    float3 _initPosition : POSITION;
    
    float3 _initVelocity : VELOCITY;
    
    float2 _size : SIZE;
    
    float _age : AGE;
        
    uint _type : TYPE;                  // �������� ���ۿ� ����
};

struct VS_OUT
{
    float3 _world   : POSITION;
    
    float2 _size    : SIZE;
    
    float4 _color   : COLOR;
    
    uint _type      : TYPE;
};

// ���� ������ Vertex Shader���� �ǽ��Ѵ�.
VS_OUT VS_Particle_Draw(VS_IN_PARTICLE input)
{
    VS_OUT output;
    
    float t = input._age;
    
    // ��� ���ӵ��� ����� ����. => using Gravity � ���� .. accel�� �޶� �� ���� �ִ�.
    output._world = 0.5f * t * t * _accel + t * input._initVelocity + input._initPosition;
    
    // �ð��� ���� ������ ����
    // ���� �������δ� Age�� LifeSpan �Ѿ�� opacity 0�� �Ǵ� ��Ȳ��.
    float opacity = 1.f - smoothstep(0.f, 1.f, t / _lifeSpan);
    
    // �ؽ�ó�� .. ��� ?
    output._color = float4(1.f, 1.f, 1.f, opacity);
    
    output._size = input._size;
    
    output._type = input._type;
    
    return output;
}