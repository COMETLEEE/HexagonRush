#pragma once

// 플레이어의 조작과 관련된 클래스
class Controller : public Muscle::IComponents
{
public:
	Controller(std::shared_ptr<Muscle::GameObject> gameObject);

	virtual ~Controller();

private:
	std::shared_ptr<Muscle::Transform> _playerTransform;

	std::shared_ptr<Player> _player;
	
	float _evasionCoolTiem;

public:
	virtual void OnStayCollision(std::shared_ptr<Muscle::Collider> _Other);

private:
	void AttackUpdate();

	void MoveUpdate();

	void PanicMoveUpdate();

	void EvasionUpdate();

public:
	virtual void Start();

	virtual void Update(); // 모든 Object Udpate돌릴 때 (ObjectManager) 오브젝트가 Components를 가지고 있으면 같이 업데이트 해주기.
};