#include "pch.h"
#include "Texture.h"

#include "Renderer.h"
#include "ShaderManager.h"
#include "ResourceManager.h"

#include "ComputeShader.h"

namespace GraphicsEngine
{
	Texture::Texture() :
		ResourceBase<RESOURCE_TYPE::TEXTURE>(), _image(ScratchImage()), _texture2D(nullptr), _shaderResourceView(nullptr)
	{
	}

	Texture::~Texture()
	{
	}

	void Texture::Initialize(const tstring& textureName, const tstring& path)
	{
		_resourceName = textureName;

		// 텍스쳐 파일 로드 후 그래픽 카드에 전달
		std::wstring ext = fs::path(StringHelper::ToWString(path)).extension();

		if (ext == L".dds" || ext == L".DDS")
			::LoadFromDDSFile(path.c_str(), DDS_FLAGS_NONE, nullptr, _image);
		else if (ext == L".tga" || ext == L".TGA")
			::LoadFromTGAFile(path.c_str(), nullptr, _image);
		else if (ext == L".hdr" || ext == L".HDR")
			::LoadFromHDRFile(path.c_str(), nullptr, _image);
		else// png, jpg, jpeg, bmp
			::LoadFromWICFile(path.c_str(), WIC_FLAGS_NONE, nullptr, _image);

		ComPtr<ID3D11Device> device = Renderer::Get()->GetDevice();

		ComPtr<ID3D11DeviceContext> deviceContext = Renderer::Get()->GetDeviceContext();

		ScratchImage mipChain;

		bool isCubeMap = _image.GetMetadata().IsCubemap();

		HRESULT hr;

		if (isCubeMap)
		{
			hr = GenerateMipMaps(_image.GetImages(), _image.GetImageCount(), _image.GetMetadata(),
				TEX_FILTER_DEFAULT, 0, mipChain);

			hr = ::CreateTexture(device.Get(), mipChain.GetImages(), mipChain.GetImageCount(), mipChain.GetMetadata(),
				reinterpret_cast<ID3D11Resource**>(_texture2D.GetAddressOf()));
		}
		else
			hr = ::CreateTexture(device.Get(), _image.GetImages(), _image.GetImageCount(), _image.GetMetadata(),
				reinterpret_cast<ID3D11Resource**>(_texture2D.GetAddressOf()));

		if (FAILED(hr))
			::MessageBoxA(nullptr, "Texture Create Failed !", nullptr, MB_OK);

		D3D11_TEXTURE2D_DESC textureDesc;

		_texture2D->GetDesc(&textureDesc);

		_width = textureDesc.Width;

		_height = textureDesc.Height;

		_levels = textureDesc.MipLevels;

		_format = textureDesc.Format;

		// 레벨 부여
		if (_levels <= 0)
			 _levels = Texture::NumMipmapLevels(_width, _height);

		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;

		ZeroMemory(&srvDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));

		srvDesc.Format = textureDesc.Format;
		srvDesc.Texture2D.MipLevels = textureDesc.MipLevels;
		srvDesc.Texture2D.MostDetailedMip = 0;

		if (!isCubeMap)
			srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		else
		{
			srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
		}

		hr = device->CreateShaderResourceView(_texture2D.Get(), &srvDesc, _shaderResourceView.GetAddressOf());

