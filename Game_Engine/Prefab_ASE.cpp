#include "pch.h"
#include "Prefab_ASE.h"

#include "GameObject.h"
#include "Transform.h"
#include "MeshRenderer.h"
#include "Animater.h"

#include "StringHelper.h"
//#include "SkinnedMeshRenderer.h"
//#include "Armature.h"

namespace Muscle
{
	NodeObjectBase::~NodeObjectBase()
	{
		for (int i = 0; i < _childNodes.size(); i++)
			_childNodes[i].reset();
	}

	std::shared_ptr<GameObject> NodeObjectBase::MakeGameObject(Prefab_ASE* prefab, std::map<std::shared_ptr<NodeObjectBase>, std::shared_ptr<GameObject>>& refGameObjectByNodeMap)
	{
		std::shared_ptr<GameObject> gameObject = Muscle::CreateGameObject();

		gameObject->SetName(_nodeName);

		gameObject->GetComponent<Transform>()->SetLocalTM(_localTM);

		SettingGameObject(prefab, gameObject);

		for (auto child : _childNodes)
		{
			std::shared_ptr<GameObject> childGO = child->MakeGameObject(prefab, refGameObjectByNodeMap);

			gameObject->SetChild(childGO);
			childGO.reset();
		}

		refGameObjectByNodeMap.insert({ this->shared_from_this(), gameObject });

		return gameObject;
	}

	void NodeObjectBase::SettingGameObject(Prefab_ASE* prefab, std::shared_ptr<GameObject> gameObject)
	{

	}

	NodeGeoObject::~NodeGeoObject()
	{

	}

	void NodeGeoObject::SettingGameObject(Prefab_ASE* prefab, std::shared_ptr<GameObject> gameObject)
	{
		// Static Mesh Object
		if (_boneNameList.empty())
		{
			auto meshRenderer = gameObject->AddComponent<MeshRenderer>();
			if (_isBone)
				meshRenderer->SetIsEnable(false);
			std::shared_ptr<::RenderingData_3D> renderingData = meshRenderer->_renderingData;


			renderingData->_dataType = RENDERINGDATA_TYPE::STATIC_MESH_OBJECT;

			// 메시 넣어줘유.
			renderingData->_objectInfo->_meshID = _meshID;

			if (_materialRef < 0 ||
				_materialRef >= prefab->_materialList.size())
			{
				// Material Ref가 유효하지 않은 경우. 그냥 놔두면 된다.
				renderingData->_shaderInfo->_vsName = TEXT("VS_ColorOnly");
				renderingData->_shaderInfo->_psName = TEXT("PS_ColorOnly");
			}
			else
			{
				std::shared_ptr<NodeMaterial> mat = prefab->_materialList[_materialRef];

				// Legacy Material
				/*renderingData->_materialInfo->_ambient = mat->_ambient;
				renderingData->_materialInfo->_diffuse = mat->_diffuse;
				renderingData->_materialInfo->_specular = mat->_specular;
				renderingData->_materialInfo->_reflect = mat->_reflect;*/

				renderingData->_materialInfo->_metallic = 0.5f;
				renderingData->_materialInfo->_roughness = 0.5f;

				renderingData->_materialInfo->_diffuseMapID = mat->_diffuseMapID;
				renderingData->_materialInfo->_normalMapID = mat->_normalMapID;
				mat.reset();
			}

			if ((renderingData->_materialInfo->_diffuseMapID != ULLONG_MAX) && (renderingData->_materialInfo->_normalMapID != ULLONG_MAX))
			{
				renderingData->_shaderInfo->_vsName = TEXT("VS_AN");
				renderingData->_shaderInfo->_psName = TEXT("PS_AN");
			}
			else if (renderingData->_materialInfo->_diffuseMapID != ULLONG_MAX)
			{
				renderingData->_shaderInfo->_vsName = TEXT("VS_A");
				renderingData->_shaderInfo->_psName = TEXT("PS_A");
			}
			else
			{
				renderingData->_shaderInfo->_vsName = TEXT("VS_ColorOnly");
				renderingData->_shaderInfo->_psName = TEXT("PS_ColorOnly");
			}

			meshRenderer.reset();
		}
		// Skinned Mesh Object
		else
		{
			auto meshRenderer = gameObject->AddComponent<SkinnedMeshRenderer>();

			std::shared_ptr<::RenderingData_3D> renderingData = meshRenderer->_renderingData;

			renderingData->_dataType = RENDERINGDATA_TYPE::SKINNED_MESH_OBJECT;

			renderingData->_objectInfo->_meshID = _meshID;

			if (_materialRef < 0 ||
				_materialRef >= prefab->_materialList.size())
			{
				// Material Ref가 유효하지 않은 경우. 그냥 놔두면 된다.
			}
			else
			{
				std::shared_ptr<NodeMaterial> mat = prefab->_materialList[_materialRef];

				// Legacy Material
				/*renderingData->_materialInfo->_ambient = mat->_ambient;
				renderingData->_materialInfo->_diffuse = mat->_diffuse;
				renderingData->_materialInfo->_specular = mat->_specular;
				renderingData->_materialInfo->_reflect = mat->_reflect;*/

				renderingData->_materialInfo->_metallic = 0.5f;
				renderingData->_materialInfo->_roughness = 0.5f;

				renderingData->_materialInfo->_diffuseMapID = mat->_diffuseMapID;
				renderingData->_materialInfo->_normalMapID = mat->_normalMapID;
			}

			if ((renderingData->_materialInfo->_diffuseMapID != ULLONG_MAX) && (renderingData->_materialInfo->_normalMapID != ULLONG_MAX))
			{
				renderingData->_shaderInfo->_vsName = TEXT("VS_AN_Skinning");
				renderingData->_shaderInfo->_psName = TEXT("PS_AN_Skinning");
			}
			else if (renderingData->_materialInfo->_diffuseMapID != ULLONG_MAX)
			{
				renderingData->_shaderInfo->_vsName = TEXT("VS_A_Skinning");
				renderingData->_shaderInfo->_psName = TEXT("PS_A_Skinning");
			}
			else
			{
				// Rim Light Is Default
				renderingData->_shaderInfo->_vsName = TEXT("VS_ColorOnly_Skinning_Rim");
				renderingData->_shaderInfo->_psName = TEXT("PS_ColorOnly_Skinning_Rim");
			}

			meshRenderer.reset();
		}
	}

