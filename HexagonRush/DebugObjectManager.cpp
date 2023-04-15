#include "pch.h"
#include "DebugObjectManager.h"
#include "QuadTree.h"

std::shared_ptr<DebugObjectManager> DebugObjectManager::_instance = nullptr;

DebugObjectManager::DebugObjectManager(std::shared_ptr<Muscle::GameObject> gameObj) :IComponents(gameObj), _gameobject(nullptr), _index(0)
{



}

DebugObjectManager::~DebugObjectManager()
{
	_instance.reset();
}

std::shared_ptr<DebugObjectManager> DebugObjectManager::Get()
{
	if (!_instance)
	{
		_instance = Muscle::CreateGameObject()->AddComponent<DebugObjectManager>();
	}

	return _instance;
}

void DebugObjectManager::PickGameObject(std::shared_ptr<Muscle::GameObject> gameobj)
{
	_gameobject = gameobj;
}

void DebugObjectManager::Start()
{
	_gameobject = Muscle::IGameEngine::Get()->GetObjManager()->m_vectorObjects[_index];

	_timeText = Muscle::CreateGameObject()->AddComponent<Muscle::TextRenderer>();
	_timeText->SetFont(TEXT("Segoe"));
	_timeText->SetPrintPosition(Vector2(10.0f, 330.0f));

	_cameraInfo = Muscle::CreateGameObject()->AddComponent<Muscle::TextRenderer>();
	_cameraInfo->SetFont(TEXT("Segoe"));
	_cameraInfo->SetPrintPosition(Vector2(10.0f, 400.0f));

	_objectInfo = Muscle::CreateGameObject()->AddComponent<Muscle::TextRenderer>();
	_objectInfo->SetFont(TEXT("Segoe"));
	_objectInfo->SetPrintPosition(Vector2(10.0f, 500.0f));

	_objectCountInfo = Muscle::CreateGameObject()->AddComponent<Muscle::TextRenderer>();
	_objectCountInfo->SetFont(TEXT("Segoe"));
	_objectCountInfo->SetPrintPosition(Vector2(1600.0f, 320.0f));

	_bossData = Muscle::CreateGameObject()->AddComponent<Muscle::TextRenderer>();
	_bossData->SetFont(TEXT("Segoe"));
	_bossData->SetPrintPosition(Vector2(1600.0f, 620.0f));

	for (auto& iter : Muscle::IGameEngine::Get()->GetObjManager()->m_vectorObjects)
	{
		if (iter->GetName() == "Boss")
		{
			_boss = iter->GetComponent<Character>(); // 보스의 정보를 출력하기 위해 캐싱..
		}
	}


	std::shared_ptr<Muscle::TextRenderer> text = Muscle::CreateGameObject()->AddComponent<Muscle::TextRenderer>();
	text->SetPrintPosition(Vector2(10.0f, 10.0f));
	text->SetText(TEXT("!Muscle and Comet Engine!"));
	text->SetFont(TEXT("Consolas"));
}

void DebugObjectManager::Update()
{
	if (Muscle::KeyBoard::Get()->KeyDown('1'))
	{
		if (_index > 0)
			_index--;
		PickGameObject(Muscle::IGameEngine::Get()->GetObjManager()->m_vectorObjects[_index]);
	}
	if (Muscle::KeyBoard::Get()->KeyDown('2'))
	{
		if (_index < Muscle::IGameEngine::Get()->GetObjManager()->m_vectorObjects.size() - 1)
			_index++;
		PickGameObject(Muscle::IGameEngine::Get()->GetObjManager()->m_vectorObjects[_index]);
	}
}

std::wstring vector_to_str(Vector3 vec)
{
	std::wstring result = std::to_wstring(vec.x) + L" , " + std::to_wstring(vec.y) + L" , " + std::to_wstring(vec.z);
	return result;
}

