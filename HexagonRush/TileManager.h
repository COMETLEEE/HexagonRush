#pragma once

/// <summary>
/// 생각보다 타일이 하는일이 많다..
/// 하늘에서 떨어지거나,
/// 일정 시간이 되면 점점 영역이 줄어들거나,
/// 타일 매니저가 필요할 듯 하다.
/// </summary>
class TileManager : public Muscle::IComponents
{
public:
	struct SELECTED_PLATFORM_INFO
	{
		uint32 _count;

		std::vector<StateColor> _platColors;
	};

	struct ATTACK_PLATFORM_INFO
	{
		uint32 _count;

		StateColor _color;
	};

public:
	TileManager(std::shared_ptr<Muscle::GameObject> gameObj);

	virtual ~TileManager();

private:
	std::vector<std::shared_ptr<Platform>> _tiles; // 필드에 깔려있는 타일들.

	std::vector<std::shared_ptr<Platform>> _tilePool; // 천장에 보관중인 타일. 타일 풀 정도라고 생각하면 될 듯.

	std::list<std::shared_ptr<Platform>> _selectPlatforms;	// 현재 플레이어가 선택한 타일

private:
	static std::shared_ptr<TileManager> _instance;

	SELECTED_PLATFORM_INFO _selectedPlatformInfo;

	ATTACK_PLATFORM_INFO _attackPlatformInfo;
	
	float _endTimer = 3.0f;

	float _timer = 0; // 전체 게임 시간을 측정한다.

	float _deleteTimer = 0;

	int _deleteIndex = 0;

public:
	void RandDropTile();

	void PushBackTile(std::shared_ptr<Platform> _tile);

	static std::shared_ptr<TileManager> Get();

	virtual void Start() override;

	virtual void Update() override;

	virtual void Finalize() override;

	void SelectCancel(); // 선택 모든 타일 선택 취소

	void SelectDelete(); // 선택 모든 색상 같으니 타일 삭제!

	bool CheckPlatforms();

	void SelectTile(std::shared_ptr<Platform> tile);

	bool FindSelectTile(std::shared_ptr<Platform> tile);


	// 현재 선택된 플랫폼들의 정보를 함축해서 반환한다ㅏ.
	const SELECTED_PLATFORM_INFO& GetCurrentSelectedInfo();

	// 쓸 때 잘 쓰셔야합니다. 공격 업데이트가 끝난 직후 ..! 사용할 것
	const ATTACK_PLATFORM_INFO& GetCurrentAttackInfo();
};