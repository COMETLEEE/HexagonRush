#pragma once


class CameraMovementManager : public Muscle::IComponents
{
public:
	CameraMovementManager(std::shared_ptr<Muscle::GameObject> gameObject);

	virtual ~CameraMovementManager();

private:
	static std::shared_ptr<CameraMovementManager> _instance;

	std::shared_ptr<Muscle::Camera> _camera; // 카메라 캐싱해서 사용.

	std::shared_ptr<Muscle::GameObject> _player; // 플레이어 캐싱해서 사용.

	std::shared_ptr<Muscle::GameObject> _boss; // 보스 캐싱해서 사용.

	bool _isMove; // 플레이어의 움직임을 막는 변수.. 현재 카메라가 이동중인가?

	float _timer; // 카메라의 남은 이동시간을 저장. 0 이하이면 카메라 이동 종료.

public:
	void Stage1Start();

	void Stage2Start();

	void BossDieMove();

	void BossPhaseChangeMove();

	void PlayerDieMove();

	void PlayerRushMove();

public:
	bool GetisMove();

	static std::shared_ptr<CameraMovementManager> Get();

	virtual void Update() override;

	virtual void LateUpdate() override;

	virtual void Start() override;

	virtual void Finalize() override;
};