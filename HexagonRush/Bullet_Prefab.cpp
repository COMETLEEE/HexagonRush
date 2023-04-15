#include "pch.h"
#include "Bullet_Prefab.h"

#include "BulletController.h"

Bullet_Prefab::Bullet_Prefab() : IPrefab(Muscle::CreateGameObject())
{
	// 여러가지 컴포넌트들 붙여주면 되는건가 ..?
	std::shared_ptr<Muscle::GameObject> go1 = Muscle::CreateGameObject();

	// Radius 없이 날아가면서 강렬한 빛을 뿜을 녀석
	go1->AddComponent<Muscle::ParticleRenderer>();
	
	go1->SetTag("Burn");

	std::shared_ptr<Muscle::GameObject> go2 = Muscle::CreateGameObject();
	
	// 흰색 빛으로 탄환 주변을 조그마하게 날아다니는 원 궤적을 뿌릴 녀석
	go2->AddComponent<Muscle::ParticleRenderer>();

	go2->SetTag("AfterBurn");

	std::shared_ptr<Muscle::GameObject> go3 = Muscle::CreateGameObject();

	// Bullet Object가 도착지에 도달하면 이 컴포넌트가 Play되며 순식간에 주변으로 다량의 파티클을 보냅니다.
	go3->AddComponent<Muscle::ParticleRenderer>();

	go3->SetTag("Boom");

	// 이 녀석에서 태그로 분류 후 위의 컴포넌트 세 개를 조종할 예정이다.
	GetGameObject()->AddComponent<BulletController>();
	// => Boom이 되면 Pool로 되돌립시다.

	GetGameObject()->SetChild(go1);
	GetGameObject()->SetChild(go2);
	GetGameObject()->SetChild(go3);
}

Bullet_Prefab::~Bullet_Prefab()
{
	// 헤응
}