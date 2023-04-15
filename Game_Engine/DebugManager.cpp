#include "pch.h"
#include "DebugManager.h"
#include "QuadTree.h"
namespace Muscle
{
	DebugManager::~DebugManager()
	{
		Release();
	}

	void DebugManager::PostPerFrameData(std::shared_ptr<::PerFrameData>& perframeData)
	{
		_perframeData = perframeData;
	}

	void DebugManager::PostCulledRenderingData(std::shared_ptr<RenderingData_3D>& culledRenderingData)
	{
		_culledRenderQueue.emplace(culledRenderingData);
	}

	void DebugManager::Initialize()
	{
		_graphicsManager = MuscleEngine::GetInstance()->GetGraphicsManager();

		_perframeData = nullptr;

		_debugOption = DEBUG_OPTION::ON_ALL;

		_workList = std::vector<std::function<void(DebugManager&)>>(DEBUG_OPTION_COUNT);

		_workList[0] = &DebugManager::DrawBoundingVolumes;			// 흰색
		_workList[1] = &DebugManager::DrawColliders;				// 녹색, 빨간색
		_workList[2] = &DebugManager::DrawQuadTree;				// 녹색, 빨간색

		_debugRenderingDataPools = std::vector<std::shared_ptr<RenderingData_3D>>(MAX_DEBUG_REDERNING_DATA);

		for (int i = 0; i < MAX_DEBUG_REDERNING_DATA; i++)
		{
			_debugRenderingDataPools[i] = std::make_shared<RenderingData_3D>();
			_debugRenderingDataPools[i]->_objectInfo->_usingLighting = false;
			_debugRenderingDataPools[i]->_shaderInfo->_vsName = TEXT("VS_ColorOnly");
			_debugRenderingDataPools[i]->_shaderInfo->_psName = TEXT("PS_ColorOnly");
			_debugRenderingDataPools[i]->_shaderInfo->_rasterizerState = ShaderInfo::RASTERIZER_STATE::WIREFRAME;
		}
	}

	void DebugManager::Render()
	{
		if (!IGameEngine::Get()->GetisDebug()) return;

		auto twoPow = [](size_t i) -> uint32&& { uint32 ret = 1; for (size_t j = 0; j < i; j++) ret *= 2; return std::move(ret);  };

		for (size_t i = 0; i < _workList.size(); i++)
		{
			auto&& iSquare = twoPow(i);

			if ((static_cast<uint32>(_debugOption) & iSquare) == iSquare)
				_workList[i](*this);
		}
		_dataIndex = 0;

	}
	void DebugManager::DrawBoundingVolumes()
	{
		while (!_culledRenderQueue.empty() && _dataIndex < MAX_DEBUG_REDERNING_DATA)
		{
			std::shared_ptr<RenderingData_3D> renderingData = _culledRenderQueue.front(); _culledRenderQueue.pop();

			std::shared_ptr<BoundingCube> boundingCubeInfo = ResourceManager::Get()->GetBoundingCube(renderingData->_objectInfo->_meshID);

			Matrix localMatrix = boundingCubeInfo->_localMatrix;
			_debugRenderingDataPools[_dataIndex]->_objectInfo->_meshID = WHITE_CUBE_MESH_ID;
			_debugRenderingDataPools[_dataIndex]->_geoInfo->_world = localMatrix * renderingData->_geoInfo->_world;
			_debugRenderingDataPools[_dataIndex]->_geoInfo->_worldViewProj = localMatrix * renderingData->_geoInfo->_worldViewProj;

			_graphicsManager->PostRenderingData_3D(_debugRenderingDataPools[_dataIndex++]);
		}
	}

