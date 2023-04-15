#pragma once

class ButtonBase;

class Player;

class PausePanelController : public Muscle::IComponents
{
	enum class PAUSE_PANEL_STATE
	{
		RESUME_SELECTED = 0,
		RESTART_SELECTED = 1,
		TITLE_SELECTED = 2,
		EXIT_SELECTED = 3,

		BUTTON_COUNT = 4,
		
		NONE = 5,
	};

public:
	PausePanelController(std::shared_ptr<Muscle::GameObject> gameObj);

	virtual ~PausePanelController();

private:
	bool _isPopUp;

	PAUSE_PANEL_STATE _currPanelState;
	
	std::shared_ptr<Muscle::UIRenderer> _cursorRenderer;

	std::shared_ptr<Muscle::UIRenderer> _panelRenderer;

	std::shared_ptr<ButtonBase> _resume;

	std::shared_ptr<Muscle::UIRenderer> _resumeRenderer;

	std::shared_ptr<ButtonBase> _restart;

	std::shared_ptr<Muscle::UIRenderer> _restartRenderer;

	std::shared_ptr<ButtonBase> _title;

	std::shared_ptr<Muscle::UIRenderer> _titleRenderer;

	std::shared_ptr<ButtonBase> _exit;

	std::shared_ptr<Muscle::UIRenderer> _exitRenderer;

	std::vector <std::shared_ptr<Muscle::UIRenderer>> _renderers;

	std::vector<std::shared_ptr<ButtonBase>> _buttons;

	std::shared_ptr<Player> _player;

public:
	// ¹öÆ°°ú ·»´õ·¯ ´Ù ²ö´Ù.
	void AllComponentControl(bool value);

	// ·»´õ·¯¸¸ ²ö´Ù.
	void RendererComponentControl(bool value);

	virtual void Start() override;

	virtual void Update() override;
};