	Prefab_ASE::~Prefab_ASE()
	{
		for (int i = 0; i < _rootNodes.size(); i++)
			_rootNodes[i].reset();

		for (auto& iter : _nodeMap)
			iter.second.reset();

		for (int i = 0; i < _materialList.size(); i++)
			_materialList[i].reset();
	}

	std::shared_ptr<GameObject> Prefab_ASE::MakeGameObject()
	{
		// Final Root. 이 친구에 모든 오브젝트들을 형성하고 던진다.
		std::shared_ptr<GameObject> outObject = Muscle::CreateGameObject();

		outObject->SetName(_prefabName);

		std::map<tstring, std::shared_ptr<GameObject>> gameObjectByNodeNameMap;

		std::map<std::shared_ptr<NodeObjectBase>, std::shared_ptr<GameObject>> gameObjectByNodeMap;

		// 루트 노드들에 대해서 애니메이션 정보를 저장한다.
		for (int i = 0; i < _rootNodes.size(); i++)
		{
			std::shared_ptr<GameObject> rootObject = _rootNodes[i]->MakeGameObject(this, gameObjectByNodeMap);

			outObject->SetChild(rootObject);

			std::vector<tstring> hierarchy{ {GraphicsEngine::StringHelper::ToTString(_rootNodes[i]->_nodeName)} };

			//_animkey.clear();
			InitializeAnimTrackList(_rootNodes[i], hierarchy);
		}

		// 본 생성 시 검색을 위해 맵 하나 만들어주자.
		for (const auto& pair : gameObjectByNodeMap)
		{
			gameObjectByNodeNameMap.insert({ pair.first->_nodeName, pair.second });
		}

		// 자자 본 스키닝 애니메이션 준비합시다.
		for (auto& node : gameObjectByNodeMap)
		{
			const auto nodeGeo = std::dynamic_pointer_cast<NodeGeoObject>(node.first);

			if (nodeGeo == nullptr)
				continue;

			if (nodeGeo->_boneNameList.empty())
				continue;

			// 여기까지 통과했다면 이 노드로부터 형성된 게임 오브젝트는 스킨 메쉬이다.
			std::vector<std::shared_ptr<Bone>> boneList(nodeGeo->_boneNameList.size());

			for (int i = 0; i < nodeGeo->_boneNameList.size(); i++)
			{
				tstring boneName = nodeGeo->_boneNameList[i];

				const auto goIter = gameObjectByNodeNameMap.find(boneName);

				if (goIter == gameObjectByNodeNameMap.end())
				{

					boneList.erase(boneList.begin() + i);
					continue;
				}

				const auto iter = _nodeMap.find(boneName);

				if (iter == _nodeMap.end())
					continue;

				std::shared_ptr<Bone> newBone = std::make_shared<Bone>();

				newBone->_boneNodeTM = iter->second->_nodeTM.tm;
				newBone->_name = boneName;
				newBone->_boneObject = goIter->second;
				newBone->_index = i;

				boneList[i] = newBone;
			}

			if (boneList.empty())
				continue;
			if (boneList[0] == nullptr)
			{
				for (auto& iter : boneList)
				{
					iter.reset();
				}

				boneList.clear();

				continue;
			}

			auto armature = node.second->GetComponent<SkinnedMeshRenderer>(); // 스키닝 애니메이션을 위해 필요한 부분!

			armature->Initialize(nodeGeo->_nodeTM.tm, boneList);

			armature.reset();

			for (auto& iter : boneList)
			{
				auto renderer = iter->_boneObject->GetComponent<MeshRenderer>();
				renderer.reset();
				iter.reset();
			}

			boneList.clear();
		}

		//최종 상단 오브젝트에게 Animator Component를 붙여준다.
		if (!_animkey.empty())
		{
			auto animator = outObject->AddComponent<Animater>();

			for (auto& iter : _animkey)
			{
				std::shared_ptr<AnimationClip> animClip = std::make_shared<AnimationClip>(iter, outObject);

				animator->AddAnimClip(iter._animName, animClip);

				animClip.reset();
			}

			animator.reset();
		}


		for (auto& iter : gameObjectByNodeNameMap)
		{
			iter.second.reset();
		}

		gameObjectByNodeNameMap.clear();

		for (auto& iter : gameObjectByNodeMap)
		{
			iter.second.reset();
		}

		gameObjectByNodeMap.clear();

		return outObject;
	}

