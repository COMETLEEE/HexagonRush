#include "pch.h"
#include "ButtonBase.h"

#define BUTTON_ALPHA 0.4f

ButtonBase::ButtonBase(std::shared_ptr<Muscle::GameObject> gameObj) :
	IComponents(gameObj)
{
	_uiRenderer = nullptr;

	_currButtonState = BUTTON_STATE::NONE;
}

ButtonBase::~ButtonBase()
{

}

void ButtonBase::RegistCallBack(std::function<void(void)> callBack)
{
	_callBack = callBack;
}

void ButtonBase::Start()
{
	_uiRenderer = GetGameObject()->GetComponent<Muscle::UIRenderer>();

	_currButtonState = BUTTON_STATE::NONE;
}

void ButtonBase::Update()
{
	switch (_currButtonState)
	{
		case BUTTON_STATE::NONE:
		{
			// 음 .. 할 것 없을 듯
			_uiRenderer->GetUIData()->_drawInfo->_color = Vector4(1.f, 1.f, 1.f, BUTTON_ALPHA);

			_uiRenderer->SetUseAlphaBlend(true);

			break;
		}

		case BUTTON_STATE::SELECTED:
		{
			_uiRenderer->GetUIData()->_drawInfo->_color = Vector4(1.f, 1.f, 1.f, 1.f);

			_uiRenderer->SetUseAlphaBlend(true);

			break;
		}

		case BUTTON_STATE::ON_CLICKED:
		{
			// Call Back !
			if (_callBack != nullptr)
				_callBack();

			_currButtonState = BUTTON_STATE::SELECTED;

			break;
		}
	}
}
