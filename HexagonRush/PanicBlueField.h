#pragma once

class PanicBlueField : public Muscle::IComponents
{
public:
	PanicBlueField(std::shared_ptr<Muscle::GameObject> gameObj);

	virtual ~PanicBlueField();

private:
	float _timer;

	std::shared_ptr<Player> _currentPlayer;

public:
	virtual void Start() override;

	virtual void Update() override;

	virtual void OnEnterCollision(std::shared_ptr<Muscle::Collider> _Other) override;
};