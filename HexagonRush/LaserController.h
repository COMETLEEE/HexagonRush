#pragma once

/// <summary>
/// 재활용은 고려되지 않은 레이져~ 
/// </summary>
class LaserController : public Muscle::IComponents
{
public:
	LaserController(std::shared_ptr<Muscle::GameObject> gameObj);

	~LaserController();
private:
	std::shared_ptr<Muscle::ParticleRenderer> _charging0;
	std::shared_ptr<Muscle::ParticleRenderer> _charging1;
	std::shared_ptr<Muscle::ParticleRenderer> _charging2;
	std::shared_ptr<Muscle::ParticleRenderer> _charging3;
	std::shared_ptr<Muscle::ParticleRenderer> _laser;

	float _speed = 20;
	float _time = 0;
	float _time1 = 0;
	bool _isCharging = false;

	Vector3 _targetPos;

public:
	void SetTarget(Vector3 targetPos);

	void Start();

	void Update();
};