#pragma once

// 아이템 사용할 때 나오는 이펙트 만드는 녀석
class BuffController : public Muscle::IComponents
{
public:
	BuffController(std::shared_ptr<Muscle::GameObject> gameObj);

	virtual ~BuffController();

private:
	std::shared_ptr<Muscle::ParticleRenderer> _hpRenderer;

	std::shared_ptr<Muscle::ParticleRenderer> _speedUpRenderer;

	std::shared_ptr<Muscle::ParticleRenderer> _powerUpRenderer;

	bool _isPlayingHP;

	bool _isPlayingSpeedUp;

	bool _isPlayingPowerUp;

	float _hpBuffEffectTime;

	float _speedUpBuffEffectTime;

	float _powerUpBuffEffectTime;

public:
	void OnHPBuff();

	void OnSpeedUpBuff();

	void OnPowerUpBuff();

	virtual void Update() override;
};