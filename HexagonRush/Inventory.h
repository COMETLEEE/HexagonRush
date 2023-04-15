#pragma once

class BuffController;

// 인벤토리 클래스.
// 아이템은 각각 3개 
class Inventory : public Muscle::IComponents
{
public:
	Inventory(std::shared_ptr<Muscle::GameObject> gameObj);

	virtual ~Inventory();

private:
	std::shared_ptr<Player> _player;

	std::shared_ptr<BuffController> _buffController;

	// 아이템 리스트
	uint32 _hpItemCount;

	uint32 _powerUpItemCount;

	uint32 _speedUpItemCount;
	
private:
	// HP Up Item 쓸 수 있는지 체크
	bool CheckCanUseHPItem();

	// Speed Up 쓸 수 있는지 체크
	bool CheckCanUseSpeedItem();

	// Power Up 쓸 수 있는지 체크
	bool CheckCanUsePowerItem();

public:
	uint32 GetHPItemCount() {return _hpItemCount;}

	uint32 GetPowerItemCount() { return _powerUpItemCount; }

	uint32 GetSpeedItemCount() { return _speedUpItemCount; }

	virtual void Start() override;

	virtual void Update() override;
};