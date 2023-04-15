#pragma once

class SceneFader;
class ButtonBase;

// Á×À¸¸é ¿À´Â ¾À
class DieScene : public Muscle::IScene
{
public:
	DieScene();

	virtual ~DieScene();

private:
	static std::string _retrySceneName;

	bool _isAnyBottonClicked;

	std::shared_ptr<SceneFader> _sceneFader;

	std::shared_ptr<ButtonBase> _retryButton;

	std::shared_ptr<Muscle::UIRenderer> _retryButtonRenderer;

	std::shared_ptr<ButtonBase> _titleButton;

	std::shared_ptr<Muscle::UIRenderer> _titleButtonRenderer;

	std::shared_ptr<Muscle::UIRenderer> _cursorRenderer;

public:
	static void SetRetrySceneName(std::string sceneName);

	virtual void Start() override;
	
	virtual void RapidUpdate() override;

	virtual void Update() override;
};