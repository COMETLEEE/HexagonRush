#pragma once

#include "QuadTree.h"

namespace Muscle
{

	enum class DEBUG_OPTION
	{
		NONE = 0,
		ON_BOUNDING_VOLUME = 1,
		ON_COLLIDER = 2,
		ON_QUAD = 4,
		ON_ALL = ON_BOUNDING_VOLUME | ON_COLLIDER | ON_QUAD

		///	값이 2의 n승 꼴로 올라가야한다.
		/// 비트연산자 &를 사용해서 골라내기 위함!
	};

	class SphereCollider;

	class BoxCollider;

	class CapsuleCollider;

	class GraphicsManager;
	
	constexpr uint32 MAX_DEBUG_REDERNING_DATA = 1000;

	constexpr uint32 DEBUG_OPTION_COUNT = 3;

	class DebugManager final
	{
	public:
		DebugManager() = default;

		~DebugManager();

		DebugManager(const DebugManager& other) = delete;

		DebugManager& operator= (const DebugManager& other) = delete;

		DebugManager(DebugManager&& other) = delete;

		DEBUG_OPTION _debugOption;

	private:
		std::shared_ptr<GraphicsManager> _graphicsManager; // 캐싱
			
		std::shared_ptr<PerFrameData> _perframeData; // 내부적으로 캐싱하고 사용할 것.

		std::queue<std::shared_ptr<RenderingData_3D>> _culledRenderQueue; // 보낼거

		std::vector<std::shared_ptr<RenderingData_3D>> _debugRenderingDataPools; // 이리 저장해놓고 꺼내쓰자.

		std::vector<std::function<void(DebugManager&)>> _workList; // 함수 포인터.

		uint32 _dataIndex;

		void DrawBoundingVolumes();

		void DrawColliders();

		void DrawQuadTree();

		void DrawQuadTreeNode(std::shared_ptr<QuadTree::Node> node);

		void DrawSphereCollider(std::shared_ptr<SphereCollider> sphereCollider);

		void DrawBoxCollider(std::shared_ptr<BoxCollider> boxCollider);

		void DrawCapsuleCollider(std::shared_ptr<CapsuleCollider> CapsuleCollider);

	public:
		void Initialize();

		void Render();

		void Release();

	public:
		void PostPerFrameData(std::shared_ptr<::PerFrameData>& perframeData); // 캐싱 하는 용도!

		void PostCulledRenderingData(std::shared_ptr<RenderingData_3D>& culledRenderingData); // 이 함수로 기즈모를 그릴 녀석을 보낸다.
	};
}


