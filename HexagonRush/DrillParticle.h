#pragma once
class DrillParticle : public Muscle::IComponents
{
public:
	DrillParticle(std::shared_ptr<Muscle::GameObject> gameObject);

	virtual ~DrillParticle();
private:
	std::shared_ptr<Muscle::ParticleRenderer> _particleRenderer[6];

	float _timer = 0;

	float _speed = 1;

	float _endTimer = 5;
public:
	void SetTimer(float time) { _endTimer = time; }

	virtual void Start() override;

	virtual void Update() override;

	virtual void Finalize() override;
};

