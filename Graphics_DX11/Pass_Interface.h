#pragma once

namespace GraphicsEngine
{
	/// <summary>
	/// Pass¿« Interface
	/// </summary>
	interface Pass_Interface
	{
	public:
		virtual void Initialize(const uint32& screenWidth, const uint32& screenHeight);

		virtual void OnResize(const uint32& screenWidth, const uint32& screenHeight);

		virtual void Release();

	protected:
		ComPtr<ID3D11Device> _device;

		ComPtr<ID3D11DeviceContext> _deviceContext;

		uint32 _screenWidth;

		uint32 _screenHeight;
	};
}