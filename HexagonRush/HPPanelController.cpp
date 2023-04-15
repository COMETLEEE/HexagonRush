#include "pch.h"
#include "HPPanelController.h"

#define PLAYER_MAX_HP 3

HPPanelController::HPPanelController(std::shared_ptr<Muscle::GameObject> gameObj) :
	IComponents(gameObj), _hpRenderers(std::vector<std::shared_ptr<Muscle::UIRenderer>>(PLAYER_MAX_HP))
{
	_currPlayerHP = 0;

	_hpYesTexID = 0;

	_hpNoTexID = 0;
}

HPPanelController::~HPPanelController()
{

}

void HPPanelController::Start()
{
	// Player Component Caching
	std::vector<std::shared_ptr<Muscle::GameObject>> sceneObjects = Muscle::IGameEngine::Get()->GetObjManager()->m_vectorObjects;

	for (auto& object : sceneObjects)
	{
		if (object->GetName() == "Player")
		{
			_playerCom = object->GetComponent<Player>();
		}
	}

	_currPlayerHP = PLAYER_MAX_HP;

	auto&& childrens = GetGameObject()->GetChildrens();

	// 각각 목적에 맞는 오브젝트들을 캐싱 및 인지 후 
	for (auto& children : childrens)
	{
		auto&& tag = children->GetTag();

		if (tag == "Left")
		{
			_hpRenderers[0] = children->GetComponent<Muscle::UIRenderer>();
		}
		else if (tag == "Mid")
		{
			_hpRenderers[1] = children->GetComponent<Muscle::UIRenderer>();
		}
		else if (tag == "Right")
		{
			_hpRenderers [2] = children->GetComponent<Muscle::UIRenderer>();
		}
	}

	// Texture ID Info Get And Set
	_hpYesTexID = ResourceManager::Get()->GetTextureID(TEXT("HP_Yes"));

	_hpNoTexID = ResourceManager::Get()->GetTextureID(TEXT("HP_No"));


	// 위치 조정
	_hpRenderers[0]->SetDrawNDCPosition(Vector2(0.55f, 0.7f), Vector2(0.65f, 0.55f));

	_hpRenderers[1]->SetDrawNDCPosition(Vector2(0.7f, 0.7f), Vector2(0.8f, 0.55f));

	_hpRenderers[2]->SetDrawNDCPosition(Vector2(0.85f, 0.7f), Vector2(0.95f, 0.55f));
}

void HPPanelController::Update()
{
	// 1. Game Process ? 에게 부탁해서 플레이어의 HP를 받아온다.
	_currPlayerHP = _playerCom->GetCurrentHP();

	// 2. HP에 대해서 UI Update
	switch (_currPlayerHP)
	{
		case 0:
		{
			for (auto& renderer : _hpRenderers)
				renderer->SetTextureID(_hpNoTexID);

			break;
		}

		case 1:
		{
			_hpRenderers[0]->SetTextureID(_hpYesTexID);

			_hpRenderers[1]->SetTextureID(_hpNoTexID);

			_hpRenderers[2]->SetTextureID(_hpNoTexID);

			break;
		}

		case 2:
		{
			_hpRenderers[0]->SetTextureID(_hpYesTexID);

			_hpRenderers[1]->SetTextureID(_hpYesTexID);

			_hpRenderers[2]->SetTextureID(_hpNoTexID);

			break;
		}

		case 3:
		{
			for (auto& renderer : _hpRenderers)
				renderer->SetTextureID(_hpYesTexID);

			break;
		}

		default:
		{
			for (auto& renderer : _hpRenderers)
				renderer->SetTextureID(_hpNoTexID);
		}
	}
}