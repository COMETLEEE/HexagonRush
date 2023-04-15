#pragma once
#include <memory>
#include "AnimationClip.h"
/// <summary>
/// 2022.07.07
/// 내 MetaData와 매우 비슷함!
/// 이름만 다른듯
/// </summary>
#include "ASEParsingData.h"

namespace Muscle
{
	class GameObject;

	struct Prefab_ASE;

	struct NodeMaterial
	{
		Vector4 _ambient;
		Vector4 _diffuse;
		Vector4 _specular;
		Vector4 _reflect;

		uint64 _diffuseMapID;
		uint64 _normalMapID;
	};

	struct NodeObjectBase : public std::enable_shared_from_this<NodeObjectBase>
	{
		virtual ~NodeObjectBase();

		std::shared_ptr<GameObject> MakeGameObject(Prefab_ASE* prefab,
			std::map<std::shared_ptr<NodeObjectBase>, std::shared_ptr<GameObject>>& refGameObjectByNodeMap);

		tstring _nodeName = TEXT("");

		tstring _nodeParent = TEXT("");

		ASEParsingData::NodeTM _nodeTM;

		Matrix _localTM;

		std::vector<ASEParsingData::TMAnimation> _tmAnimations;

		std::shared_ptr<NodeObjectBase> _parentNode;

		std::vector<std::shared_ptr<NodeObjectBase>> _childNodes;

		bool _isBone;

	private:
		virtual void SettingGameObject(Prefab_ASE* prefab, std::shared_ptr<GameObject> gameObject);
	};

	struct NodeGeoObject : NodeObjectBase
	{
		~NodeGeoObject() override;

		uint64 _meshID;

		uint32 _materialRef;
		std::vector<tstring> _boneNameList;
		std::vector<ASEParsingData::WVertex> _wVertexs;

	private:
		virtual void SettingGameObject(Prefab_ASE* prefab, std::shared_ptr<GameObject> gameObject) override;
	};

	struct NodeHelperObject : NodeObjectBase
	{
	};

	struct NodeShapeObject : NodeObjectBase
	{
	};

	struct Prefab_ASE
	{
		Prefab_ASE(std::vector<std::shared_ptr<NodeObjectBase>> rootNodes,
			std::map<tstring, std::shared_ptr<NodeObjectBase>> nodeMap,
			std::vector<std::shared_ptr<NodeMaterial>> materialList)
		{
			_rootNodes = std::move(rootNodes);
			_nodeMap = std::move(nodeMap);
			_materialList = std::move(materialList);
		}

		~Prefab_ASE();

		std::shared_ptr<GameObject> MakeGameObject();

		// 최종 부모를 형성하는 녀석들. 이 친구들을 하나의 엔티티에 담는 방식으로 게임 오브젝트를 구성한다.
		std::vector<std::shared_ptr<NodeObjectBase>> _rootNodes;
		std::map<tstring, std::shared_ptr<NodeObjectBase>> _nodeMap;
		std::vector<std::shared_ptr<NodeMaterial>> _materialList;

		// 본 애니메이션으로 각 본의 로컬 포지션을 옮겨도 되는 이유
		// 어차피 게임 오브젝트의 이동, 피격 등은 최종 부모 오브젝트의 트랜스폼에 따르기 때문이다.
		void InitializeAnimTrackList(std::shared_ptr<NodeObjectBase> currentNode, std::vector<tstring> currentNodeHierarchy);

		std::vector<AnimationKey> _animkey;

		// 기타 정보
		tstring _prefabName;
	};

}