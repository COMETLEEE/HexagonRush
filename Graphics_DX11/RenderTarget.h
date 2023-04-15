#pragma once

namespace GraphicsEngine
{
	class RenderTarget
	{
	public:
		void Initialize(ComPtr<ID3D11Device> device, uint32 textureWidth, uint32 textureHeight, 
			bool usingMSAA = false, DXGI_FORMAT textureFormat = DXGI_FORMAT_R16G16B16A16_FLOAT);

		void OnResize(ComPtr<ID3D11Device> device, uint32 textureWidth, uint32 textureHeight);

		void Release();

	public:
		ComPtr<ID3D11RenderTargetView>& GetRTV() { return _renderTargetView; }

		ComPtr<ID3D11ShaderResourceView>& GetSRV() { return _shaderResourceView; }

		void ClearRenderTargetView(ComPtr<ID3D11DeviceContext> deviceContext);

		ComPtr<ID3D11ShaderResourceView>& GetSRVResolved() { return _shaderResourceView; }

		void CreateResolveTexture(ComPtr<ID3D11DeviceContext> deviceContext);

		const uint32& GetTextureWidth() { return _textureWidth; }

		const uint32& GetTextureHeight() { return _textureHeight; }

	private:
		void CreateAllViews(ComPtr<ID3D11Device> device, uint32& textureWidth, uint32& textureHeigh, 
			bool usingMSAA, DXGI_FORMAT textureFormat);

		void InitializeForResolvedTexture(ComPtr<ID3D11Device> device, uint32 textureWidth, uint32 textureHeight, 
			DXGI_FORMAT textureFormat);

	private:
		uint32 _textureWidth;

		uint32 _textureHeight;

		DXGI_FORMAT _textureFormat;

		bool _usingMSAA;

		ComPtr<ID3D11Texture2D> _renderTargetTexture;

		ComPtr<ID3D11RenderTargetView> _renderTargetView;

		ComPtr<ID3D11ShaderResourceView> _shaderResourceView;

		ComPtr<ID3D11UnorderedAccessView> _unorderedAccessView;

		ComPtr<ID3D11Texture2D> _renderTargetTextureResolved;

		ComPtr<ID3D11ShaderResourceView> _shaderResourceViewResolved;
	};
}