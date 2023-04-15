#pragma once
#include "Graphics_RenderingData_3D.h"
#include "Graphics_PerFrameData.h"

namespace GraphicsEngine
{
	// 2의 31승
	constexpr uint32 LIGHT_CULL_MASK = static_cast<uint32>(INT_MAX) + 1;

	// 1 프레임 당 1회 업데이트하는 Constant Buffer
	__declspec(align(16)) struct CB_PERFRAME
	{
		CameraInfo _cameraInfo;

		uint32 _directionalLightCount;

		Vector3 _pad1;

		DirectionalLightInfo _directionalLightInfos[LIGHT_INFO_MAX];

		uint32 _spotLightCount;

		Vector3 _pad2;

		SpotLightInfo _spotLightInfos[LIGHT_INFO_MAX];

		uint32 _pointLightCount;

		Vector3 _pad3;

		PointLightInfo _pointLightInfos[LIGHT_INFO_MAX];

		// Screen Space Ambient Occlusion
		SSAOInfo _ssaoInfo;
	};

#pragma region Legacy Lighting Model
	/*__declspec(align(16)) struct CB_PEROBJECT
	{
		Matrix _world;

		Matrix _worldInvTranspose;

		Matrix _worldViewProj;

		Matrix _texTransform;

		Vector4 _ambient;

		Vector4 _diffuse;

		Vector4 _specular;

		Vector4 _reflect;

		Matrix _boneMatrix[30];

		uint32 _objectID;
	};*/
#pragma endregion

#pragma region PBR Lighting Model
	__declspec(align(16)) struct CB_PEROBJECT
	{
		Matrix _world;

		Matrix _worldInvTranspose;

		Matrix _worldViewProj;

		Matrix _texTransform;

		float _metallic;

		float _roughtness;

		Vector2 _pad;

		Vector4 _emissive;

		Matrix _boneMatrix[70];

		uint32 _objectID;
	}; 
#pragma endregion

	__declspec(align(16)) struct CB_PERTEXTURE
	{
		// x : Width - y : Height - z : 1 / Width - w : 1 / Height
		Vector4 _screenInfo;

		Vector4 _textureInfo;
	};

	constexpr uint32 FRUSTUM_CASCADE_COUNT = 3;

	__declspec(align(16)) struct CB_DIRLIGHTVIEWPROJ
	{
		// 각 케스케이드 별 View Proj Mat
		Matrix _lightViewProjList[FRUSTUM_CASCADE_COUNT];

		// 각 케스케이드의 끝 (Far) => 이것을 기준으로 어느 케스케이드에 속하는지
		// 계산한다. 쉐이더에서는 Float4로 받아요 !
		float _cascadeEnds[FRUSTUM_CASCADE_COUNT];

		float _pad;
	};

	__declspec(align(16)) struct CB_LIGHTVIEWPROJ
	{
		Matrix _lightViewProj;
	};

	__declspec(align(16)) struct CB_TONEMAPPING
	{
		float _exposure;

		Vector3 _pad;
	};

	// In IBL, Specular Pre-Filtered Map을 만들 때 사용하는 Constant Buffer입니다.
	// Roughness에 따른 참조 레벨 별 스페큘러 밉맵을 만들기 위하여
	__declspec(align(16)) struct CB_SPECMAPFILTER
	{
		float _roughness;

		Vector3 _pad;
	};

	// ----------------------- Particle System -----------------------
	__declspec(align(16)) struct CB_PARTICLE_STREAMOUT			// 스트림 아웃 페이즈 때 필요한 정보
	{
		Vector3 _emitVelocity;		// 입자가 방출되는 속도
    
		float _radius;				// 반지름 
    
		Vector2 _averageSize;		// 평균 사이즈
    
		float _emitTime;			// 입자를 방출하는 시간
    
		float _lifeSpan;			// 입자를 죽이는 시간

		float _period;				// 같은 행동을 반복하는 주기 (몇 초에 한 번 반복)

		float _randNumber;			// 랜덤 배리어블

		Vector2 _pad;

		Matrix _transformMatrix;	// World Rotation Matrix => Look 벡터를 기준으로 파티클이 생성되고 그것을 이 행렬로 Transform 한다.
	};
	
	__declspec(align(16)) struct CB_PARTICLE_DRAW				// 입자를 그리는 페이즈 때 필요한 정보
	{
		// 상수 가속도, 상수 가속도를 가지는 모델만 시뮬레이션으로 사용해보자.
		Vector3 _acceleration;

		float _lifeSpan;			// 입자를 죽이는 시간
	};

	__declspec(align(16)) struct CB_PARTICLE_TIMER
	{
		CB_PARTICLE_TIMER(): _deltaTime(0.f), _totalTime(0.f)
		{

		}

		float _deltaTime;		// 물리 시뮬레이션이다보니 시간이 중요하다.

		float _totalTime;		// 토탈 타임은 주기성을 이용하고 싶을 때 잘 쓸 수 있을 것 같아서 야미 ~

		Vector2 _pad;
	};

	__declspec(align(16)) struct CB_PARTICLE_COLOR
	{
		CB_PARTICLE_COLOR() : _color(Vector4(1.f, 1.f, 1.f, 1.f))
		{}

		Vector4 _color;
	};

	// ----------------------- UI -----------------------
	__declspec(align(16)) struct CB_COLOR
	{
		CB_COLOR() : _color(Vector4(1.f, 1.f, 1.f ,1.f))
		{}

		Vector4 _color;
	};
}