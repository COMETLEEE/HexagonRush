#include "pch.h"
#include "Pass_Interface.h"

#include "Renderer.h"

namespace GraphicsEngine
{
	void Pass_Interface::Initialize(const uint32& screenWidth, const uint32& screenHeight)
	{
		_device = Renderer::Get()->GetDevice();

		_deviceContext = Renderer::Get()->GetDeviceContext();

		_screenWidth = screenWidth;

		_screenHeight = screenHeight;
	}

	void Pass_Interface::OnResize(const uint32& screenWidth, const uint32& screenHeight)
	{
		_screenWidth = screenWidth;

		_screenHeight = screenHeight;
	}

	void Pass_Interface::Release()
	{
		_device.Reset();

		_deviceContext.Reset();
	}
}