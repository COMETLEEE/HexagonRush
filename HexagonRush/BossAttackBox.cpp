#include "pch.h"
#include "BossAttackBox.h"

BossAttackBox::BossAttackBox(std::shared_ptr<Muscle::GameObject> gameObject) : IComponents(gameObject)
{
	_damage = 1;
	_lifeTime = 2.0f;
	_time = 0;
	// 테스트용!!
}

void BossAttackBox::OnEnterCollision(std::shared_ptr<Muscle::Collider> _Other)
{
	std::shared_ptr<Player> player = _Other->GetComponent<Player>();
	if (player)
	{
		player->Hit(_damage);
		return;
	}
	//std::shared_ptr<Platform> platform = _Other->GetComponent<Platform>();
	//if (platform)
	//{
	//	platform->DeletePlatform();
	//	return;
	//}
}

void BossAttackBox::Start()
{
}

void BossAttackBox::Update()
{
	if (GameProcess::Get()->GetIsPausePanelUp())
		return;

	_time += Muscle::CTime::GetGameDeltaTime();

	if (_lifeTime < _time)
		Muscle::DeleteGameObject(GetGameObject());
}