void DebugObjectManager::CameraInfoUpdate()
{
	Vector3 camerapos = Muscle::IGameEngine::Get()->GetMainCamera()->GetTransform()->GetWorldPosition();
	std::wstring camerainfo;
	camerainfo = L"Camera Info. \n\nCamera Pos :" + vector_to_str(camerapos) + L"\n\n";
	Vector3 cameralook = Muscle::IGameEngine::Get()->GetMainCamera()->GetTransform()->GetLook();
	camerainfo += L"CameraLook : " + vector_to_str(cameralook);
	_cameraInfo->SetText(camerainfo);

}
void DebugObjectManager::ObjectInfoUpdate()
{
	// todo  : 나중에 픽킹 생기면 추가 할 것.
	std::wstring objectInfo = TEXT("Picked Object Info : \n\n");
	std::wstring objectName, objectPos, objcomponents;
	objcomponents += L"\n\n";
	if (_gameobject)
	{
		objectName.assign(_gameobject->GetName().begin(), _gameobject->GetName().end());

		objectPos = vector_to_str(_gameobject->GetTransform()->GetWorldPosition());

		for (auto& iter : _gameobject->GetComponents())
		{
			std::string convert = typeid(*iter.get()).name();
			std::wstring str;
			str.assign(convert.begin(), convert.end());
			objcomponents += str + L"\n\n";
		}
		objectName += L"\n\n";
		objectPos += L"\n\n";
		objcomponents += L"\n\n";

	}
	else
	{
		objectName = L"Nullptr \n\n";
		objectPos = L"Nullptr \n\n";
		objcomponents = L"Nullptr \n\n";
	}

	objectInfo += L"Object Name : " + objectName;
	objectInfo += L"Object Pos : " + objectPos;
	objectInfo += L"Object Name : " + objcomponents;


	_renderbase = _gameobject->GetComponent<Muscle::RendererBase>();
	if (_renderbase)
	{
		std::wstring roughness, metalic;

		roughness = std::to_wstring(_renderbase->_renderingData->_materialInfo->_roughness);
		metalic = std::to_wstring(_renderbase->_renderingData->_materialInfo->_metallic);

		objectInfo += L"Object Metalic   ++ I / -- U  : " + metalic + L"\n\n";
		objectInfo += L"Object roughness ++ P / -- O  : " + roughness + L"\n\n";

		if (Muscle::KeyBoard::Get()->KeyPress('U'))
			_renderbase->_renderingData->_materialInfo->_metallic -= 0.5f * Muscle::CTime::GetGameDeltaTime();
		if (Muscle::KeyBoard::Get()->KeyPress('I'))
			_renderbase->_renderingData->_materialInfo->_metallic += 0.5f * Muscle::CTime::GetGameDeltaTime();
		if (Muscle::KeyBoard::Get()->KeyPress('P'))
			_renderbase->_renderingData->_materialInfo->_roughness += 0.5f * Muscle::CTime::GetGameDeltaTime();
		if (Muscle::KeyBoard::Get()->KeyPress('O'))
			_renderbase->_renderingData->_materialInfo->_roughness -= 0.5f * Muscle::CTime::GetGameDeltaTime();

	}
	_objectInfo->SetText(objectInfo);

}

void DebugObjectManager::ObjectCountUpdate()
{
	std::wstring str;
	str = L"Object Count : ";
	str += std::to_wstring(Muscle::IGameEngine::Get()->GetObjManager()->m_vectorObjects.size()) + L"\n\n";
	str += L"Collider Count : ";
	str += std::to_wstring(Muscle::IGameEngine::Get()->GetObjManager()->GetColliders().size()) + L"\n\n";
	str += L"QuadTree Leaf Count : ";
	str += std::to_wstring(Muscle::IGameEngine::Get()->GetObjManager()->GetQuadTree()->GetLeafNodes().size()) + L"\n\n";

	_objectCountInfo->SetText(str);
}

void DebugObjectManager::BossInfoUpdate()
{
	std::wstring str;
	if (_boss)
	{
		str = TEXT("Boss Info :\n\n");
		str += TEXT("Boss HP : ");
		str += std::to_wstring(_boss->GetHp()) + L"\n\n";
		str += TEXT("Boss Max : ");
		str += std::to_wstring(_boss->GetMaxHp()) + L"\n\n";
	}
	_bossData->SetText(str);
}

void DebugObjectManager::LateUpdate()
{
	_timeText->SetText(TEXT("FPS : ") + std::to_wstring(Muscle::CTime::GetFPS()));

	CameraInfoUpdate();

	ObjectInfoUpdate();

	ObjectCountUpdate();

	BossInfoUpdate();
}

void DebugObjectManager::Finalize()
{
	_instance.reset();
}
