#include "pch.h"
#include "Pass_Debug.h"

#include "Renderer.h"
#include "ShaderManager.h"
#include "ResourceManager.h"

#include "Mesh.h"
#include "VertexShader.h"
#include "PixelShader.h"

#include "Pass_Deferred.h"

namespace GraphicsEngine
{
	void Pass_Debug::Initialize(const uint32& screenWidth, const uint32& screenHeight)
	{
		__super::Initialize(screenWidth, screenHeight);
		
		_vsQuad = ShaderManager::Get()->GetVS(TEXT("VS_Quad"));

		_psDebug = ShaderManager::Get()->GetPS(TEXT("PS_Debug"));

		_debugMesh = ResourceManager::Get()->GetMesh(STATIC_DEBUG_MESH);

		_deferredPassRef = Renderer::Get()->GetDeferredPass();
	}

	void Pass_Debug::OnResize(const uint32& screenWidth, const uint32& screenHeight)
	{
		__super::OnResize(screenWidth, screenHeight);
	}

	void Pass_Debug::Release()
	{
		_debugMesh.reset();

		_vsQuad.reset();

		_psDebug.reset();

		__super::Release();
	}

	void Pass_Debug::Render()
	{
		_vsQuad->Update();

		_psDebug->UpdateShaderResource(_deferredPassRef->GetDepth()->GetSRV(), SRV_REGISTER::t0);
		_psDebug->UpdateShaderResource(_deferredPassRef->GetNormal()->GetSRV(), SRV_REGISTER::t1);
		_psDebug->UpdateShaderResource(_deferredPassRef->GetPosition()->GetSRV(), SRV_REGISTER::t2);
		_psDebug->UpdateShaderResource(_deferredPassRef->GetAlbedo()->GetSRV(), SRV_REGISTER::t3);
		_psDebug->UpdateShaderResource(_deferredPassRef->GetEmissive()->GetSRV(), SRV_REGISTER::t4);

		_psDebug->Update();

		_debugMesh->Update();
	}
}