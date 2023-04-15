#pragma once
#include "Resource_def.h"

namespace GraphicsEngine
{
	class ResourceIDGenerator
	{
		inline static uint64 _nextID = 0;

	public:
		// 최대 42억 개의 오브젝트를 다룰 수 있다.
		inline static uint64 Generate() { return _nextID++; }
	};

	/// <summary>
	/// 리소스들의 통합 관리를 위한 추상 인터페이스 클래스
	/// </summary>
	template <RESOURCE_TYPE ResourceType>
	interface ResourceBase
	{
	public:
		ResourceBase();

		virtual ~ResourceBase();

		virtual void Release() abstract;

		void Reset() { _refCount = max(0, _refCount--); }

	public:
		inline uint32 GetRefCount() { return _refCount; }

		inline RESOURCE_TYPE GetResourceType() { return _resourceType; }

		// 레퍼런스 카운트가 0이된 이후 지나간 시간 => 기준을 두어서 특정 시간만큼 흘렀다면 리소스 다운
		inline float GetTimeFromRefZero() { return _timeFromRefZero; }

		inline const tstring& GetName() { return _resourceName; }

		void SetName(const tstring& name) { _resourceName = name; }

		inline const uint64& GetID() { return _resourceID; }

	protected:
		RESOURCE_TYPE _resourceType;

		uint32 _refCount;

		float _timeFromRefZero;

		// Hashing, Searching 등에 사용되는 리소스의 이름
		tstring _resourceName;

		// 리소스 생성 시에 발급되는 ID
		uint64 _resourceID;
	};

	template <RESOURCE_TYPE ResourceType>
	ResourceBase<ResourceType>::ResourceBase() :
		_resourceType(ResourceType), _refCount(0), _timeFromRefZero(0.f)
	{
		// 헤응
		_resourceID = ResourceIDGenerator::Generate();
	}

	template <RESOURCE_TYPE ResourceType>
	ResourceBase<ResourceType>::~ResourceBase()
	{
		// 에옹
	}
}