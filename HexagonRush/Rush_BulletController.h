#pragma once

class Boss;

class Character;

class Player;

// 이건 풀링해야할듯 .. 엄청나게 빠른 속도로 탄환이 생성된다.
class Rush_BulletController : public Muscle::IComponents
{
	enum class RUSH_BULLET_STATE
	{
		LAUNCHED,
		STORED,
	};

public:
	Rush_BulletController(std::shared_ptr<Muscle::GameObject> gameObj);

	virtual ~Rush_BulletController();

private:
	std::shared_ptr<Player> _player;

	std::shared_ptr<Character> _boss;

	std::shared_ptr<Muscle::Transform> _playerTransform;

	std::shared_ptr<Muscle::GameObject> _burn;

	std::shared_ptr<Muscle::ParticleRenderer> _burnRenderer;

	std::shared_ptr<Muscle::GameObject> _afterBurn;

	std::shared_ptr<Muscle::ParticleRenderer> _afterBurnRenderer;

	std::shared_ptr<Muscle::GameObject> _boom;

	std::shared_ptr<Muscle::ParticleRenderer> _boomRenderer;

private:
	float _bulletLifeTimer;

	RUSH_BULLET_STATE _rushBulletState;

	Vector3 _destination;

	bool _isCollided;

	float _deleteTimer;

	Vector4 _color;
	
	float CheckDistance();

private:
	void SettingParticleOfBurn();

	void SettingParticleOfAfterBurn();

	void SettingParticleOfBoom();

public:
	void LaunchTo(const Vector3& dest);
	
	void SetColor(const Vector4& color);

	virtual void Start() override;

	virtual void Update() override;
};