	void DebugManager::DrawColliders()
	{
		std::shared_ptr<ObjectManager> objectManager = MuscleEngine::GetInstance()->GetObjManager();

		if (objectManager == nullptr)
			return;

		const auto& colliderList = objectManager->GetColliders();

		// Current Scene의 Collider List 순회하면서 현재 상태에 따라서 콜라이더를 현 위치에 그려줌
		for (auto& collider : colliderList)
		{
			if (!collider.second->GetGameObject()->GetIsEnable())
				continue;

			std::shared_ptr<SphereCollider> isSphere = std::dynamic_pointer_cast<SphereCollider>(collider.second);
			std::shared_ptr<BoxCollider> isBox = std::dynamic_pointer_cast<BoxCollider>(collider.second);
			std::shared_ptr<CapsuleCollider> isCapsule = std::dynamic_pointer_cast<CapsuleCollider>(collider.second);

			// Sphere Collider
			if (isSphere != nullptr)
				DrawSphereCollider(isSphere);

			// Box Collider
			if (isBox != nullptr)
				DrawBoxCollider(isBox);
			if (isCapsule != nullptr)
				DrawCapsuleCollider(isCapsule);

			if (_dataIndex >= MAX_DEBUG_REDERNING_DATA)
				break;
		}
	}

	void DebugManager::DrawQuadTree()
	{
		std::shared_ptr<QuadTree> QuadTree = MuscleEngine::GetInstance()->GetObjManager()->GetQuadTree();

		for (auto& QuadBox : QuadTree->GetLeafNodes())
		{
			DrawQuadTreeNode(QuadBox);
		}
		DrawQuadTreeNode(QuadTree->GetRoot());
	}

	void DebugManager::DrawQuadTreeNode(std::shared_ptr<QuadTree::Node> node)
	{

		_debugRenderingDataPools[_dataIndex]->_objectInfo->_meshID = RED_CUBE_MESH_ID;

		_debugRenderingDataPools[_dataIndex]->_geoInfo->_world = Matrix::CreateScale(node->_width, 5, node->_height) *
			Matrix::CreateTranslation(node->_posX, 5, node->_posY);

		_debugRenderingDataPools[_dataIndex]->_geoInfo->_worldViewProj = _debugRenderingDataPools[_dataIndex]->_geoInfo->_world
			* _perframeData->_cameraInfo->_viewMatrix * _perframeData->_cameraInfo->_projMatrix;

		_graphicsManager->PostRenderingData_3D(_debugRenderingDataPools[_dataIndex++]);


	}

	void DebugManager::DrawSphereCollider(std::shared_ptr<SphereCollider> sphereCollider)
	{
		if (sphereCollider->GetIsCollided())
			_debugRenderingDataPools[_dataIndex]->_objectInfo->_meshID = RED_SPHERE_MESH_ID;
		else
			_debugRenderingDataPools[_dataIndex]->_objectInfo->_meshID = GREEN_SPHERE_MESH_ID;

		_debugRenderingDataPools[_dataIndex]->_geoInfo->_world = Matrix::CreateScale(sphereCollider->GetRadius()) *
			Matrix::CreateTranslation(sphereCollider->GetWorldPosition());

		_debugRenderingDataPools[_dataIndex]->_geoInfo->_worldViewProj = _debugRenderingDataPools[_dataIndex]->_geoInfo->_world
			* _perframeData->_cameraInfo->_viewMatrix * _perframeData->_cameraInfo->_projMatrix;

		_graphicsManager->PostRenderingData_3D(_debugRenderingDataPools[_dataIndex++]);
	}

	void DebugManager::DrawBoxCollider(std::shared_ptr<BoxCollider> boxCollider)
	{
		if (boxCollider->GetIsCollided())
			_debugRenderingDataPools[_dataIndex]->_objectInfo->_meshID = RED_CUBE_MESH_ID;
		else
			_debugRenderingDataPools[_dataIndex]->_objectInfo->_meshID = GREEN_CUBE_MESH_ID;

		_debugRenderingDataPools[_dataIndex]->_geoInfo->_world = Matrix::CreateScale(boxCollider->GetSize()) * boxCollider->GetWorldRotTM() *
			Matrix::CreateTranslation(boxCollider->GetWorldPosition());

		_debugRenderingDataPools[_dataIndex]->_geoInfo->_worldViewProj = _debugRenderingDataPools[_dataIndex]->_geoInfo->_world
			* _perframeData->_cameraInfo->_viewMatrix * _perframeData->_cameraInfo->_projMatrix;

		_graphicsManager->PostRenderingData_3D(_debugRenderingDataPools[_dataIndex++]);
	}

