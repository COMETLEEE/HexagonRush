#include "pch.h"
#include "PlatformWall.h"

#include "Player.h"

PlatformWall::PlatformWall(std::shared_ptr<Muscle::GameObject> gameObj) :
	IComponents(gameObj)
{

}

PlatformWall::~PlatformWall()
{

}

void PlatformWall::Start()
{
	std::vector<std::shared_ptr<Muscle::GameObject>> sceneObjects = Muscle::IGameEngine::Get()->GetObjManager()->m_vectorObjects;

	for (auto& object : sceneObjects)
	{
		if (object->GetName() == "Player")
			_player = object->GetComponent<Player>();
	}
}

void PlatformWall::Update()
{

}

void PlatformWall::OnEnterCollision(std::shared_ptr<Muscle::Collider> _Other)
{
	// 플레이어랑 충돌했다면 !
	if (_Other->GetGameObject()->GetName() == "Player_Wall")
	{
		float gameDelta = Muscle::CTime::GetGameDeltaTime();

		std::shared_ptr<Muscle::Transform> playerTransform = _player->GetGameObject()->GetComponent<Muscle::Transform>();

		const Vector3& currPos = playerTransform->GetPosition();

		Vector3 curReflecVec = -currPos;

		curReflecVec.y = 0;

		curReflecVec.Normalize();

		playerTransform->SetPosition(currPos + _player->GetSpeed() * curReflecVec * gameDelta);
	}
}

void PlatformWall::OnStayCollision(std::shared_ptr<Muscle::Collider> _Other)
{
	// 플레이어랑 충돌했다면 !
	if (_Other->GetGameObject()->GetName() == "Player_Wall")
	{
		float gameDelta = Muscle::CTime::GetGameDeltaTime();

		std::shared_ptr<Muscle::Transform> playerTransform = _player->GetGameObject()->GetComponent<Muscle::Transform>();

		const Vector3& currPos = playerTransform->GetPosition();

		Vector3 curReflecVec = -currPos;

		curReflecVec.y = 0;

		curReflecVec.Normalize();

		playerTransform->SetPosition(currPos + _player->GetSpeed() * curReflecVec * gameDelta);
	}
}