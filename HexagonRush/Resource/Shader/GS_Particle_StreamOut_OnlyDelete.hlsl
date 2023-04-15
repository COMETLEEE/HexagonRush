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
    
    float _period; // �ֱ⼺ (�� �ʿ� �� �� �Ȱ��� �ùķ��̼� ?)
    
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

// ��Ʈ�� �ƿ��� �� ������ �����.
[maxvertexcount(2)]
void GS_Particle_StreamOut_OnlyDelete(point VS_IN_PARTICLE input[1],
    inout PointStream<VS_IN_PARTICLE> ptStream)
{
    input[0]._age += _deltaTime;
    
    // ���⼭ ������ ���� ������ �����Ѵ�.
    // �⺻������ ������ ª���� �����Ѵ�.
    // ����, ������ �� ���� ���ٸ� �����. (���ؽ� ���۸� ���� �����ϴ� ���̴� �̷� ������ �ʿ��ϴ�.)
    if (input[0]._age < _lifeSpan)
    {
        ptStream.Append(input[0]);
    }
}