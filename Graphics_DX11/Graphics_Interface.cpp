#include "pch.h"
#include "Graphics_Interface.h"

// Graphics Renderer 구현부. => DX11를 반환하겠습니다.
#include "Graphics_DX11.h"

std::shared_ptr<Graphics_Interface> Graphics_Interface::_graphics = nullptr;

std::shared_ptr<Graphics_Interface> Graphics_Interface::Get()
{
	if (_graphics == nullptr)
		_graphics = std::make_shared<GraphicsEngine::Graphics_DX11>();

	return _graphics;
}