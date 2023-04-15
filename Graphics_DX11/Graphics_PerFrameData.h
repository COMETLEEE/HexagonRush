#pragma once
#include "Graphics_DX11_typedef.h"
#include <random>

#pragma region Legacy Lighting Model
//struct CameraInfo
//{
//	CameraInfo() : _cameraWorldPosition(Vector3::Zero), _viewMatrix(Matrix::Identity), _projMatrix(Matrix::Identity),
//		_prevViewMatrix(Matrix::Identity), _prevProjMatrix(Matrix::Identity)
//	{}
//
//	Vector3 _cameraWorldPosition;
//
//private:
//	float _pad;
//
//public:
//	Matrix _viewMatrix;
//
//	Matrix _projMatrix;
//
//	Matrix _prevViewMatrix;
//
//	Matrix _prevProjMatrix;
//};
//
//struct LightInfo
//{
//	LightInfo() : _ambient(Vector4(0.4f, 0.4f, 0.4f, 1.f)), _diffuse(Vector4(0.4f, 0.4f, 0.4f, 1.f)),
//		_specular(Vector4(0.4f, 0.4f, 0.4f, 1.f))
//	{}
//
//	Vector4 _ambient;
//	Vector4 _diffuse;
//	Vector4 _specular;
//};
//
//struct DirectionalLightInfo final : public LightInfo
//{
//	DirectionalLightInfo() : LightInfo(), _direction(Vector3(1.f, 0.f, 0.f)), _pad(0.f)
//	{}
//
//	// World Rotation Look Vector
//	Vector3 _direction;
//
//private:
//	float _pad;
//};
//
//struct PointLightInfo final : public LightInfo
//{
//	PointLightInfo() : LightInfo(), _position(Vector3(0.f, 0.f, 0.f)),
//		_range(25.f), _attenuation(Vector3(0.f, 0.1f, 0.f)), _pad(0.f)
//	{}
//
//	Vector3 _position;
//	float _range;
//	Vector3 _attenuation;
//
//private:
//	float _pad;
//};
//
//struct SpotLightInfo final : public LightInfo
//{
//	SpotLightInfo() : LightInfo(), _position(Vector3(0.f, 0.f, 0.f)),
//		_range(25.f), _direction(Vector3(1.f, 0.f, 0.f)), _spot(10.f), _attenuation(Vector3(0.f, 0.1f, 0.f)), _pad(0.f)
//	{}
//
//	Vector3 _position;
//	float _range;
//	Vector3 _direction;
//	float _spot;
//	Vector3 _attenuation;
//
//private:
//	float _pad;
//};
//
//struct SSAOInfo final
//{
//	SSAOInfo() : _occlusionRadius(0.5f), _occlusionFadeStart(0.2f),
//		_occlusionFadeEnd(2.f), _surfaceEpsilon(0.05f), _sampleCount(14)
//	{
//		// 8 cube corners
//		_offsetVectors[0] = Vector4(+1.0f, +1.0f, +1.0f, 0.0f);
//		_offsetVectors[1] = Vector4(-1.0f, -1.0f, -1.0f, 0.0f);
//		_offsetVectors[2] = Vector4(-1.0f, +1.0f, +1.0f, 0.0f);
//		_offsetVectors[3] = Vector4(+1.0f, -1.0f, -1.0f, 0.0f);
//		_offsetVectors[4] = Vector4(+1.0f, +1.0f, -1.0f, 0.0f);
//		_offsetVectors[5] = Vector4(-1.0f, -1.0f, +1.0f, 0.0f);
//		_offsetVectors[6] = Vector4(-1.0f, +1.0f, -1.0f, 0.0f);
//		_offsetVectors[7] = Vector4(+1.0f, -1.0f, +1.0f, 0.0f);
//
//		// 6 ters of cube faces
//		_offsetVectors[8] = Vector4(-1.0f, 0.0f, 0.0f, 0.0f);
//		_offsetVectors[9] = Vector4(+1.0f, 0.0f, 0.0f, 0.0f);
//		_offsetVectors[10] = Vector4(0.0f, -1.0f, 0.0f, 0.0f);
//		_offsetVectors[11] = Vector4(0.0f, +1.0f, 0.0f, 0.0f);
//		_offsetVectors[12] = Vector4(0.0f, 0.0f, -1.0f, 0.0f);
//		_offsetVectors[13] = Vector4(0.0f, 0.0f, +1.0f, 0.0f);
//
//		std::random_device rd;
//
//		std::mt19937 gen(rd());
//
//		std::uniform_real_distribution<float> dist(0.25f, 1.f);
//
//		for (int i = 0; i < 14; ++i)
//		{
//			// 0.25 ~ 1 사이의 Random Length
//			float s = dist(gen);
//
//			_offsetVectors[i].Normalize();
//
//			_offsetVectors[i] = s * _offsetVectors[i];
//		}
//	}
//
//	// 최대 14개의 Sampling을 지원함
//	Vector4 _offsetVectors[14];
//
//	// Left Top, Right Top, Left Bottom, Right Bottom
//	Vector4 _cameraFrustumCorner[4];
//
//	float _occlusionRadius;
//
//	float _occlusionFadeStart;
//
//	float _occlusionFadeEnd;
//
//	float _surfaceEpsilon;
//
//	uint32 _sampleCount;
//
//	Vector3 _pad;
//};
//
//constexpr uint32 LIGHT_INFO_MAX = 100;
//
//enum class POSTPROCESS_OPTION
//{
//	ON_SSAO =				0b00000001,
//	ON_GAUSSIAN_BLUR =		0b00000010,
//	ON_CAM_BLUR =			0b00000100,
//	ON_BLOOM =				0b00001000,
//	ON_FXAA =				0b00010000
//};
//
//struct PerFrameData
//{
//	PerFrameData() : _cameraInfo(std::make_shared<CameraInfo>()),
//		_directionalLightInfos(std::vector<std::shared_ptr<DirectionalLightInfo>>()),
//		_spotLightInfos(std::vector<std::shared_ptr<SpotLightInfo>>()),
//		_pointLightInfos(std::vector<std::shared_ptr<PointLightInfo>>()),
//		_ssaoInfo(std::make_shared<SSAOInfo>()),
//		_postProcessOption(static_cast<POSTPROCESS_OPTION>(static_cast<uint32>(POSTPROCESS_OPTION::ON_FXAA) | static_cast<uint32>(POSTPROCESS_OPTION::ON_CAM_BLUR)))
//	{}
//
//	std::shared_ptr<CameraInfo> _cameraInfo;
//
//	std::vector<std::shared_ptr<DirectionalLightInfo>> _directionalLightInfos;
//
//	std::vector<std::shared_ptr<SpotLightInfo>> _spotLightInfos;
//
//	std::vector<std::shared_ptr<PointLightInfo>> _pointLightInfos;
//
//	// 각종 포스트 프로세스 인포가 있어야하는데 .. 사실 이거 SSAOInfo도 소멸시키고 싶다 그냥 ..
//	std::shared_ptr<SSAOInfo> _ssaoInfo;
//
//	POSTPROCESS_OPTION _postProcessOption;
//};
#pragma endregion

