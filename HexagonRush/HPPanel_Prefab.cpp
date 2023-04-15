#include "pch.h"
#include "HPPanel_Prefab.h"

#include "HPPanelController.h"

HPPanel_Prefab::HPPanel_Prefab() : IPrefab(Muscle::CreateGameObject())
{
	// HP Panel Controller => 플레이어의 현재 정보를 계속 업데이트하며
	// HP에 따라서 Left, Middle, Right의 Texture ID를 컨트롤한다. (있는거, 없는거)
	GetGameObject()->AddComponent<HPPanelController>();

	// HP를 표현할 이미지 3장
	std::shared_ptr<Muscle::GameObject> left = Muscle::CreateGameObject();

	left->AddComponent<Muscle::UIRenderer>();

	left->SetTag("Left");

	std::shared_ptr<Muscle::GameObject> middle = Muscle::CreateGameObject();

	middle->AddComponent<Muscle::UIRenderer>();

	middle->SetTag("Mid");

	std::shared_ptr<Muscle::GameObject> right = Muscle::CreateGameObject();

	right->AddComponent<Muscle::UIRenderer>();

	right->SetTag("Right");

	GetGameObject()->SetChild(left);
	GetGameObject()->SetChild(middle);
	GetGameObject()->SetChild(right);
}

HPPanel_Prefab::~HPPanel_Prefab()
{

}