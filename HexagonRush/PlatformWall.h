#pragma once

class PlatformWall : public Muscle::IComponents
{
public:
	PlatformWall(std::shared_ptr<Muscle::GameObject> gameObj);

	virtual ~PlatformWall();

private:
	std::shared_ptr<Player> _player;

public:
	virtual void Start() override;

	virtual void Update() override;

	virtual void OnEnterCollision(std::shared_ptr<Muscle::Collider> _Other) override;

	virtual void OnStayCollision(std::shared_ptr<Muscle::Collider> _Other) override;
};