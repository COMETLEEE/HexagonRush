#pragma once

namespace GraphicsEngine
{
	struct WindowInfo
	{
		HWND _hWnd;
		uint32 _screenWidth;
		uint32 _screenHeight;

		bool _enable4xMSAA;
		uint32 _4xMSAAQuality;
	};
}