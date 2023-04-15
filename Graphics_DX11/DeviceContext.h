#pragma once

class DeviceContext
{
public:
	void Initialize();

	void Release();

	ComPtr<ID3D11DeviceContext> GetDeviceContext() { return _deviceContext; }
	
private:
	ComPtr<ID3D11DeviceContext> _deviceContext;

	ComPtr<ID3D11DeviceContext> _deviceContext1;
};