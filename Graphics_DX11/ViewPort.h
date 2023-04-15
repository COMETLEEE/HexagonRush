#pragma once

namespace GraphicsEngine
{
	// 그냥 뷰포트 래퍼
	class ViewPort
	{
	public:
		void Initialize(const Vector2& leftTop, const uint32& screenWidth, const uint32& screenHeight);

		void OnResize(const uint32& screenWidth, const uint32& screenHeight);

		void OnResize(const Vector2 leftTop, const uint32& screenWidth, const uint32& screenHeight);

		void SetViewPort(ComPtr<ID3D11DeviceContext> deviceContext);

		void Release();

	private:
		Vector2 _leftTop;

		D3D11_VIEWPORT _viewport;
	};
}