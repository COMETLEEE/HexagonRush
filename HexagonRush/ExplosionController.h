#pragma once
class ExplosionController : public Muscle::IComponents
{
public:
	ExplosionController(std::shared_ptr<Muscle::GameObject> gameObj);

	~ExplosionController();
private:
	std::shared_ptr<Muscle::ParticleRenderer> _charging;
	std::shared_ptr<Muscle::ParticleRenderer> _charging1;
	std::shared_ptr<Muscle::ParticleRenderer> _explosion;

	float _speed = 1;

	float _time = 0;

	float _size = 1;

	bool _isCharging = false;

public:
	void SetTexture(tstring name);

	void SetSpeed(float speed);

	void SetSize(float size);

	void Start();

	void Update();
};

