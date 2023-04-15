#pragma once

class ClearTimePanelUI : public Muscle::IComponents
{
public:
	ClearTimePanelUI(std::shared_ptr<Muscle::GameObject> gameObj);

	virtual ~ClearTimePanelUI();

	std::shared_ptr<Muscle::UIRenderer> _renderer[5];

public:
	virtual void Start() override;

	virtual void Update() override;
};