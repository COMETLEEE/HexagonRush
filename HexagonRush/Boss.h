#pragma once

// 스테이지 1 (튜토리얼이였는데 어쩌다보니 그냥 스테이지 1 급의 스케일)
class Boss : public Character, public FSM<Boss>
{
public:
	enum Status
	{
		// 페이즈 1공격
		ATTACK1,
		ATTACK2,
		ATTACK9,
		ATTACK10,
		ATTACK7,

		// 페이즈 2공격
		ATTACK4,
		ATTACK3,

		// 페이즈 3공격
		ATTACK6,
		ATTACK8,


		IDLE,
		HIT,
		DIE,
		NONE
	};

	enum Phase
	{
		Phase1,
		Phase2,
		Phase3
	};

public:
	Boss(std::shared_ptr<Muscle::GameObject> gameObj);

	virtual ~Boss();

private:
	Phase _phase; // 보스 체력에 따른 페이즈, 시작은 1단계

	Status _checkStatus; // 스테이터스의 상태를 체크하기 위한 변수. // Enter Stay Exit를 위해 필요함.

	StateColor _currentColor; // 현재 속성의 색상,

	std::weak_ptr<Muscle::GameObject> _hand1; // 불 발사 지점.

	std::weak_ptr<Muscle::GameObject> _hand2; // 불 발사 지점.

	std::list<std::shared_ptr<Muscle::SkinnedMeshRenderer>> _coreRenderer; // 색상 변환하는 

	std::vector<std::shared_ptr<NumberRenderer>> _numberRenderers;

	Vector3 _initPosition; // 쉐이크로 위치가 바뀐다.. 초기지점으로 돌려주자..!

	float _time = 0;

	float _colorTimer = 0;

	float _attackSpeed = 3.0f;

public:
	virtual StateColor GetColor() { return _currentColor; };

	Vector3 GetInitialPosition() { return _initPosition; }

	void SetColor(StateColor color);

	void SetState(Status status);

	void ChangePhase(Phase phase);

	virtual void Attack(std::shared_ptr<Character> enermy) override; // 공격하다!

	virtual void Attack() override; // 

	virtual void Hit(int damage) override; // 공격을 당하다!

	virtual void Die() override; // 죽다!

	virtual void Finalize() override;

	std::shared_ptr<Muscle::GameObject> GetHand1();

	std::shared_ptr<Muscle::GameObject> GetHand2();

public:
	virtual void Start() override;

	virtual void Update() override;
};