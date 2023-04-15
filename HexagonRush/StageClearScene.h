#pragma once

// 스테이지 클리어하면 들어오는 씬. 버튼 한 개만 일단 놔두자 ..
// 8월 19일 오전 12시 5분 .. 어떻게 벌써 열 두시 ~
class StageClearScene : public Muscle::IScene
{
public:
	StageClearScene();

	virtual ~StageClearScene();

	static float GetPrevScenePlayTime();

private:
	static std::string _nextSceneName;

	static float _playTimePrevScene;


	bool _isAnyBottonClicked;

	std::shared_ptr<SceneFader> _sceneFader;

	std::shared_ptr<ButtonBase> _nextButton;

	std::shared_ptr<Muscle::UIRenderer> _nextButtonRenderer;

public:
	static void SetNextSceneName(std::string sceneName);

	static void SetPrevScenePlayTime(float time);

	virtual void Start() override;

	virtual void RapidUpdate() override;
	
	virtual void Update() override;
};