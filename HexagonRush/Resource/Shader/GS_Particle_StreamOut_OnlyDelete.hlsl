#include "ConstantBuffer.hlsli"

static const float Pi = 3.141592f;

static const float TwoPi = 3.141592f * 2.f;

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

// 스트림 아웃할 때 모조리 지운다.
[maxvertexcount(2)]
void GS_Particle_StreamOut_OnlyDelete(point VS_IN_PARTICLE input[1],
    inout PointStream<VS_IN_PARTICLE> ptStream)
{
    input[0]._age += _deltaTime;
    
    // 여기서 입자의 유지 조건을 지정한다.
    // 기본적으로 수명보다 짧으면 유지한다.
    // 만약, 수명보다 더 많이 갔다면 지운다. (버텍스 버퍼를 직접 접근하는 것이니 이런 관리가 필요하다.)
    if (input[0]._age < _lifeSpan)
    {
        ptStream.Append(input[0]);
    }
}