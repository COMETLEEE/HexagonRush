#pragma once
class FireSpriteUI : public Muscle::IComponents
{
public:
	FireSpriteUI(std::shared_ptr<Muscle::GameObject>);

	~FireSpriteUI();

private:
	std::vector<UINT> _resourceID;

	std::shared_ptr<Muscle::UIRenderer> _fireRenderer;

	UINT _Animindex;

	float _timer = 0;
public:
	virtual void Start();

	virtual void Update();
};