#pragma region PBR Lighting Model
struct CameraInfo
{
	CameraInfo() : _cameraWorldPosition(Vector3::Zero), _viewMatrix(Matrix::Identity), _projMatrix(Matrix::Identity),
		_prevViewMatrix(Matrix::Identity), _prevProjMatrix(Matrix::Identity)
	{}

	Vector3 _cameraWorldPosition;

private:
	float _pad;

public:
	Matrix _viewMatrix;

	Matrix _projMatrix;

	Matrix _prevViewMatrix;

	Matrix _prevProjMatrix;
};

struct LightInfo
{
	LightInfo() : _color(Vector3(0.7f, 0.7f, 0.7f)), _lumen(1.f)
	{}

	Vector3 _color;

	float _lumen;
};

struct DirectionalLightInfo final : public LightInfo
{
	DirectionalLightInfo() : LightInfo(), _direction(Vector3(1.f, 0.f, 0.f)), _pad(0.f)
	{}

	// == In Game Engine, World Rotation Look Vector
	Vector3 _direction;

private:
	float _pad;
};

struct PointLightInfo final : public LightInfo
{
	PointLightInfo() : LightInfo(), _position(Vector3(0.f, 0.f, 0.f)),
		_range(25.f)
	{}

	Vector3 _position;

	float _range;
};

struct SpotLightInfo final : public LightInfo
{
	SpotLightInfo() : LightInfo(), _direction(Vector3(1.f, 0.f, 0.f)), _halfAngle(3.141592f / 4), 
		_position(Vector3::Zero), _range(25.f)
	{}

	Vector3 _direction;
	float _halfAngle;

	Vector3 _position;
	float _range;
};

