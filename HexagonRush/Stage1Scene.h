#pragma once

class PausePanel_Prefab;

class HPPanel_Prefab;

class Stage1Scene : public Muscle::IScene
{
public:
	Stage1Scene();

	virtual ~Stage1Scene();

	static float _playTime;

	static float GetPlayTime();

	std::shared_ptr<PausePanel_Prefab> _pausePanelUI;

	std::shared_ptr<HPPanel_Prefab> _hpPanelUI;

	std::shared_ptr<Muscle::GameObject> _player;

	std::shared_ptr<Muscle::GameObject> testCol;

private:
	void CreateStageOneObjects();

public:
	virtual void RapidUpdate() override;

	virtual void Start() override;

	virtual void Update() override;
};