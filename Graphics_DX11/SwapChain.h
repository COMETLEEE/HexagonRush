#pragma once
#include "WindowInfo.h"

namespace GraphicsEngine
{
	class Device;

	class RenderTarget;

	class DepthStencilBuffer;

	class SwapChain
	{
	public:
		void Initialize(WindowInfo& info, std::shared_ptr<Device> device, int screenWidth, int screenHeight);

		void BeginRenderToBackBuffer(ComPtr<ID3D11DeviceContext> deviceContext);

		void Release();

		void OnResize(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext, int screenWidth, int screenHeight);

		void Present();

		ComPtr<IDXGISwapChain> GetSwapChain() { return _swapChain; }

	public:
		ComPtr<IDXGISwapChain> _swapChain;

		ComPtr<IDXGISwapChain1> _swapChain1;

		std::shared_ptr<RenderTarget> _backBuffer;

		std::shared_ptr<DepthStencilBuffer> _depthStencilBuffer;
	};
}