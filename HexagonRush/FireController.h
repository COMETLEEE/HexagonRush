#pragma once
class FireController : public Muscle::IComponents
{
public:
	FireController(std::shared_ptr<Muscle::GameObject> gameObj);

	~FireController();
private:
	std::shared_ptr<Muscle::ParticleRenderer> _fire1;
	std::shared_ptr<Muscle::ParticleRenderer> _fire2;
	std::shared_ptr<Muscle::ParticleRenderer> _fire3;

	float _speed = 1;

	float _time = 0;

	float _size = 1;

	float _endTime = 3;

	bool _isCharging = false;


public:
	void SetSize(float size);

	void SetEndTime(float endTime) { _endTime = endTime; }

	void Start();

	void Update();
};