	void DebugManager::DrawCapsuleCollider(std::shared_ptr<CapsuleCollider> cylinderCollider)
	{
		uint64 cylindermeshID;
		uint64 spheremeshID;
		if (cylinderCollider->GetIsCollided())
		{
			cylindermeshID = RED_CYLINDER_MESH_ID;
			spheremeshID = RED_HALFSPHERE_MESH_ID;
		}
		else
		{
			cylindermeshID = GREEN_CYLINDER_MESH_ID;
			spheremeshID = GREEN_HALFSPHERE_MESH_ID;
		}
		// 위에 있는 구.
		// 밑에 있는 구.
		// 가운데 실린더.
		// 3개를 합쳐서 캡슐을 만든다...!

		// 위에 구.
		_debugRenderingDataPools[_dataIndex]->_objectInfo->_meshID = spheremeshID;

		_debugRenderingDataPools[_dataIndex]->_geoInfo->_world = Matrix::CreateScale(cylinderCollider->GetRadius()) *
			cylinderCollider->GetWorldRotTM()
			* Matrix::CreateTranslation(cylinderCollider->GetWorldPosition() + cylinderCollider->GetTopPosition());

		_debugRenderingDataPools[_dataIndex]->_geoInfo->_worldViewProj = _debugRenderingDataPools[_dataIndex]->_geoInfo->_world
			* _perframeData->_cameraInfo->_viewMatrix * _perframeData->_cameraInfo->_projMatrix;

		_graphicsManager->PostRenderingData_3D(_debugRenderingDataPools[_dataIndex++]);

		// 밑에 구.
		_debugRenderingDataPools[_dataIndex]->_objectInfo->_meshID = spheremeshID;

		_debugRenderingDataPools[_dataIndex]->_geoInfo->_world = Matrix::CreateScale(-cylinderCollider->GetRadius()) *
			cylinderCollider->GetWorldRotTM()
			* Matrix::CreateTranslation(cylinderCollider->GetWorldPosition() + cylinderCollider->GetBottomPosition());

		_debugRenderingDataPools[_dataIndex]->_geoInfo->_worldViewProj = _debugRenderingDataPools[_dataIndex]->_geoInfo->_world
			* _perframeData->_cameraInfo->_viewMatrix * _perframeData->_cameraInfo->_projMatrix;

		_graphicsManager->PostRenderingData_3D(_debugRenderingDataPools[_dataIndex++]);

		// 가운데 실린더.
		_debugRenderingDataPools[_dataIndex]->_objectInfo->_meshID = cylindermeshID;

		_debugRenderingDataPools[_dataIndex]->_geoInfo->_world = Matrix::CreateScale(Vector3(cylinderCollider->GetRadius(), cylinderCollider->GetDistance(), cylinderCollider->GetRadius())) *
			cylinderCollider->GetWorldRotTM()
			* Matrix::CreateTranslation(cylinderCollider->GetWorldPosition());

		_debugRenderingDataPools[_dataIndex]->_geoInfo->_worldViewProj = _debugRenderingDataPools[_dataIndex]->_geoInfo->_world
			* _perframeData->_cameraInfo->_viewMatrix * _perframeData->_cameraInfo->_projMatrix;

		_graphicsManager->PostRenderingData_3D(_debugRenderingDataPools[_dataIndex++]);

	}

	void DebugManager::Release()
	{
		_graphicsManager.reset();

		_perframeData.reset();

		while (!_culledRenderQueue.empty())
		{
			std::shared_ptr<RenderingData_3D> renderingData = _culledRenderQueue.front(); _culledRenderQueue.pop();

			renderingData.reset();
		}

		for (auto& renderingData : _debugRenderingDataPools)
			renderingData.reset();
	}
}