#pragma once

class HPPanelController : public Muscle::IComponents
{
public:
	HPPanelController(std::shared_ptr<Muscle::GameObject> gameObj);

	virtual ~HPPanelController();

private:
	int _currPlayerHP;

	uint64 _hpYesTexID;

	uint64 _hpNoTexID;

	std::vector<std::shared_ptr<Muscle::UIRenderer>> _hpRenderers;

	std::shared_ptr<Player> _playerCom;

public:
	virtual void Start() override;

	virtual void Update() override;
};