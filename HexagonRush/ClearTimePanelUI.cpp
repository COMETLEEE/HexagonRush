#include "pch.h"
#include "ClearTimePanelUI.h"

ClearTimePanelUI::ClearTimePanelUI(std::shared_ptr<Muscle::GameObject> gameObj) :
	IComponents(gameObj)
{

}

ClearTimePanelUI::~ClearTimePanelUI()
{

}

void ClearTimePanelUI::Start()
{
	std::vector<std::shared_ptr<Muscle::GameObject>> childs = GetGameObject()->GetChildrens();

	for (int i = 0 ; i < childs.size() ; i++)
	{
		_renderer[i] = childs[i]->GetComponent<Muscle::UIRenderer>();
	}

	// 이전 씬에서 걸린 게임 플레이 시간
	float playTime = StageClearScene::GetPrevScenePlayTime();

	// 플레이 시간, 정수. 초단위
	int playTimeint = static_cast<int>(playTime);

	int minute = playTimeint / 60;

	int sec = playTimeint % 60;

	// 플레이 시간, 스트링
	std::string playMinute = std::to_string(minute);

	std::string playSec = std::to_string(sec);

	// 길이가 2, 2가 될 때 까지 만들어줍시다.
	while (playMinute.length() < 2)
		playMinute = '0' + playMinute;

	while (playSec.length() < 2)
		playSec = '0' + playSec;

	for (int i = 0; i < 2 ; i++)
	{
		// 숫자에 따라서 분기쓰
		char num = (playMinute[i] - '0');
		
		std::string sName = "_" + std::to_string(num);

		std::wstring name = std::wstring(sName.begin(), sName.end());

		_renderer[i]->GetUIData()->_drawInfo->_textureID = ResourceManager::Get()->GetTextureID(name);
	}

	// 세미콜론
	_renderer[2]->GetUIData()->_drawInfo->_textureID = ResourceManager::Get()->GetTextureID(TEXT("Colone"));

	for (int i = 3; i < 5; i++)
	{
		// 숫자에 따라서 분기쓰
		char num = (playSec[i - 3] - '0');

		std::string sName = "_" + std::to_string(num);

		std::wstring name = std::wstring(sName.begin(), sName.end());

		_renderer[i]->GetUIData()->_drawInfo->_textureID = ResourceManager::Get()->GetTextureID(name);
	}
}

void ClearTimePanelUI::Update()
{

}