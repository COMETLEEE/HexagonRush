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
  
// ���� ���͵��� ��� �ؽ�ó
Texture1D Texture_Random : register(t0);

SamplerState Sam_Clamp : register(s0);

// Real Distribution���� RandNumber�� ����.
float3 RandUnitVec3(float offset)
{
    float u = _randNumber + offset;
    
    float3 vec = Texture_Random.SampleLevel(Sam_Clamp, u, 0).xyz;
    
    return normalize(vec);
}

// ������ �������� ���� Stream Output�� ���� !
// �� ������ ����� ���� ������ �ı��� ����Ѵ�.
// �и��� ���� �ý��۸��� ��Ģ�� �ٸ����ϱ� .. ���⵵ �ٸ� ����.
// ���⼭ ������ ���ְ� ������
[maxvertexcount(2)]
void GS_Particle_StreamOut(point VS_IN_PARTICLE input[1],
    inout PointStream<VS_IN_PARTICLE> ptStream)
{
// ���� ����
#ifdef USE_SPHERE
    input[0]._age += _deltaTime;
    
    // ������� ..a
    if (input[0]._type == PT_EMITTER)
    {
        // ���� Ÿ�̹��� �Ǿ��°� => ���� ���̴� Ÿ�� �Ͱ� �����Ͽ� EmitTime < FrameTime�̸� issue. �������� �ʿ䰡 �ִ�.
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
            
            // �� ���� Ư : �� ������� �갳�ϴ� ���� ���
            p._initVelocity = 10.0f * normalize((p._initPosition - mul(float4(0.f, 0.f, 0.f, 1.f), _transformMatrix).xyz)) * length(_emitVelocity);
            
            // Averaga Size�� ��� ��� �� ���� .. �ϴ� Average Size�� �־��.
            // ǥ�� ������ �޾Ƽ� �븻 ��Ʈ��������� ��ƼŬ�� ������ ..?
            
            
            p._size = float2(_averageSize.x, _averageSize.y);
            
            p._age = 0.0f;
            
            p._type = PT_FLARE;
            
            ptStream.Append(p);
            
            // ���� �ð��� �缳���Ѵ�.
            input[0]._age = 0.f;
        }
        
        // ����� ���ڴ� �׻� �����Ѵ�.
        ptStream.Append(input[0]);
    }
    // ���̴� ��ƼŬ ����Ʈ ���ڶ�� ..
    else
    {
        // ���⼭ ������ ���� ������ �����Ѵ�.
        // �⺻������ ������ ª���� �����Ѵ�.
        if (input[0]._age <= _lifeSpan)
            ptStream.Append(input[0]);
    }
#endif
    
// ���� ����
#ifdef USE_CIRCLE
     input[0]._age += _deltaTime;
    
    // ������� ..a
    if (input[0]._type == PT_EMITTER)
    {
        // ���� Ÿ�̹��� �Ǿ��°� => ���� ���̴� Ÿ�� �Ͱ� �����Ͽ� EmitTime < FrameTime�̸� issue. �������� �ʿ䰡 �ִ�.
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
            
            // Averaga Size�� ��� ��� �� ���� .. �ϴ� Average Size�� �־��.
            // ǥ�� ������ �޾Ƽ� �븻 ��Ʈ��������� ��ƼŬ�� ������ ..?
            p._size = float2(_averageSize.x, _averageSize.y);
            
            p._age = 0.0f;
            
            p._type = PT_FLARE;
            
            ptStream.Append(p);
            
            // ���� �ð��� �缳���Ѵ�.
            input[0]._age = 0.f;
        }
        
        // ����� ���ڴ� �׻� �����Ѵ�.
        ptStream.Append(input[0]);
    }
    // ���̴� ��ƼŬ ����Ʈ ���ڶ�� ..
    else
    {
        // ���⼭ ������ ���� ������ �����Ѵ�.
        // �⺻������ ������ ª���� �����Ѵ�.
        if (input[0]._age <= _lifeSpan)
            ptStream.Append(input[0]);
    }
#endif
}