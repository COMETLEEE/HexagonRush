#include "pch.h"
#include "Rush_Bullet_Prefab.h"

#include "Rush_BulletController.h"

Rush_Bullet_Prefab::Rush_Bullet_Prefab() : IPrefab(Muscle::CreateGameObject())
{
	GetGameObject()->AddComponent<Rush_BulletController>();

	std::shared_ptr<Muscle::GameObject> go1 = Muscle::CreateGameObject();

	// Radius 없이 날아가면서 강렬한 빛을 뿜을 녀석
	go1->AddComponent<Muscle::ParticleRenderer>()->SetParticleTextureID(ResourceManager::Get()->GetTextureID(TEXT("Particle")));
	
	go1->SetTag("Burn");

	std::shared_ptr<Muscle::GameObject> go2 = Muscle::CreateGameObject();

	// 흰색 빛으로 탄환 주변을 조그마하게 날아다니는 원 궤적을 뿌릴 녀석
	go2->AddComponent<Muscle::ParticleRenderer>()->SetParticleTextureID(ResourceManager::Get()->GetTextureID(TEXT("Particle")));

	go2->SetTag("AfterBurn");

	std::shared_ptr<Muscle::GameObject> go3 = Muscle::CreateGameObject();

	// Bullet Object가 도착지에 도달하면 이 컴포넌트가 Play되며 순식간에 주변으로 다량의 파티클을 보냅니다.
	go3->AddComponent<Muscle::ParticleRenderer>()->SetParticleTextureID(ResourceManager::Get()->GetTextureID(TEXT("Particle")));

	go3->SetTag("Boom");

	GetGameObject()->SetChild(go1);
	GetGameObject()->SetChild(go2);
	GetGameObject()->SetChild(go3);
}

Rush_Bullet_Prefab::~Rush_Bullet_Prefab()
{

}