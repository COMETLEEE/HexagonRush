#pragma once

#include "BulletController.h"

/// <summary>
/// 플레이어 컴포넌트
/// </summary>
class Platform;

class Rush_Bullet_Prefab;

// 맥스 콤보 카운트가 되면 Bullet Time 및 Rush 
constexpr uint32 MAX_COMBO_COUNT = 5;

class Player : public FSM<Player>, public Character
{
public:
	enum class SHOOTING_BULLET_INFO
	{
		ONE_RED = 0,
		ONE_GREEN = 1,
		ONE_BLUE = 2,

		TWO_RED = 3,
		TWO_GREEN = 4,
		TWO_BLUE = 5,

		THREE_RED = 6,
		THREE_GREEN = 7,
		THREE_BLUE = 8
	};

	enum Status
	{
		IDLE = 0,
		MOVE = 1,
		ATTACK = 2,
		HIT = 3,
		DIE = 4,
		EVASION = 5,
		RUSH = 6,
		NONE = 7
	};

public:
	Player(std::shared_ptr<Muscle::GameObject> gameObj);

	virtual ~Player();

private:
	Status _checkStatus; // 스테이터스의 상태를 체크하기 위한 변수. // Enter Stay Exit를 위해 필요함.

	float _hpRemaningCool;

	bool _isSpeedUpBuff;

	float _speedUpBuffTime;

	float _speedUpRemainingCool;

	bool _isPowerUpBuff;

	float _powerUpBuffTime;

	float _powerUpRemainingCool;

	// 콤보 카운트가 5가 되면 State Rush !
	uint32 _comboCount;

	// 쏜 총알의 정보들을 기록하고 있다가 데미지 계산에 사용한다.
	std::queue<SHOOTING_BULLET_INFO> _shootingBulletInfo;

	std::shared_ptr<Muscle::UIRenderer> _rushButtonUI;

private:
	std::vector<std::shared_ptr<Muscle::GameObject>> _rushBulletPrefabPool;

	int _rushBulletPrefabIndex = 0;

	float _rushRemainingTime = 0.f;

	bool _isInvincibility;

public:
	inline std::shared_ptr<Muscle::GameObject> GetRushBullet()
	{
		_rushBulletPrefabPool[_rushBulletPrefabIndex]->SetIsEnable(true);

		return _rushBulletPrefabPool[_rushBulletPrefabIndex++];
	}

	inline void StoreRushBullet(std::shared_ptr<Muscle::GameObject> rushBullet) { rushBullet->SetIsEnable(false); }

	inline float GetRushRemainingTime() { return _rushRemainingTime; }

	inline void SetRushRemainingTime(const float& time) { _rushRemainingTime = time; }

	inline void ResetRushBulletPrefabIndex() { _rushBulletPrefabIndex = 0; }

private:
	bool _isStun;

	bool _isPanic;

public:
	std::shared_ptr<Muscle::UIRenderer> GetRushButtonUI() { return _rushButtonUI; }

	const int& GetCurrentHP() { return _hp; }

	void SetHPUpBuff();

	void SetSpeedUpBuff();

	void SetPowerUpBuff();

	// 스피드 감소 너프
	void SetSpeedDown();

	// 정상화
	void SetSpeedNormal();

	// 스턴 상태
	void SetStun();

	// 스턴 해제
	void SetNotStun();

	// 패닉 상태
	void SetPanic();

	// 패닉 해제
	void SetNotPanic();

	inline bool GetIsStun() { return _isStun; }

	inline bool GetIsPanic() { return _isPanic; }

	inline void AddComboCount() { _comboCount++; }

private:
	// 적에게 가할 데미지를 계산합니다. (적 속성, 내 탄환 속성 등 ..)
	int CalcDamage(std::shared_ptr<Character> enemy);

	void LookEnermy();

public:
	void SetState(Status status);

	// 캐릭터를 무적으로 만든다.
	void SetInvincible();

	// 무적 해제
	void SetNotInvincible();

	virtual void Attack(std::shared_ptr<Character> enermy) override;

	virtual void Attack() override;

	virtual void Hit(int damage) override;

	virtual void Move(const Vector3& directional) override;

	virtual void Die() override;

	virtual void Evasion() override;

	virtual void Finalize() override;

	virtual StateColor GetColor() {return StateColor(); };

public:
	void PostShootingBulletInfo(Player::SHOOTING_BULLET_INFO info);

	inline const Status& GetCheckStatus() { return _checkStatus; }

	virtual void Start();

	virtual void Update();

	friend class Controller;

	friend class Inventory;

	friend class Player_Prefab;

	friend class ItemUIPanel;
};