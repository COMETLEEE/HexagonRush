#include "pch.h"
#include "Font.h"
#include "SpriteBatch.h"

#include "Renderer.h"

namespace GraphicsEngine
{
	Font::Font() : 
		ResourceBase<RESOURCE_TYPE::FONT>(), _spriteFont(nullptr), _lineSpacing(15.f)
	{

	}

	Font::~Font()
	{

	}

	void Font::Initialize(const tstring& fontName, const tstring& path)
	{
		_resourceName = fontName;

		_spriteFont = std::make_shared<SpriteFont>(Renderer::Get()->GetDevice().Get(), StringHelper::ToWString(path).c_str());

		_spriteFont->SetLineSpacing(_lineSpacing);
	}

	void Font::Release()
	{
		_spriteFont.reset();
	}

	RECT Font::MeasureDrawBounds(const tstring& text, const Vector2& startCoord, bool ignoreWhiteSpace)
	{
		return _spriteFont->MeasureDrawBounds(StringHelper::ToString(text.c_str()).c_str(), 
			startCoord, ignoreWhiteSpace);
	}

	float Font::GetLineSpacing()
	{
		return _lineSpacing;
	}

	void Font::SetLingSpacing(float value)
	{
		_spriteFont->SetLineSpacing(value);

		_lineSpacing = _spriteFont->GetLineSpacing();
	}

	void Font::DrawTextColor(std::shared_ptr<SpriteBatch> spriteBatch, const Vector2& startCoord, const Vector4& color, const tstring& outText)
	{
		_spriteFont->DrawString(spriteBatch.get(), StringHelper::ToWString(outText).c_str(), startCoord, XMLoadFloat4(&color));
	}
}