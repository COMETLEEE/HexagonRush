#pragma once
class BossAttackBox : public Muscle::IComponents
{
public:
	BossAttackBox(std::shared_ptr<Muscle::GameObject> gameObject);
	virtual ~BossAttackBox() = default;
private:
	int _damage; // 일단 1 고정.

	float _lifeTime;

	float _time;

public:

	void SetLifeTime(float lifeTime) { _lifeTime = lifeTime; };
	void SetDamage(float damage) { _damage = damage; } ;
	virtual void OnEnterCollision(std::shared_ptr<Muscle::Collider> _Other) override;

	virtual void Start() override;

	virtual void Update() override;
};

