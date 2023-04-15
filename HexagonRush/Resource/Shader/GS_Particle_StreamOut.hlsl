#include "ConstantBuffer.hlsli"

static const float Pi = 3.141592f;

static const float TwoPi = 3.141592f * 2.f;

#define PT_EMITTER (uint) 0
#define PT_FLARE (uint) 1

cbuffer CB_PARTICLE_STREAMOUT : register(b1)
{
    float3 _emitVelocity;
    
    float _radius;
    
    float2 _averageSize;
    
    float _emitTime;
    
    float _lifeSpan;
    
    float _period; // 주기성 (몇 초에 한 번 똑같은 시뮬레이션 ?)
    
    float _randNumber;
    
    float2 _paddd;
    
    float4x4 _transformMatrix;
};

cbuffer CB_PARTICLE_TIMER : register(b2)
{
    float _deltaTime;
    
    float _totalTime;
    
    float2 _padd;
}
    
struct VS_IN_PARTICLE
{
    float3 _initPosition : POSITION;
    
    float3 _initVelocity : VELOCITY;
    
    float2 _size : SIZE;
    
    float _age : AGE;
    
    uint _type : TYPE;
};
  
// 랜덤 벡터들이 담긴 텍스처
Texture1D Texture_Random : register(t0);

SamplerState Sam_Clamp : register(s0);

// Real Distribution으로 RandNumber를 받자.
float3 RandUnitVec3(float offset)
{
    float u = _randNumber + offset;
    
    float3 vec = Texture_Random.SampleLevel(Sam_Clamp, u, 0).xyz;
    
    return normalize(vec);
}

// 점들을 랜덤으로 만들어서 Stream Output에 던짐 !
// 새 입자의 방출과 기존 입자의 파괴만 담당한다.
// 분명히 입자 시스템마다 규칙이 다를꺼니까 .. 여기도 다를 것임.
// 여기서 입자의 생애가 정해짐
[maxvertexcount(2)]
void GS_Particle_StreamOut(point VS_IN_PARTICLE input[1],
    inout PointStream<VS_IN_PARTICLE> ptStream)
{
// 구의 궤적
#ifdef USE_SPHERE
    input[0]._age += _deltaTime;
    
    // 방출기라면 ..a
    if (input[0]._type == PT_EMITTER)
    {
        // 방출 타이밍이 되었는가 => 기하 쉐이더 타는 것과 관련하여 EmitTime < FrameTime이면 issue. 가산해줄 필요가 있다.
        if (input[0]._age > _emitTime)
        {
            float3 vRandom = RandUnitVec3(0.f);
            vRandom.x *= 0.5f;
            vRandom.y *= 0.5f;
            
            VS_IN_PARTICLE p;
            
            p._initPosition = mul(float4(_radius * sin(TwoPi * _totalTime / _period) * cos(TwoPi * _totalTime / (_period * vRandom.y)),
             _radius * sin(TwoPi * _totalTime / _period) * sin(TwoPi * _totalTime / (_period * vRandom.y)),
            _radius * cos(TwoPi * _totalTime / _period), 1.f), _transformMatrix);
            
            // p._initVelocity = 10.0f * abs(vRandom.x) * _emitVelocity;
            
            // 구 폭발 특 : 구 모양으로 산개하는 듯한 모양
            p._initVelocity = 10.0f * normalize((p._initPosition - mul(float4(0.f, 0.f, 0.f, 1.f), _transformMatrix).xyz)) * length(_emitVelocity);
            
            // Averaga Size를 어떻게 흔들 수 없나 .. 일단 Average Size를 넣어보자.
            // 표준 편차를 받아서 노말 디스트리뷰션으로 파티클을 박을까 ..?
            
            
            p._size = float2(_averageSize.x, _averageSize.y);
            
            p._age = 0.0f;
            
            p._type = PT_FLARE;
            
            ptStream.Append(p);
            
            // 방출 시간을 재설정한다.
            input[0]._age = 0.f;
        }
        
        // 방출기 입자는 항상 유지한다.
        ptStream.Append(input[0]);
    }
    // 보이는 파티클 이펙트 입자라면 ..
    else
    {
        // 여기서 입자의 유지 조건을 지정한다.
        // 기본적으로 수명보다 짧으면 유지한다.
        if (input[0]._age <= _lifeSpan)
            ptStream.Append(input[0]);
    }
#endif
    
// 원의 궤적
#ifdef USE_CIRCLE
     input[0]._age += _deltaTime;
    
    // 방출기라면 ..a
    if (input[0]._type == PT_EMITTER)
    {
        // 방출 타이밍이 되었는가 => 기하 쉐이더 타는 것과 관련하여 EmitTime < FrameTime이면 issue. 가산해줄 필요가 있다.
        if (input[0]._age > _emitTime)
        {    
            float3 vRandom = RandUnitVec3(0.f);
            vRandom.x *= 0.5f;
            vRandom.y *= 0.5f;
            
            VS_IN_PARTICLE p;
            
            p._initPosition = mul(float4(_radius * cos(TwoPi * _totalTime / _period),
             _radius * sin(TwoPi * _totalTime / _period),
            0.f, 1.f), _transformMatrix);
            
            p._initVelocity = 10.0f * abs(vRandom.x) * _emitVelocity;
            
            // Averaga Size를 어떻게 흔들 수 없나 .. 일단 Average Size를 넣어보자.
            // 표준 편차를 받아서 노말 디스트리뷰션으로 파티클을 박을까 ..?
            p._size = float2(_averageSize.x, _averageSize.y);
            
            p._age = 0.0f;
            
            p._type = PT_FLARE;
            
            ptStream.Append(p);
            
            // 방출 시간을 재설정한다.
            input[0]._age = 0.f;
        }
        
        // 방출기 입자는 항상 유지한다.
        ptStream.Append(input[0]);
    }
    // 보이는 파티클 이펙트 입자라면 ..
    else
    {
        // 여기서 입자의 유지 조건을 지정한다.
        // 기본적으로 수명보다 짧으면 유지한다.
        if (input[0]._age <= _lifeSpan)
            ptStream.Append(input[0]);
    }
#endif
}