		// Mip Level이 만약 0이면 .. Mip Map 형성해주자.
		if (textureDesc.MipLevels == 0)
			deviceContext->GenerateMips(this->_shaderResourceView.Get());
	}

	void Texture::Release()
	{
		_texture2D.ReleaseAndGetAddressOf();

		_shaderResourceView.ReleaseAndGetAddressOf();

		_unorderedAccessView.ReleaseAndGetAddressOf();
	}

	std::shared_ptr<GraphicsEngine::Texture> Texture::CreateEmptyTextureCube(const uint32& width, const uint32& height,
		DXGI_FORMAT format /*= DXGI_FORMAT_R16G16B16A16_FLOAT*/, D3D11_BIND_FLAG plusBindFlag /*= D3D11_BIND_UNORDERED_ACCESS*/,
		const uint32& levels)
	{
		std::shared_ptr<Texture> ret = std::make_shared<Texture>();

		ret->_levels = (levels > 0) ? levels : Texture::NumMipmapLevels(width, height);

		ret->_width = width;

		ret->_height = height;

		ret->_format = format;

		D3D11_TEXTURE2D_DESC desc = {};

		desc.Width = width;
		desc.Height = height;
		desc.MipLevels = ret->_levels;
		desc.ArraySize = 6;
		desc.Format = format;
		desc.SampleDesc.Count = 1;
		desc.Usage = D3D11_USAGE_DEFAULT;

		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | plusBindFlag;
		desc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;

		if (levels == 0) 
		{
			desc.BindFlags |= D3D11_BIND_RENDER_TARGET;

			desc.MiscFlags |= D3D11_RESOURCE_MISC_GENERATE_MIPS;
		}

		ComPtr<ID3D11Device> device = Renderer::Get()->GetDevice();

		if (FAILED(device->CreateTexture2D(&desc, nullptr, ret->_texture2D.GetAddressOf()))) 
		{
			throw std::runtime_error("Failed to create cubemap texture");
		}

		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};

		srvDesc.Format = desc.Format;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
		srvDesc.TextureCube.MostDetailedMip = 0;
		srvDesc.TextureCube.MipLevels = -1;

		if (FAILED(device->CreateShaderResourceView(ret->_texture2D.Get(), &srvDesc, ret->_shaderResourceView.GetAddressOf()))) 
		{
			throw std::runtime_error("Failed to create cubemap texture SRV");
		}

		return ret;
	}

	std::shared_ptr<GraphicsEngine::Texture> Texture::CreateEmptyTexture2D(const uint32& width, const uint32& height, 
		DXGI_FORMAT format /*= DXGI_FORMAT_R16G16B16A16_FLOAT*/, D3D11_BIND_FLAG plusBindFlag /*= D3D11_BIND_UNORDERED_ACCESS*/, const uint32& levels /*= 0*/)
	{
		std::shared_ptr<Texture> ret = std::make_shared<Texture>();

		ret->_levels = (levels > 0) ? levels : Texture::NumMipmapLevels(width, height);

		ret->_width = width;

		ret->_height = height;

		ret->_format = format;

		D3D11_TEXTURE2D_DESC desc = {};
		desc.Width = width;
		desc.Height = height;
		desc.MipLevels = ret->_levels;
		desc.ArraySize = 1;
		desc.Format = format;

		desc.SampleDesc.Count = 1;
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | plusBindFlag;

		if (levels == 0)
		{
			desc.BindFlags |= D3D11_BIND_RENDER_TARGET;
			desc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;
		}

		ComPtr<ID3D11Device> device = Renderer::Get()->GetDevice();

		if (FAILED(device->CreateTexture2D(&desc, nullptr, ret->_texture2D.GetAddressOf())))
		{
			throw std::runtime_error("Failed to create cubemap texture");
		}

		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};

		srvDesc.Format = desc.Format;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.TextureCube.MostDetailedMip = 0;
		srvDesc.TextureCube.MipLevels = -1;

		if (FAILED(device->CreateShaderResourceView(ret->_texture2D.Get(), &srvDesc, ret->_shaderResourceView.GetAddressOf())))
		{
			throw std::runtime_error("Failed to create cubemap texture 2D SRV");
		}

		return ret;
	}

	void Texture::CreateTextureUAV(uint32 mipSlice)
	{
		D3D11_TEXTURE2D_DESC desc;

		this->_texture2D->GetDesc(&desc);

		D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};

		uavDesc.Format = desc.Format;

		// 단일 텍스쳐
		if (desc.ArraySize == 1) 
		{
			uavDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
			uavDesc.Texture2D.MipSlice = mipSlice;
		}
		// 단일 텍스쳐가 아님.
		else 
		{
			uavDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2DARRAY;
			uavDesc.Texture2DArray.MipSlice = mipSlice;
			uavDesc.Texture2DArray.FirstArraySlice = 0;
			uavDesc.Texture2DArray.ArraySize = desc.ArraySize;
		}

		ComPtr<ID3D11Device> device = Renderer::Get()->GetDevice();

		if (FAILED(device->CreateUnorderedAccessView(this->_texture2D.Get(), &uavDesc, this->_unorderedAccessView.GetAddressOf()))) 
		{
			throw std::runtime_error("Failed to create texture UAV");
		}
	}

	void Texture::CreateTextureRTV(uint32 mipSlice)
	{
		D3D11_TEXTURE2D_DESC desc;

		this->_texture2D->GetDesc(&desc);

		D3D11_RENDER_TARGET_VIEW_DESC rtvDesc = {};

		rtvDesc.Format = desc.Format;

		// 단일 텍스쳐
		if (desc.ArraySize == 1)
		{
			rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
			rtvDesc.Texture2D.MipSlice = mipSlice;
		}
		// 단일 텍스쳐가 아님.
		else
		{
			rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;
			rtvDesc.Texture2DArray.MipSlice = mipSlice;
			rtvDesc.Texture2DArray.ArraySize = desc.ArraySize;
			rtvDesc.Texture2DArray.FirstArraySlice = 0;
		}

		ComPtr<ID3D11Device> device = Renderer::Get()->GetDevice();

		if (FAILED(device->CreateRenderTargetView(this->_texture2D.Get(), &rtvDesc, this->_renderTargetView.GetAddressOf())))
		{
			throw std::runtime_error("Failed to create texture RTV");
		}
	}
}