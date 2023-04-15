#pragma once

constexpr float LANDMINE_TIME = 2.f;

class LandMineField : public Muscle::IComponents
{
public:
	LandMineField(std::shared_ptr<Muscle::GameObject> gameObj);

	virtual ~LandMineField();

private:
	float _timer;

public:
	virtual void Start() override;

	virtual void Update() override;
};