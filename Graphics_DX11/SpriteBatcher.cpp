#include "pch.h"
#include "SpriteBatcher.h"

namespace GraphicsEngine
{
	SpriteBatcher::SpriteBatcher()
	{

	}

	SpriteBatcher::~SpriteBatcher()
	{
	}

	void SpriteBatcher::Initialize(ComPtr<ID3D11DeviceContext> deviceContext)
	{
		_spriteBatch = std::make_shared<::SpriteBatch>(deviceContext.Get());
	}

	void SpriteBatcher::Release()
	{
		_spriteBatch.reset();
	}

	void SpriteBatcher::Begin(SpriteSortMode sorteMode /*= SpriteSortMode_Deferred*/, _In_opt_ ID3D11BlendState* blendState /*= nullptr*/, _In_opt_ ID3D11SamplerState* samplerState /*= nullptr*/, _In_opt_ ID3D11DepthStencilState* depthStencilState /*= nullptr*/, _In_opt_ ID3D11RasterizerState* rasterizerState /*= nullptr*/, _In_opt_ std::function<void __cdecl()> setCustomShaders /*= nullptr*/, Matrix transformMatrix /*= Matrix::Identity*/)
	{
		_spriteBatch->Begin(sorteMode, blendState, samplerState, depthStencilState,
			rasterizerState, setCustomShaders, transformMatrix);
	}

	void SpriteBatcher::End()
	{
		_spriteBatch->End();
	}
}