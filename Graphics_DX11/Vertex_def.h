#pragma once
#include "SimpleMath.h"

namespace Color
{
	extern const __declspec(selectany) Vector4 White = { 1.0f, 1.0f, 1.0f, 1.0f };
	extern const __declspec(selectany) Vector4 Red = { 1.0f, 0.0f, 0.0f, 1.0f };
}

// Basic Vertex
struct StaticMeshVertex
{
	StaticMeshVertex(Vector3 Pos = Vector3(0.f, 0.f, 0.f), Vector2 UV = Vector2(0.f, 1.f), Vector3 Normal = Vector3(1.f, 0.f, 0.f), Vector3 Tangent = Vector3(0.f, 0.f, 0.f),
		Vector4 Color = ::Color::White) :
		_position(Pos), _uv(UV), _normal(Normal), _tangent(Tangent), _color(Color) {}

	DirectX::SimpleMath::Vector3 _position;

	DirectX::SimpleMath::Vector2 _uv;

	DirectX::SimpleMath::Vector3 _normal;

	DirectX::SimpleMath::Vector3 _tangent;

	DirectX::SimpleMath::Vector4 _color;
};

// Skinning Animation Mesh Vertex
struct SkinnedMeshVertex : public StaticMeshVertex
{
	const StaticMeshVertex& operator = (const SkinnedMeshVertex& other)
	{
		StaticMeshVertex smVertex;

		smVertex._color = other._color;
		smVertex._normal = other._normal;
		smVertex._position = other._position;
		smVertex._tangent = other._tangent;
		smVertex._uv = other._uv;

		return smVertex;
	}

	SkinnedMeshVertex(Vector3 Pos = Vector3(0.f, 0.f, 0.f), Vector2 UV = Vector2(0.f, 1.f), Vector3 Normal = Vector3(1.f, 0.f, 0.f), Vector3 Tangent = Vector3(0.f, 0.f, 0.f),
		Vector4 Color = ::Color::White) : StaticMeshVertex(Pos, UV, Normal, Tangent, Color)
	{ 
		for (int i = 0; i < 12; i++)
		{
			_boneIndex[i] = 0; _boneWeight[i] = 0.f;
		}
	}

	// 4x4 행렬 갯수의 인덱스 값 지원
	float _boneWeight[12];

	// 4x4 행렬 갯수의 인덱스 값 지원
	uint32 _boneIndex[12];
};

// Particle System 
struct Particle
{
	Vector3 _initPosition;

	Vector3 _initVelocity;

	Vector2 _size;

	float _age;

	uint32 _type;
};