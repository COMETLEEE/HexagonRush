#pragma once
class ItemUIPanel : public Muscle::IComponents
{
public:
	ItemUIPanel(std::shared_ptr<Muscle::GameObject> gameObject);

	virtual ~ItemUIPanel();

private:
	std::shared_ptr<Inventory> _playerInventory;

	std::shared_ptr<Player> _player;

	std::shared_ptr<Muscle::UIRenderer> _hpItem;

	std::shared_ptr<Muscle::UIRenderer> _powerItem;

	std::shared_ptr<Muscle::UIRenderer> _speedItem;

	std::shared_ptr<Muscle::UIRenderer> _rgbSign;

	std::shared_ptr<NumberRenderer> _numberRenderer[3];

	std::shared_ptr<NumberRenderer> _coolTimeRender[3];

	std::shared_ptr<Muscle::UIRenderer> _comboUI;

	std::shared_ptr<Muscle::GameObject> _fireSprite;public:

	virtual void Start() override;

	virtual void Update() override;
};