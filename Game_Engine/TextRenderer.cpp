#include "pch.h"
#include "TextRenderer.h"

Muscle::TextRenderer::TextRenderer(std::shared_ptr<GameObject> _GameObject) :IComponents(_GameObject)
{
	_textData = std::make_shared<TextData>();
}

Muscle::TextRenderer::~TextRenderer()
{
	// 삭제하는 코드 추가
	_textData.reset();
}

void Muscle::TextRenderer::SetPrintNDCPosition(Vector2 pos)
{
	/*_textData->_startCoord = pos;*/
}

void Muscle::TextRenderer::SetPrintPosition(Vector2 _pos)
{
	//RECT _rect;
	//GetClientRect(MuscleEngine::GetInstance()->GetHWND(), &_rect);
	//_rect.right, _rect.bottom;
	//_pos *= 2;
	//_pos.x = _pos.x / _rect.right - 1;
	//_pos.y = _pos.y / _rect.bottom - 1;

	_textData->_startCoord = _pos;
	//SetPrintNDCPosition(_pos);
}

void Muscle::TextRenderer::SetText(tstring str)
{
	_textData->_outText = str;
}

void Muscle::TextRenderer::SetFont(tstring str)
{
	_textData->_fontID = MuscleEngine::Get()->GetResourceManager()->GetFontID(str);
}

void Muscle::TextRenderer::Start()
{
}

void Muscle::TextRenderer::Update()
{

}

void Muscle::TextRenderer::Render()
{
	if (!IGameEngine::Get()->GetisDebug()) return;

	MuscleEngine::Get()->GetGraphicsManager()->PostTextData(_textData);
}
