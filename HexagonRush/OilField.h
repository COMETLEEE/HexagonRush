#pragma once

class Player;

// 밟고 있으면 이속을 감소시킨다. 여기 영역에서만 !
class OilField : public Muscle::IComponents
{
public:
	OilField(std::shared_ptr<Muscle::GameObject> gameObj);

	virtual ~OilField();

private:
	// 플레이어
	std::shared_ptr<Player> _currentPlayer;

	float _timer;

	float _isSetUp;

public:
	virtual void Start() override;

	virtual void Update() override;

	virtual void OnStayCollision(std::shared_ptr<Muscle::Collider> _Other) override;
	
	virtual void OnExitCollision(std::shared_ptr<Muscle::Collider> _Other) override;
};