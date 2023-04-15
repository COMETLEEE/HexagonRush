#pragma once

class SceneFader;

/// <summary>
/// 게임 중 특정 키를 눌렀을 때 화면에 나오는 Object.
/// 
/// 각 씬별로 해당 키 누를 때 PausePanel Enable 켜주면 될 듯 ?
/// 
/// 아니면 그냥 만들어버리던지 ㅋㅋ .. 게임 타임은 멈추어야 한다.
/// </summary>
class PausePanel_Prefab : public IPrefab
{
public:
	PausePanel_Prefab(std::string currSceneName);

	virtual ~PausePanel_Prefab();

private:
	std::string _currSceneName;

	std::shared_ptr<SceneFader> _fader;
};