#pragma once

// 선택되면, 지정된 이벤트를 실행한다.
class ButtonBase : public Muscle::IComponents
{
public:
	enum class BUTTON_STATE
	{
		NONE,

		SELECTED,

		ON_CLICKED,
	};

public:
	ButtonBase(std::shared_ptr<Muscle::GameObject> gameObj);

	virtual ~ButtonBase();

private:
	std::shared_ptr<Muscle::UIRenderer> _uiRenderer;

	BUTTON_STATE _currButtonState;

	std::function<void(void)> _callBack;

public:
	void RegistCallBack(std::function<void(void)> callBack);

	inline BUTTON_STATE GetButtonState() { return _currButtonState; }

	inline void SetButtonState(BUTTON_STATE state) { _currButtonState = state; }

	virtual void Start() override;

	virtual void Update() override;
};

// On Button 뭐시기 만들어도 되는데 .. 어차피 알파 값으로만 조정할 것 같으니 안 만들겠음.