struct SSAOInfo final
{
	SSAOInfo() : _occlusionRadius(0.5f), _occlusionFadeStart(0.2f),
		_occlusionFadeEnd(2.f), _surfaceEpsilon(0.05f), _sampleCount(14)
	{
		// 8 cube corners
		_offsetVectors[0] = Vector4(+1.0f, +1.0f, +1.0f, 0.0f);
		_offsetVectors[1] = Vector4(-1.0f, -1.0f, -1.0f, 0.0f);
		_offsetVectors[2] = Vector4(-1.0f, +1.0f, +1.0f, 0.0f);
		_offsetVectors[3] = Vector4(+1.0f, -1.0f, -1.0f, 0.0f);
		_offsetVectors[4] = Vector4(+1.0f, +1.0f, -1.0f, 0.0f);
		_offsetVectors[5] = Vector4(-1.0f, -1.0f, +1.0f, 0.0f);
		_offsetVectors[6] = Vector4(-1.0f, +1.0f, -1.0f, 0.0f);
		_offsetVectors[7] = Vector4(+1.0f, -1.0f, +1.0f, 0.0f);

		// 6 ters of cube faces
		_offsetVectors[8] = Vector4(-1.0f, 0.0f, 0.0f, 0.0f);
		_offsetVectors[9] = Vector4(+1.0f, 0.0f, 0.0f, 0.0f);
		_offsetVectors[10] = Vector4(0.0f, -1.0f, 0.0f, 0.0f);
		_offsetVectors[11] = Vector4(0.0f, +1.0f, 0.0f, 0.0f);
		_offsetVectors[12] = Vector4(0.0f, 0.0f, -1.0f, 0.0f);
		_offsetVectors[13] = Vector4(0.0f, 0.0f, +1.0f, 0.0f);

		std::random_device rd;

		std::mt19937 gen(rd());

		std::uniform_real_distribution<float> dist(0.25f, 1.f);

		for (int i = 0; i < 14; ++i)
		{
			// 0.25 ~ 1 사이의 Random Length
			float s = dist(gen);

			_offsetVectors[i].Normalize();

			_offsetVectors[i] = s * _offsetVectors[i];
		}
	}

	// 최대 14개의 Sampling을 지원함
	Vector4 _offsetVectors[14];

	// Left Top, Right Top, Left Bottom, Right Bottom
	Vector4 _cameraFrustumCorner[4];

	float _occlusionRadius;

	float _occlusionFadeStart;

	float _occlusionFadeEnd;

	float _surfaceEpsilon;

	uint32 _sampleCount;

	Vector3 _pad;
};

constexpr uint32 LIGHT_INFO_MAX = 100;

enum class POSTPROCESS_OPTION
{
	ON_SSAO = 0b00000001,
	ON_GAUSSIAN_BLUR = 0b00000010,
	ON_CAM_BLUR = 0b00000100,
	ON_BLOOM = 0b00001000,
	ON_FXAA = 0b00010000,
	ON_VIGNETTING = 0b00100000,
	ON_DEBUG_PANEL = 0b01000000
};

struct PerFrameData
{
	PerFrameData() : _cameraInfo(std::make_shared<CameraInfo>()),
		_directionalLightInfos(std::vector<std::shared_ptr<DirectionalLightInfo>>()),
		_spotLightInfos(std::vector<std::shared_ptr<SpotLightInfo>>()),
		_pointLightInfos(std::vector<std::shared_ptr<PointLightInfo>>()),
		_ssaoInfo(std::make_shared<SSAOInfo>()),
		_postProcessOption(static_cast<POSTPROCESS_OPTION>(static_cast<uint32>(POSTPROCESS_OPTION::ON_FXAA) | static_cast<uint32>(POSTPROCESS_OPTION::ON_CAM_BLUR))),
		_deltaTime(0.f)
	{}

	std::shared_ptr<CameraInfo> _cameraInfo;

	std::vector<std::shared_ptr<DirectionalLightInfo>> _directionalLightInfos;

	std::vector<std::shared_ptr<SpotLightInfo>> _spotLightInfos;

	std::vector<std::shared_ptr<PointLightInfo>> _pointLightInfos;

	// 각종 포스트 프로세스 인포가 있어야하는데 .. 사실 이거 SSAOInfo도 소멸시키고 싶다 그냥 ..
	std::shared_ptr<SSAOInfo> _ssaoInfo;

	POSTPROCESS_OPTION _postProcessOption;

	float _deltaTime;						// 시간이 얼마나 흘렀는가
};
#pragma endregion