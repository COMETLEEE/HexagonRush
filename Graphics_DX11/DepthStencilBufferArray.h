#pragma once

namespace GraphicsEngine
{
	/// <summary>
	/// 그림자를 만들 때 외에는 .. 사용될 일이 사실상 없을 듯 ..?
	/// </summary>
	class DepthStencilBufferArray
	{
	public:
		void Initialize(ComPtr<ID3D11Device> device, uint32 textureWidth, uint32 textureHeight);

		void Release();

		void OnResize(ComPtr<ID3D11Device> device, uint32 textureWidth, uint32 textureHeight);

		void ClearDepthStencilView(ComPtr<ID3D11DeviceContext> deviceContext);

	public:
		ComPtr<ID3D11Texture2D>& GetDepthStencilBuffer() { return _depthStencilBuffer; }

		ComPtr<ID3D11DepthStencilView>& GetDSV() { return _depthStencilView; }

		ComPtr<ID3D11ShaderResourceView>& GetSRV() { return _shaderResourceView; }

	private:
		ComPtr<ID3D11Texture2D> _depthStencilBuffer;

		ComPtr<ID3D11DepthStencilView> _depthStencilView;

		ComPtr<ID3D11ShaderResourceView> _shaderResourceView;
	};
}