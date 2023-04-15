#pragma once
#include "ResourceBase.h"

namespace GraphicsEngine
{
	class Texture : public ResourceBase<RESOURCE_TYPE::TEXTURE>
	{
	public:
		Texture();

		virtual ~Texture();

		void Initialize(const tstring& textureName, const tstring& path);

		virtual void Release() override;

		ComPtr<ID3D11ShaderResourceView> GetSRV() { return _shaderResourceView; }

		ComPtr<ID3D11UnorderedAccessView> GetUAV() { return _unorderedAccessView; }

		inline const uint32& GetWidth() { return _width; }

		inline const uint32& GetHeight() { return _height; }

	public:
		static std::shared_ptr<Texture> CreateEmptyTextureCube(const uint32& width, const uint32& height,
			DXGI_FORMAT format = DXGI_FORMAT_R16G16B16A16_FLOAT, 
			D3D11_BIND_FLAG plusBindFlag = D3D11_BIND_UNORDERED_ACCESS,
			const uint32& levels = 0);

		static std::shared_ptr<Texture> CreateEmptyTexture2D(const uint32& width, const uint32& height,
			DXGI_FORMAT format = DXGI_FORMAT_R16G16B16A16_FLOAT, 
			D3D11_BIND_FLAG plusBindFlag = D3D11_BIND_UNORDERED_ACCESS, 
			const uint32& levels = 0);

		void CreateTextureUAV(uint32 mipSlice);
		
		void CreateTextureRTV(uint32 mipSlice);

		template<typename T> static constexpr T NumMipmapLevels(T width, T height)
		{
			T levels = 1;

			while ((width | height) >> levels)
			{
				++levels;
			}

			return levels;
		}

	private:
		DXGI_FORMAT _format;

		uint32 _levels;

		uint32 _width;

		uint32 _height;

		ScratchImage _image;

		ComPtr<ID3D11Texture2D> _texture2D;

		ComPtr<ID3D11ShaderResourceView> _shaderResourceView;

		ComPtr<ID3D11UnorderedAccessView> _unorderedAccessView;

		ComPtr<ID3D11RenderTargetView> _renderTargetView;
	
		friend class ResourceManager;

		friend class Graphics_DX11;
	};
}