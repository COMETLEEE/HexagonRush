#pragma once
#include "WindowInfo.h"

namespace GraphicsEngine
{
	class DepthStencilBuffer
	{
	public:
		void Initialize(ComPtr<ID3D11Device> device, uint32 textureWidth, uint32 textureHeight, 
			bool usingMSAA = false, bool usingShadowMap = false);


		void Release();

		void OnResize(ComPtr<ID3D11Device> device, uint32 textureWidth, uint32 textureHeight);

		void ClearDepthStencilView(ComPtr<ID3D11DeviceContext> deviceContext);

	public:
		ComPtr<ID3D11Texture2D>& GetDepthStencilBuffer() { return _depthStencilBuffer; }

		ComPtr<ID3D11DepthStencilView>& GetDSV() { return _depthStencilView; }

		ComPtr<ID3D11ShaderResourceView>& GetSRV() { return _shaderResourceView; }

	private:
		void InitializeForShadowMap(ComPtr<ID3D11Device> device, uint32 textureWidth, uint32 textureHeight);
		
		bool _usingMSAA;

		bool _usingShadowMap;

		ComPtr<ID3D11Texture2D> _depthStencilBuffer;

		ComPtr<ID3D11DepthStencilView> _depthStencilView;

		ComPtr<ID3D11ShaderResourceView> _shaderResourceView;
	};
}