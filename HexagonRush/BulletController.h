#pragma once

// Player Header 추가 금지

class Character;

/// <summary>
/// Bullet_Prefab 내부의 게임 오브젝트에게 붙여져서 실질적인 해당 총알의 역할을 하게될 녀석
/// </summary>
class BulletController : public Muscle::IComponents
{
public:
	// 총알의 현재 상태
	enum class BULLET_STATE
	{
		LOAD,
		FLY,
		BOOM,
		END
	};

	// 총알의 레벨 (트리거가 3개냐, 4개냐, 5개냐 ..)
	enum class BULLET_LEVEL
	{
		ONE = 1,
		TWO = 2,
		THREE = 3
	};

	enum class BULLET_COLOR
	{
		RED = 0,
		GREEN = 1,
		BLUE = 2
	};

public:
	BulletController(std::shared_ptr<Muscle::GameObject> gameObj);

	virtual ~BulletController();

private:
	std::shared_ptr<Muscle::Transform> _topTransform;

	BULLET_STATE _currBulletState;

	BULLET_LEVEL _currBulletLevel;

	BULLET_COLOR _currBulletColor;

	std::shared_ptr<Character> _agent;

	Vector3 _destination;

	float _timeFromBoom;

	std::shared_ptr<Muscle::GameObject> _burn;

	std::shared_ptr<Muscle::ParticleRenderer> _burnRenderer;

	std::shared_ptr<Muscle::GameObject> _afterBurn;

	std::shared_ptr<Muscle::ParticleRenderer> _afterBurnRenderer;

	std::shared_ptr<Muscle::GameObject> _boom;

	std::shared_ptr<Muscle::ParticleRenderer> _boomRenderer;

private:
	void SettingParticleOfBurn();

	void SettingParticleOfAfterBurn();

	void SettingParticleOfBoom();

public:
	// 발사 전 발사한 사람, 목적지, 레벨, 컬러 세팅해주고 발사할 것 !
	void SettingAgent(std::shared_ptr<Character> agent) { _agent = agent; }

	void SetDestination(Vector3 dest) { _destination = dest; }

	void SetBulletLevel(BULLET_LEVEL level) { _currBulletLevel = level; }

	void SetBulletColor(BULLET_COLOR color) { _currBulletColor = color; }

	virtual void Start() override;

	virtual void Update() override;
};