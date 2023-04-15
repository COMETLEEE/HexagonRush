#pragma once

/// <summary>
/// 2_Boss_idle_00
/// 2_Boss_Jump_01
/// 2_Boss_Stomp_02
/// 2_Boss_thrusting_03
/// 2_Boss_Dril_launch_04
/// 2_Boss_thrusting_DrilUp_05
/// 2_Boss_oilBlack_Bob_06
/// 2_Boss_electronicYellow_Bob_07
/// 2_Boss_LandmineRed_Bob_08
/// 2_Boss_PanicBlue_Bob_09
/// 2_Boss_Dead_10
/// </summary>

class Second_Boss : public Character, public FSM<Second_Boss>
{
public:
	enum Status
	{
		ATTACK1, // jump
		ATTACK2, // stomp
		ATTACK3, // thrusting

		ATTACK4, // dril_lauch
		ATTACK6, // oilBlack_Bob
		ATTACK7, // electronicYellow

		ATTACK5, // dril_up
		ATTACK8, // LandminedRed
		ATTACK9, // PanicBlue

		IDLE,
		HIT,
		DIE,
		NONE
	};

	enum Phase
	{
		Phase1,
		Phase2,
		Phase3,
		Phase4
	};
public:
	Second_Boss(std::shared_ptr<Muscle::GameObject> gameObj);

	virtual ~Second_Boss();

private:
	Status _checkStatus; // 스테이터스의 상태를 체크하기 위한 변수. // Enter Stay Exit를 위해 필요함.

	StateColor _currentColor; // 현재 속성의 색상,

	std::list<std::shared_ptr<Muscle::SkinnedMeshRenderer>> _coreRenderer; // 색상 변환하는 렌더를 모아놓은 리스트.

	std::vector<std::shared_ptr<NumberRenderer>> _numberRenderers;

	Vector3 _initPosition; // 쉐이크로 위치가 바뀐다.. 초기지점으로 돌려주자..!

	std::shared_ptr<Muscle::GameObject> _rightHand;

	std::shared_ptr<Muscle::GameObject> _dril;

	float _time = 0;

	float _colorTimer = 0;

	float _attackSpeed = 5.0f;

	Phase _phase = Phase::Phase1;
public:
	std::shared_ptr<Muscle::GameObject> GetDrill() { return _dril; }

	virtual StateColor GetColor() { return _currentColor; };

	Vector3 GetInitialPosition() { return _initPosition; }

	void SetColor(StateColor color);

	void SetState(Status status);

	virtual void Attack(std::shared_ptr<Character> enermy) override; // 공격하다!

	virtual void Attack() override; // 

	virtual void Hit(int damage) override; // 공격을 당하다!

	virtual void Die() override; // 죽다!

	virtual void Finalize() override;

	void ChangePhase(Phase phase);

	Phase GetPhase() { return _phase; }

public:
	virtual void Start() override;

	virtual void Update() override;
};

