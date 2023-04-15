#pragma once
#include "Graphics_DX11_typedef.h"

// 내 실력의 한계로 인해 일반적인 팩터들을 이용한 물리 시물레이션 구현은 어려울 것 같다 ..
// 그냥 여러 타입을 나눠서 해당 타입의 방출 조건대로 시뮬레이션을 하도록 하자.
enum class PARTICLE_EFFECT_TYPE
{
	CIRCLE,
	SPHERE,
};

struct Particle_InitInfo
{
	Particle_InitInfo() : _particleEffectType(PARTICLE_EFFECT_TYPE::CIRCLE), _firstRun(true), _isLoop(true),
		_duration(5.f) ,_particlePlayID(ULLONG_MAX), _maxParticleCount(500)
	{}

	PARTICLE_EFFECT_TYPE _particleEffectType;	// 그리는데 사용하는 쉐이더가 다를 것 같고 ..
												// 모델이 달라지면 Particle 구조체 또한 달라질 수 있다.

	bool _firstRun;					// 시작인가요 ..?

	bool _isLoop;					// 계속 반복하나요 ? => False이면 

	float _duration;				// 지속 시간 (isLoop False이면 이 시간만큼만 진행 후 삭제

	uint32 _maxParticleCount;		// 최대 파티클의 갯수

	uint64 _particlePlayID;			// First Run을 할 때 부여받음. 이 녀석으로 Curr와 연결됨.
};

// 초기 속도, 초기 위치, 상수 가속도 (시간에 대한 함수도 가능할지도 .. ?!) 에 의해서 결정되는 모듈
struct Particle_ShapeInfo
{
	Particle_ShapeInfo() :  _transformMatrix(Matrix::Identity), _emitVelocity(Vector3::Up), _radius(5.f), _emitTime(0.005f), _period(0.2f), 
		_particleAverageSize(Vector2(0.5f, 0.5f)), _deviation(Vector2(0.001f, 0.001f)), _useXYSameDeviation(true), _refTextureID(ULLONG_MAX), _acceleration(Vector3(0.f, 3.f, 0.f)), _lifeSpan(1.5f), _color(Vector4(1.f, 1.f, 1.f, 1.f))
	{}

	// Stream - Output Info	
	Vector3 _emitVelocity;			// 방출하는 속도

	Matrix _transformMatrix;		// 파티클의 생성 위치 및 각을 조정

	float _radius;					// 반지름 (_emitPosition에서 _radius만큼 떨어진 곳에서 입자가 생성된다.)

	float _emitTime;				// 입자 방출에 걸리는 시간 (밀도 정도가 될 듯)

	float _period;					// 주기 (몇 초의 주기로 비슷한 궤적의 모션을 그릴지)

	Vector2 _particleAverageSize;	// Particle의 평균 사이즈.

	Vector2 _deviation;				// X와 Y 각각 편차

	bool _useXYSameDeviation;		// X와 Y 똑같은 편차를 사용하냐는 옵션 => 같은 크기로 나와야하는 파티클이 다른 편차를 사용하면 파티클의 생김새가 무너질 수도 있기 떄문.

	uint64 _refTextureID;			// 파티클 이펙트가 사용하는 아이디 (여러 개를 참조하는 경우도 있을텐데 ..)

	Vector3 _acceleration;			// 상수 가속도

	float _lifeSpan;				// 파티클 입자 간 수명

	Vector4 _color;					// 파티클의 색상 정보 경향
};

// 파티클 시스템을 사용하기 위한 인터페이스
// 한 개의 입자 시스템을 묘사한다.
struct RenderingData_Particle
{
	RenderingData_Particle() : _initInfo(std::make_shared<Particle_InitInfo>()), _shapeInfo(std::make_shared<Particle_ShapeInfo>())
	{}

	std::shared_ptr<Particle_InitInfo> _initInfo;

	std::shared_ptr<Particle_ShapeInfo> _shapeInfo;
};