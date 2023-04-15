#pragma once

/// <summary>
/// 인터페이스 클래스이다..!
/// 공격을 주고 받기위해서 캐릭터 클래스를 상속시켜 만든다!
/// </summary>

class Character : public Muscle::IComponents
{
public:
	Character(std::shared_ptr<Muscle::GameObject> gameObj) :IComponents(gameObj), _speed(8.0f), _hp(5), _damage(1), _target()
	{
	}

	virtual ~Character() = default;

protected:
	std::weak_ptr<Character> _target; // 공격 대상! 

	std::shared_ptr<Muscle::Transform> _Trasform;

	std::shared_ptr<Muscle::Animater> _animater;

	float _speed;

	int _hp;

	int _Maxhp;

	int _damage;

public:
	virtual void Start()
	{
		_Trasform = GetGameObject()->GetTransform();

		_animater = GetComponent<Muscle::Animater>();
	}; // Transfrom 캐싱 시켜주자

	virtual void Attack(std::shared_ptr<Character> enermy) abstract; // 공격하다!

	virtual void Attack() abstract; // 공격하다!

	virtual void Hit(int damage) abstract; // 공격을 당하다!

	virtual void Die() abstract; // 공격을 당하다!

	virtual void Move(const Vector3& directional) {};

	virtual void Evasion() {};

	virtual StateColor GetColor() abstract;

	int GetHp() { return _hp; };

	int GetMaxHp() { return _Maxhp; };

	virtual void Finalize() override
	{
		_target.reset();
		_Trasform.reset();
		_animater.reset();
	}

	std::shared_ptr<Character> GetTarget() { return _target.lock(); }

	void SetTarget(std::shared_ptr<Character> teraget)
	{
		_target = teraget;
	};

	std::shared_ptr<Muscle::Animater> GetAnimater()
	{
		return _animater;
	};


public:
	const float& GetSpeed() { return _speed; }

};

