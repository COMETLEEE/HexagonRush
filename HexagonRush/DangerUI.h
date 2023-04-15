#pragma once
class DangerUI : public Muscle::IComponents
{
public:
	DangerUI(std::shared_ptr<Muscle::GameObject> gameObject);

	virtual ~DangerUI();

private:
	std::shared_ptr<Muscle::UIRenderer> _uiRenderer;

	std::shared_ptr<Muscle::UIRenderer> _backGroundRenderer;

	bool isToggle = false;
public:
	virtual void Start() override;

	virtual void Update() override;
};

