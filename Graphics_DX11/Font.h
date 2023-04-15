#pragma once
#include "ResourceBase.h"

namespace GraphicsEngine
{
	class Font : public ResourceBase<RESOURCE_TYPE::FONT>
	{
	public:
		Font();

		virtual ~Font();

		void Initialize(const tstring& fontName, const tstring& path);

		void Release();

		inline std::shared_ptr<SpriteFont> GetSpriteFont() { return _spriteFont; }
		
		RECT MeasureDrawBounds(const tstring& text, const Vector2& startCoord, bool ignoreWhiteSpace = true);

		float GetLineSpacing();

		void SetLingSpacing(float value);

		void DrawTextColor(std::shared_ptr<SpriteBatch> spriteBatch, const Vector2& startCoord, const Vector4& color, const tstring& outText);

	private:
		float _lineSpacing;

		std::shared_ptr<SpriteFont> _spriteFont;
	};
}