	void Prefab_ASE::InitializeAnimTrackList(std::shared_ptr<NodeObjectBase> currentNode, std::vector<tstring> currentNodeHierarchy)
	{

		if (_animkey.empty())
		{
			for (const auto& iter : currentNode->_tmAnimations)
			{
				AnimationKey animKey;
				animKey._animName = iter.AnimName;
				_animkey.push_back(animKey);
				// 포지션 애니메이션이 있다 
			}
		}

		for (int i = 0; i < currentNode->_tmAnimations.size(); i++)
		{

			if (currentNode->_tmAnimations[i].controlPosSamples.empty() == false)
			{
				_animkey[i]._animPosTrackList.push_back({ currentNodeHierarchy, currentNode->_tmAnimations[i].controlPosSamples });
			}

			// 회전 애니메이션이 있다.
			if (currentNode->_tmAnimations[i].controlRotSamples.empty() == false)
			{
				_animkey[i]._animRotTrackList.push_back({ currentNodeHierarchy, currentNode->_tmAnimations[i].controlRotSamples });
			}
		}

		// 본인까지 하이어라키를 꽂아서 애니메이션 트랙 리스트를 형성한다.
		for (const auto& child : currentNode->_childNodes)
		{
			currentNodeHierarchy.push_back(GraphicsEngine::StringHelper::ToTString(child->_nodeName));

			InitializeAnimTrackList(child, currentNodeHierarchy);

			currentNodeHierarchy.pop_back();
		}
	}

}