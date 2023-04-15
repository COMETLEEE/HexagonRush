#pragma once
#include "Graphics_DX11_typedef.h"

#include "Graphics_PerFrameData.h"
#include "Graphics_TextData.h"

struct ObjectInfo
{
	ObjectInfo() : _meshID(2), _objectID(0), _usingLighting(true), _usingShadow(true)
	{}

	uint64 _meshID;

	// 게임
	uint32 _objectID;

	bool _usingLighting;

	bool _usingShadow;
};

// Legacy Model
//struct MaterialInfo
//{
//	MaterialInfo() : _diffuseMapID(ULLONG_MAX), _normalMapID(ULLONG_MAX)
//		, _ambient(Vector4(0.5f, 0.5f, 0.5f, 1.f)), _diffuse(Vector4(0.5f, 0.5f, 0.5f, 1.f))
//		, _specular(Vector4(0.5f, 0.5f, 0.5f, 1.f)), _reflect(0.1f, 0.1f, 0.1f, 1.f)
//	{}
//
//	Vector4 _ambient;
//	Vector4 _diffuse;
//	Vector4 _specular;
//	Vector4 _reflect;
//
//	// 쉐이더에 데이터 복사할 때 (디퍼드 패스에서) 이거 빼서 복사하는 중이니까 .. 나중에 뒤에 뭐 추가되면
//	// 잘 수정하렴 미래의 나야.
//	uint64 _diffuseMapID;
//	uint64 _normalMapID;
//};

// PBR Model
struct MaterialInfo
{
	MaterialInfo() : _diffuseMapID(ULLONG_MAX), _normalMapID(ULLONG_MAX), _metallicMapID(ULLONG_MAX),
		_roughnessMapID(ULLONG_MAX), _ambientOcclusionMapID(ULLONG_MAX), _emissiveMapID(ULLONG_MAX)
		, _metallic(1.f), _roughness(0.3f), _emissive(Vector4::Zero)
	{}

	float _metallic;

	float _roughness;

	Vector4 _emissive;

	// 쉐이더에 데이터 복사할 때 (디퍼드 패스에서) 이거 빼서 복사하는 중이니까 .. 나중에 뒤에 뭐 추가되면
	// 잘 수정하렴 미래의 나야.
	uint64 _diffuseMapID;

	uint64 _normalMapID;

	uint64 _metallicMapID;

	uint64 _roughnessMapID;

	uint64 _ambientOcclusionMapID;

	uint64 _emissiveMapID;
};

struct ShaderInfo
{
	enum class RASTERIZER_STATE
	{
		SOLID = 0,
		WIREFRAME = 1,
	};

	ShaderInfo() : _vsName(TEXT("VS_A")), _psName(TEXT("PS_A")), _rasterizerState(RASTERIZER_STATE::SOLID)
	{}

	tstring _vsName;
	tstring _psName;

	RASTERIZER_STATE _rasterizerState;
};

struct GeometryInfo
{
	GeometryInfo() : _world(Matrix::Identity), _worldInvTranspose(Matrix::Identity), 
		_worldViewProj(Matrix::Identity), _texTransform(Matrix::Identity)
	{}

	Matrix _world;

	Matrix _worldInvTranspose;

	Matrix _worldViewProj;

	Matrix _texTransform;
};

struct AnimationInfo
{
	AnimationInfo():
		_boneMatrixList()
	{}

	Matrix _boneMatrixList[70];
};

enum class RENDERINGDATA_TYPE
{
	// Static Mesh
	STATIC_MESH_OBJECT,

	// Skinned Mesh
	SKINNED_MESH_OBJECT,

	// Sky Box (Cube Map)
	SKY_BOX
};

struct RenderingData_3D
{
	RenderingData_3D() : _dataType(RENDERINGDATA_TYPE::STATIC_MESH_OBJECT),
		_objectInfo(std::make_shared<ObjectInfo>()), _materialInfo(std::make_shared<MaterialInfo>()),
		_shaderInfo(std::make_shared<ShaderInfo>()), _geoInfo(std::make_shared<GeometryInfo>()), _animInfo(std::make_shared<AnimationInfo>())
	{}

	~RenderingData_3D()
	{
		_objectInfo.reset();

		_materialInfo.reset();

		_shaderInfo.reset();

		_geoInfo.reset();

		_animInfo.reset();
	}

	RENDERINGDATA_TYPE _dataType;

	std::shared_ptr<ObjectInfo> _objectInfo;

	std::shared_ptr<MaterialInfo> _materialInfo;

	std::shared_ptr<ShaderInfo> _shaderInfo;

	std::shared_ptr<GeometryInfo> _geoInfo;

	std::shared_ptr<AnimationInfo> _animInfo;
};