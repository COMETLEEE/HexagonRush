#include "pch.h"
#include "Device.h"

#include "Graphics_DX11.h"

namespace GraphicsEngine
{
	void Device::Initialize(WindowInfo& info)
	{
		HRESULT hr = S_FALSE;

		UINT createDeviceFlag = 0;

		// Debug가 활성화되어 있으면 디버그용 플래그 추가해주기 ..!
		// 이렇게하면 디버그 모드 빌드에서 디버그 계층을 활성화할 수 있다.
#if defined(DEBUG) || defined(_DEBUG)
		createDeviceFlag |= D3D11_CREATE_DEVICE_DEBUG;
#endif

		D3D_DRIVER_TYPE driverTypes[] =
		{
			D3D_DRIVER_TYPE_HARDWARE,
			D3D_DRIVER_TYPE_WARP,
			D3D_DRIVER_TYPE_REFERENCE
		};

		uint32 numDriverTypes = ARRAYSIZE(driverTypes);

		D3D_FEATURE_LEVEL featureLevels[] =
		{
			D3D_FEATURE_LEVEL_11_1,
			D3D_FEATURE_LEVEL_11_0
		};

		uint32 numFeatureLevels = ARRAYSIZE(featureLevels);

		D3D_DRIVER_TYPE d3dDriverType;
		D3D_FEATURE_LEVEL featureLevel;

		// 1. Direct3D의 디바이스와 디바이스 컨텍스트를 생성한다.
		for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
		{
			d3dDriverType = driverTypes[driverTypeIndex];

			// 다이렉트 11은 ImediateContext와 디바이스를 동시에 만든다.
			hr = D3D11CreateDevice(nullptr, d3dDriverType, nullptr, createDeviceFlag, featureLevels, numFeatureLevels,
				D3D11_SDK_VERSION, _device.GetAddressOf(), &featureLevel, _deviceContext.GetAddressOf());

			// Direct 11.1
			HR(_device.As(&_device1));
			HR(_deviceContext.As(&_deviceContext1));

			if (hr == E_INVALIDARG)
			{
				// Direct3D 11.0 API는 D3D_FEATURE_LEVEL_11_1을 인식하지 못하므로 기능 수준 11.0 이하를 시도해야 합니다.
				hr = D3D11CreateDevice(nullptr, d3dDriverType, nullptr, createDeviceFlag, &featureLevels[1], numFeatureLevels - 1,
					D3D11_SDK_VERSION, _device.GetAddressOf(), &featureLevel, _deviceContext.GetAddressOf());
			}

			if (SUCCEEDED(hr))
				break;
		}

		// 생성에 실패하였다면 메시지 박스 출현
		if (FAILED(hr))
		{
			MessageBox(0, L"D3D11CreateDevice Failed.", 0, 0);

			return;
		}

		// Direct3D 11의 Feature Level이 지원되지 않는다면 메시지 박스 출현
		if ((featureLevel != D3D_FEATURE_LEVEL_11_0) && (featureLevel != D3D_FEATURE_LEVEL_11_1))
		{
			MessageBox(0, L"Direct3D Feature Level 11 Unsupported.", 0, 0);

			return;
		}

		HR(_device->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &info._4xMSAAQuality));

		// 4X MSAA가 항상 지원되므로 반환된 품질 수준의 값은 반드시 0보다 커야 한다. 따라서, 0보다 작거나 같으면 assert !
		assert(info._4xMSAAQuality > 0);

		ComPtr<IDXGIDevice> dxgiDevice = nullptr;

		// Adapter 및 Factory 생성.
		HR(_device.As(&dxgiDevice));

		HR(dxgiDevice->GetAdapter(_dxgiAdapter.GetAddressOf()));

		HR(_dxgiAdapter->GetParent(__uuidof(IDXGIFactory1), reinterpret_cast<void**>(_dxgi.GetAddressOf())));

		// Direct3D 응용 프로그램의 기본 설정 값인 Alt + Enter 전체 화면으로의 전환을 비활성화합니다.
		_dxgi->MakeWindowAssociation(info._hWnd, DXGI_MWA_NO_ALT_ENTER | DXGI_MWA_NO_WINDOW_CHANGES);

		dxgiDevice.ReleaseAndGetAddressOf();
	}

	void Device::Release()
	{
		_dxgi.ReleaseAndGetAddressOf();

		_dxgiAdapter.ReleaseAndGetAddressOf();

		_device.Reset();

		_deviceContext.Reset();

		_device1.ReleaseAndGetAddressOf();

		_deviceContext1.ReleaseAndGetAddressOf();
	}
}