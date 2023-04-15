#pragma once
class Crosshair : public Muscle::IComponents
{
public:
	Crosshair(std::shared_ptr<Muscle::GameObject> gameObject);

	virtual ~Crosshair();

private:
	float _timer = 0;

	float _speed = 1;

	float _spinSpeed = 6;

	float _scaleSpeed = 4;

	float _spinTimer = 2;

	float _size = 1;

	bool _complate = false;

public:
	void SetSize(float size);

	virtual void Start() override;

	virtual void Update() override;
};