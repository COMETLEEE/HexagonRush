#pragma once

class CannonBall : public Muscle::IComponents
{
public:
	CannonBall(std::shared_ptr<Muscle::GameObject> gameObj);

	virtual ~CannonBall();

public:
	// 랜덤 위치로 발사한다. 그리고 지면에 닿으면 터진다
	void Launch();

public:
	virtual void Start() override;

	virtual void Update() override;
};