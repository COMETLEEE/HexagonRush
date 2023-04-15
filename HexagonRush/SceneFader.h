#pragma once

class SceneFader : public Muscle::IComponents
{
	enum class FADE_STATE
	{
		NONE,
		FADE_IN,
		FADE_OUT,
		FADE_END,
	};

public:
	SceneFader(std::shared_ptr<Muscle::GameObject> gameObj);

	virtual ~SceneFader();

private:
	std::shared_ptr<Muscle::UIRenderer> _uiRenderer;

	std::shared_ptr<RenderingData_UI> _uiData;

public:
	FADE_STATE _currFadeState;

	float _fadeTime;

	float _accumTime;
		
	std::string _nextSceneName;

	std::function<void(void)> _callBack;

	// 콜백 버전, 콜백은 페이드 아웃만 지원한다.
	void FadeOut(float fadeOutTime, std::function<void(void)> callBack);

	void FadeIn(float fadeInTime);

public:
	virtual void Start() override;

	virtual void Update() override;
};