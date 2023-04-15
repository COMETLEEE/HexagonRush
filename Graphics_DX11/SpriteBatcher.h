#pragma once

namespace GraphicsEngine
{
	class SpriteBatcher final
	{
	public:
		SpriteBatcher();

		~SpriteBatcher();

		void Initialize(ComPtr<ID3D11DeviceContext> deviceContext);

		void Release();

		// Batch하기 위한 Sprite Draw를 시작한다.
		void Begin(SpriteSortMode sorteMode = SpriteSortMode_Deferred,
			_In_opt_ ID3D11BlendState* blendState = nullptr,
			_In_opt_ ID3D11SamplerState* samplerState = nullptr,
			_In_opt_ ID3D11DepthStencilState* depthStencilState = nullptr,
			_In_opt_ ID3D11RasterizerState* rasterizerState = nullptr,
			_In_opt_ std::function<void __cdecl()> setCustomShaders = nullptr,
			Matrix transformMatrix = Matrix::Identity);

		// 종료한다. => 현재 렌더 타겟에 일괄 시행한다.
		void End();

		inline std::shared_ptr<SpriteBatch> GetSpriteBatch() { return _spriteBatch; }

	private:
		std::shared_ptr<SpriteBatch> _spriteBatch;
	};
}