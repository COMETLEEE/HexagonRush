#pragma once

class PausePanel_Prefab;

class HPPanel_Prefab;

class Stage2Scene : public Muscle::IScene
{
public:
	Stage2Scene();

	virtual ~Stage2Scene();

	static float _playTime;

	static float GetPlayTime();

private:
	// UI Prefab 여기에 놔두어야 멤버 변수 참조해서 컨트롤링 가능
	std::shared_ptr<PausePanel_Prefab> _pausePanelUI;

	std::shared_ptr<HPPanel_Prefab> _hpPanelUI;

	std::shared_ptr<Muscle::GameObject> _player;

private:
	void CreateStageTwoObjects();

public:
	virtual void Start() override;

	virtual void RapidUpdate() override;

	virtual void Update() override;
};

