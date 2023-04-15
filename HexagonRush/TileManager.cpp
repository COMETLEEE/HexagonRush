#include "pch.h"
#include "TileManager.h"

#define MAX_SELECTED_PLATFORM 5

std::shared_ptr<TileManager> TileManager::_instance = nullptr;

TileManager::TileManager(std::shared_ptr<Muscle::GameObject> gameObj) : IComponents(gameObj)
{
	_selectedPlatformInfo._count = 0;

	_selectedPlatformInfo._platColors = std::vector<StateColor>(5);
}

TileManager::~TileManager()
{

}

void TileManager::RandDropTile()
{
	int index = Muscle::CTime::GetRand<int>(0, _tiles.size() - 1);

	for (auto& iter : _tilePool)
	{
		if (iter->GetState() == PlatStatus::Idle)
		{
			if (_tiles[index]->GetState() == PlatStatus::PermanentlyDelete) return;
			iter->Drop(_tiles[index]);
			break;
		}
	}

}

void TileManager::PushBackTile(std::shared_ptr<Platform> _tile)
{
	_tiles.emplace_back(_tile);
	// 타일 생성 후 타일매니저에 넣어서 관리한다..
}

std::shared_ptr<TileManager> TileManager::Get()
{
	if (!_instance)
		_instance = Muscle::CreateGameObject()->AddComponent<TileManager>();

	return _instance;
}

void TileManager::Start()
{
	_tilePool.resize(30); // 30개 정도면 여유로울듯?

	for (auto& iter : _tilePool)
	{
		auto platform = std::make_shared<Platform_Prefab>()->GetGameObject()->GetComponent<Platform>();
		platform->GetGameObject()->SetIsEnable(false);
		platform->SetPosition(Vector3(0.0f, 20.0f, 0.0f));
		platform->GetGameObject()->GetComponent<Muscle::Collider>()->SetIsEnable(false);
		iter = platform;
	}
}

void TileManager::Update()
{
	if (CameraMovementManager::Get()->GetisMove()) return;
	if (_deleteIndex > 84)return;
	// 3분부터 하나씩 제거
	// 5초 단위
	// 5분 이후 한번에 모두 제거.
	_timer += Muscle::CTime::GetGameDeltaTime();
	_deleteTimer += Muscle::CTime::GetGameDeltaTime();

	if (_timer < 90.0f) return;
	if (_deleteTimer < _endTimer) return;

	_endTimer -= 3.0f * Muscle::CTime::GetGameDeltaTime();

	if (_endTimer < 0.2f)
		_endTimer = 0.2f;

	_deleteTimer = 0;
	_tiles[_deleteIndex++]->PermanentlyDelete();


}

void TileManager::Finalize()
{
	for (auto& iter : _tiles)
	{
		iter.reset();
	}

	_tiles.clear();

	for (auto& iter : _tilePool)
	{
		iter.reset();
	}

	_tilePool.clear();

	for (auto& iter : _selectPlatforms)
	{
		iter.reset();
	}

	_selectPlatforms.clear();


	_instance.reset();
}

void TileManager::SelectCancel()
{
	for (auto& iter : _selectPlatforms)
	{
		iter->SelectCanclePlatform();
	}

	_selectPlatforms.clear();
}

void TileManager::SelectDelete()
{
	for (auto& iter : _selectPlatforms)
	{
		iter->DeletePlatform();
	}

	_selectPlatforms.clear();
}

bool TileManager::CheckPlatforms()
{
	StateColor Color = StateColor::None;

	if (_selectPlatforms.size() < 3)
	{
		return false;
	}

	// 모든 블럭들의 색깔이 같은지 검사한다.
	// 하나라도 다른 경우이면 return false;
	for (auto& iter : _selectPlatforms)
	{
		if (Color == StateColor::None)
			Color = iter->GetColor();

		if (Color == StateColor::None) return false;

		if (Color != iter->GetColor())
		{
			return false;
		}
	}

	_attackPlatformInfo._color = Color;

	_attackPlatformInfo._count = _selectPlatforms.size();

	return true;
}

void TileManager::SelectTile(std::shared_ptr<Platform> tile)
{
	// 5개 이상은 밟을 수 없다 ..!
	if (_selectPlatforms.size() >= MAX_SELECTED_PLATFORM)
		return;

	for (auto& iter : _selectPlatforms)
	{
		if (iter != tile)
			continue;
		else
			return;				// 이미 셀렉트 된 플랫폼은 추가로 선택하지 않는다.
	}

	_selectPlatforms.emplace_back(tile);

	tile->SelectPlatform();
}

bool TileManager::FindSelectTile(std::shared_ptr<Platform> tile)
{
	for (auto& iter : _selectPlatforms)
	{
		if (iter != tile)
			continue;
		else
			return true; // 있따!
	}
	return false; // 없다!
}

const TileManager::SELECTED_PLATFORM_INFO& TileManager::GetCurrentSelectedInfo()
{
	_selectedPlatformInfo._count = _selectPlatforms.size();

	_selectedPlatformInfo._platColors.clear();

	for (auto& platform : _selectPlatforms)
	{
		const StateColor& color = platform->GetColor();

		_selectedPlatformInfo._platColors.push_back(color);
	}

	return _selectedPlatformInfo;
}

const TileManager::ATTACK_PLATFORM_INFO& TileManager::GetCurrentAttackInfo()
{
	return _attackPlatformInfo;
}