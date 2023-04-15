cbuffer CB_PARTICLE_DRAW : register(b1)
{
    // 상수 가속도 (파티클의 움직임을 컨트롤한다.)
    float3 _accel;
    
    // 수명, 투명도 비례를 위해서 사용한다 .. 정도 ..
    float _lifeSpan;
};

struct VS_IN_PARTICLE
{
    float3 _initPosition : POSITION;
    
    float3 _initVelocity : VELOCITY;
    
    float2 _size : SIZE;
    
    float _age : AGE;
        
    uint _type : TYPE;                  // 만들어놓는 수밖에 없엉
};

struct VS_OUT
{
    float3 _world   : POSITION;
    
    float2 _size    : SIZE;
    
    float4 _color   : COLOR;
    
    uint _type      : TYPE;
};

// 물리 갱신을 Vertex Shader에서 실시한다.
VS_OUT VS_Particle_Draw(VS_IN_PARTICLE input)
{
    VS_OUT output;
    
    float t = input._age;
    
    // 상수 가속도인 경우의 공식. => using Gravity 등에 따라서 .. accel이 달라 질 수도 있다.
    output._world = 0.5f * t * t * _accel + t * input._initVelocity + input._initPosition;
    
    // 시간에 따른 색상의 감소
    // 지금 수식으로는 Age가 LifeSpan 넘어가면 opacity 0이 되는 상황임.
    float opacity = 1.f - smoothstep(0.f, 1.f, t / _lifeSpan);
    
    // 텍스처는 .. 어떻게 ?
    output._color = float4(1.f, 1.f, 1.f, opacity);
    
    output._size = input._size;
    
    output._type = input._type;
    
    return output;
}