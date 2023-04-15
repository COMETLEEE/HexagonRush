#include "pch.h"
#include "ShaderManager.h"

#include "VertexShader.h"
#include "PixelShader.h"
#include "GeometryShader.h"
#include "ComputeShader.h"

// TEST CODE
#include "Graphics_DX11.h"
#include "Graphics_RenderingData_3D.h"
#include "CB_Structure.h"

std::shared_ptr<GraphicsEngine::ShaderManager> GraphicsEngine::ShaderManager::_shaderManager = nullptr;

namespace GraphicsEngine
{
	std::shared_ptr<GraphicsEngine::ShaderManager> ShaderManager::Get()
	{
		if (_shaderManager == nullptr)
			_shaderManager = std::make_shared<GraphicsEngine::ShaderManager>();

		return _shaderManager;
	}

	void ShaderManager::Initialize()
	{
#pragma region Legacy Lighting Model
		//// -------------------------------------------------
		//// -------------   Basic Color Shader  -------------
		//// ------------------------------------------------- 
		//std::shared_ptr<VertexShader> basicVertex = std::make_shared<VertexShader>();
		//std::shared_ptr<PixelShader> basicPixel = std::make_shared<PixelShader>();

		//basicVertex->Initialize(TEXT("Resource\\Shader\\Model.hlsli"), TEXT("VS_ColorOnly"), TEXT("VS_Main"), TEXT("vs_5_0"));
		//basicPixel->Initialize(TEXT("Resource\\Shader\\Model.hlsli"), TEXT("PS_ColorOnly"), TEXT("PS_Main"), TEXT("ps_5_0"));

		//basicVertex->CreateConstantBuffer<CB_PEROBJECT>(CBV_REGISTER::b1);
		//basicVertex->UpdateSamplerState(Renderer::Get()->GetSamplerState()->GetWrapSamplerState().Get(), SAM_REGISTER::s0);

		//basicPixel->CreateShaderResource(SRV_REGISTER::t0);
		//basicPixel->CreateSamplerState(SAM_REGISTER::s0);
		//basicPixel->UpdateSamplerState(Renderer::Get()->GetSamplerState()->GetWrapSamplerState().Get(), SAM_REGISTER::s0);

		//_vertexShaders.insert({ basicVertex->GetName(), basicVertex });
		//_pixelShaders.insert({ basicPixel->GetName(), basicPixel });

		//// -----------------------------------------------------------
		//// ------------- Use Diffuse Texture Static Shader -------------
		//// -----------------------------------------------------------
		//D3D_SHADER_MACRO diffuseMacro[2] = { "USE_DIFFUSE", NULL };

		//std::shared_ptr<VertexShader> diffuseVertex = std::make_shared<VertexShader>();
		//std::shared_ptr<PixelShader> diffusePixel = std::make_shared<PixelShader>();

		//diffuseVertex->Initialize(TEXT("Resource\\Shader\\Model.hlsli"), TEXT("VS_Diffuse"), TEXT("VS_Main"), TEXT("vs_5_0"), diffuseMacro);
		//diffusePixel->Initialize(TEXT("Resource\\Shader\\Model.hlsli"), TEXT("PS_Diffuse"), TEXT("PS_Main"), TEXT("ps_5_0"), diffuseMacro);

		//diffuseVertex->CreateConstantBuffer<CB_PEROBJECT>(CBV_REGISTER::b1);
		//diffuseVertex->UpdateSamplerState(Renderer::Get()->GetSamplerState()->GetWrapSamplerState().Get(), SAM_REGISTER::s0);

		//diffusePixel->CreateShaderResource(SRV_REGISTER::t0);
		//diffusePixel->CreateSamplerState(SAM_REGISTER::s0);
		//diffusePixel->UpdateSamplerState(Renderer::Get()->GetSamplerState()->GetWrapSamplerState().Get(), SAM_REGISTER::s0);

		//_vertexShaders.insert({ diffuseVertex->GetName(), diffuseVertex });
		//_pixelShaders.insert({ diffusePixel->GetName(), diffusePixel });

		//// --------------------------------------------------------------------------
		//// ------------- Use Diffuse Texture + Normal Map Static Shader -------------
		//// --------------------------------------------------------------------------
		//D3D_SHADER_MACRO diffuseNormalMacro[4] = { "USE_DIFFUSE", NULL, "USE_NORMAL", NULL };

		//std::shared_ptr<VertexShader> normalVertex = std::make_shared<VertexShader>();
		//std::shared_ptr<PixelShader> normalPixel = std::make_shared<PixelShader>();

		//normalVertex->Initialize(TEXT("Resource\\Shader\\Model.hlsli"), TEXT("VS_DiffuseNormal"), TEXT("VS_Main"), TEXT("vs_5_0"), diffuseNormalMacro);
		//normalPixel->Initialize(TEXT("Resource\\Shader\\Model.hlsli"), TEXT("PS_DiffuseNormal"), TEXT("PS_Main"), TEXT("ps_5_0"), diffuseNormalMacro);

		//normalVertex->CreateConstantBuffer<CB_PEROBJECT>(CBV_REGISTER::b1);
		//normalVertex->UpdateSamplerState(Renderer::Get()->GetSamplerState()->GetWrapSamplerState().Get(), SAM_REGISTER::s0);

		//normalPixel->CreateShaderResource(SRV_REGISTER::t0);
		//normalPixel->CreateShaderResource(SRV_REGISTER::t1);
		//normalPixel->CreateSamplerState(SAM_REGISTER::s0);
		//normalPixel->UpdateSamplerState(Renderer::Get()->GetSamplerState()->GetWrapSamplerState().Get(), SAM_REGISTER::s0);

		//_vertexShaders.insert({ normalVertex->GetName(), normalVertex });
		//_pixelShaders.insert({ normalPixel->GetName(), normalPixel });

		//// -------------------------------------------------------------------
		//// ------------- Use Diffuse Texture And SKinning Shader -------------
		//// -------------------------------------------------------------------
		//D3D_SHADER_MACRO diffuseSkinningdMacro[4] = { "USE_DIFFUSE", NULL, "USE_SKINNING", NULL };

		//std::shared_ptr<VertexShader> skinningVertex = std::make_shared<VertexShader>();
		//std::shared_ptr<PixelShader> skinningPixel = std::make_shared<PixelShader>();

		//skinningVertex->Initialize(TEXT("Resource\\Shader\\Model.hlsli"), TEXT("VS_DiffuseSkinning"), TEXT("VS_Main"), TEXT("vs_5_0"), diffuseSkinningdMacro);
		//skinningPixel->Initialize(TEXT("Resource\\Shader\\Model.hlsli"), TEXT("PS_DiffuseSkinning"), TEXT("PS_Main"), TEXT("ps_5_0"), diffuseSkinningdMacro);

		//skinningVertex->CreateConstantBuffer<CB_PEROBJECT>(CBV_REGISTER::b1);
		//skinningVertex->UpdateSamplerState(Renderer::Get()->GetSamplerState()->GetWrapSamplerState().Get(), SAM_REGISTER::s0);

		//skinningPixel->CreateShaderResource(SRV_REGISTER::t0);
		//skinningPixel->CreateSamplerState(SAM_REGISTER::s0);
		//skinningPixel->UpdateSamplerState(Renderer::Get()->GetSamplerState()->GetWrapSamplerState().Get(), SAM_REGISTER::s0);

		//_vertexShaders.insert({ skinningVertex->GetName(), skinningVertex });
		//_pixelShaders.insert({ skinningPixel->GetName(), skinningPixel });

		//// --------------------------------------------------------------------------------
		//// ------------- Use Diffuse Texture + Normal Map And SKinning Shader -------------
		//// --------------------------------------------------------------------------------
		//D3D_SHADER_MACRO diffuseNormalSkinningdMacro[6] = { "USE_DIFFUSE", NULL, "USE_SKINNING", NULL, "USE_NORMAL", NULL };

		//std::shared_ptr<VertexShader> normalSkinningVertex = std::make_shared<VertexShader>();
		//std::shared_ptr<PixelShader> normalSkinningPixel = std::make_shared<PixelShader>();

		//normalSkinningVertex->Initialize(TEXT("Resource\\Shader\\Model.hlsli"), TEXT("VS_DiffuseNormalSkinning"), TEXT("VS_Main"), TEXT("vs_5_0"), diffuseNormalSkinningdMacro);
		//normalSkinningPixel->Initialize(TEXT("Resource\\Shader\\Model.hlsli"), TEXT("PS_DiffuseNormalSkinning"), TEXT("PS_Main"), TEXT("ps_5_0"), diffuseNormalSkinningdMacro);

		//normalSkinningVertex->CreateConstantBuffer<CB_PEROBJECT>(CBV_REGISTER::b1);
		//normalSkinningVertex->UpdateSamplerState(Renderer::Get()->GetSamplerState()->GetWrapSamplerState().Get(), SAM_REGISTER::s0);

		//normalSkinningPixel->CreateShaderResource(SRV_REGISTER::t0);
		//normalSkinningPixel->CreateShaderResource(SRV_REGISTER::t1);
		//normalSkinningPixel->CreateSamplerState(SAM_REGISTER::s0);
		//normalSkinningPixel->UpdateSamplerState(Renderer::Get()->GetSamplerState()->GetWrapSamplerState().Get(), SAM_REGISTER::s0);

		//_vertexShaders.insert({ normalSkinningVertex->GetName(), normalSkinningVertex });
		//_pixelShaders.insert({ normalSkinningPixel->GetName(), normalSkinningPixel });
#pragma endregion

#pragma region PBR Lighting Model
	// -------------------------------------------------
	// -------------   Basic Color Shader  -------------
	// ------------------------------------------------- 
	std::shared_ptr<VertexShader> basicVertex = std::make_shared<VertexShader>();
	std::shared_ptr<PixelShader> basicPixel = std::make_shared<PixelShader>();

	basicVertex->Initialize(TEXT("Resource\\Shader\\Model_PBR.hlsli"), TEXT("VS_ColorOnly"), TEXT("VS_Main"), TEXT("vs_5_0"));
	basicPixel->Initialize(TEXT("Resource\\Shader\\Model_PBR.hlsli"), TEXT("PS_ColorOnly"), TEXT("PS_Main"), TEXT("ps_5_0"));

	basicVertex->CreateConstantBuffer<CB_PEROBJECT>(CBV_REGISTER::b1);
	basicVertex->UpdateSamplerState(Renderer::Get()->GetSamplerState()->GetWrapSamplerState().Get(), SAM_REGISTER::s0);

	_vertexShaders.insert({ basicVertex->GetName(), basicVertex });
	_pixelShaders.insert({ basicPixel->GetName(), basicPixel });

	// -----------------------------------------------------------
	// ------------- Use Albedo Texture Static Shader -------------
	// -----------------------------------------------------------
	D3D_SHADER_MACRO aMacro[2] = { "USE_ALBEDO", NULL };

	std::shared_ptr<VertexShader> diffuseVertex = std::make_shared<VertexShader>();
	std::shared_ptr<PixelShader> diffusePixel = std::make_shared<PixelShader>();

	diffuseVertex->Initialize(TEXT("Resource\\Shader\\Model_PBR.hlsli"), TEXT("VS_A"), TEXT("VS_Main"), TEXT("vs_5_0"), aMacro);
	diffusePixel->Initialize(TEXT("Resource\\Shader\\Model_PBR.hlsli"), TEXT("PS_A"), TEXT("PS_Main"), TEXT("ps_5_0"), aMacro);

	diffuseVertex->CreateConstantBuffer<CB_PEROBJECT>(CBV_REGISTER::b1);
	diffuseVertex->UpdateSamplerState(Renderer::Get()->GetSamplerState()->GetWrapSamplerState().Get(), SAM_REGISTER::s0);

	diffusePixel->CreateShaderResource(SRV_REGISTER::t0);
	diffusePixel->CreateSamplerState(SAM_REGISTER::s0);
	diffusePixel->UpdateSamplerState(Renderer::Get()->GetSamplerState()->GetWrapSamplerState().Get(), SAM_REGISTER::s0);

	_vertexShaders.insert({ diffuseVertex->GetName(), diffuseVertex });
	_pixelShaders.insert({ diffusePixel->GetName(), diffusePixel });

	// -----------------------------------------------------------------------------------
	// ------------- Use Albedo Texture + Metallic + Roughness Static Shader -------------
	// -----------------------------------------------------------------------------------
	D3D_SHADER_MACRO amrMacro[6] = { "USE_ALBEDO", NULL, "USE_METALLIC", NULL, "USE_ROUGHNESS", NULL };

	std::shared_ptr<VertexShader> amrVertex = std::make_shared<VertexShader>();
	std::shared_ptr<PixelShader> amrPixel = std::make_shared<PixelShader>();

	amrVertex->Initialize(TEXT("Resource\\Shader\\Model_PBR.hlsli"), TEXT("VS_AMR"), TEXT("VS_Main"), TEXT("vs_5_0"), amrMacro);
	amrPixel->Initialize(TEXT("Resource\\Shader\\Model_PBR.hlsli"), TEXT("PS_AMR"), TEXT("PS_Main"), TEXT("ps_5_0"), amrMacro);

	amrVertex->CreateConstantBuffer<CB_PEROBJECT>(CBV_REGISTER::b1);
	amrVertex->UpdateSamplerState(Renderer::Get()->GetSamplerState()->GetWrapSamplerState().Get(), SAM_REGISTER::s0);

	amrPixel->CreateShaderResource(SRV_REGISTER::t0);
	amrPixel->CreateShaderResource(SRV_REGISTER::t2);
	amrPixel->CreateSamplerState(SAM_REGISTER::s0);
	amrPixel->UpdateSamplerState(Renderer::Get()->GetSamplerState()->GetWrapSamplerState().Get(), SAM_REGISTER::s0);

	_vertexShaders.insert({ amrVertex->GetName(), amrVertex });
	_pixelShaders.insert({ amrPixel->GetName(), amrPixel });

	// --------------------------------------------------------------------------
	// ------------- Use Albedo Texture + Normal Map Static Shader -------------
	// --------------------------------------------------------------------------
	D3D_SHADER_MACRO anMacro[4] = { "USE_ALBEDO", NULL, "USE_NORMAL", NULL };

	std::shared_ptr<VertexShader> normalVertex = std::make_shared<VertexShader>();
	std::shared_ptr<PixelShader> normalPixel = std::make_shared<PixelShader>();

	normalVertex->Initialize(TEXT("Resource\\Shader\\Model_PBR.hlsli"), TEXT("VS_AN"), TEXT("VS_Main"), TEXT("vs_5_0"), anMacro);
	normalPixel->Initialize(TEXT("Resource\\Shader\\Model_PBR.hlsli"), TEXT("PS_AN"), TEXT("PS_Main"), TEXT("ps_5_0"), anMacro);

	normalVertex->CreateConstantBuffer<CB_PEROBJECT>(CBV_REGISTER::b1);
	normalVertex->UpdateSamplerState(Renderer::Get()->GetSamplerState()->GetWrapSamplerState().Get(), SAM_REGISTER::s0);

	normalPixel->CreateShaderResource(SRV_REGISTER::t0);
	normalPixel->CreateShaderResource(SRV_REGISTER::t1);
	normalPixel->CreateSamplerState(SAM_REGISTER::s0);
	normalPixel->UpdateSamplerState(Renderer::Get()->GetSamplerState()->GetWrapSamplerState().Get(), SAM_REGISTER::s0);

	_vertexShaders.insert({ normalVertex->GetName(), normalVertex });
	_pixelShaders.insert({ normalPixel->GetName(), normalPixel });

	// --------------------------------------------------------------------------
	// ------------- Use Albedo Texture + Normal Map + Roughness Static Shader -------------
	// --------------------------------------------------------------------------
	D3D_SHADER_MACRO anrMacro[6] = { "USE_ALBEDO", NULL, "USE_NORMAL", NULL, "USE_ROUGHNESS", NULL};

	std::shared_ptr<VertexShader> anrVertex = std::make_shared<VertexShader>();
	std::shared_ptr<PixelShader> anrPixel = std::make_shared<PixelShader>();

	anrVertex->Initialize(TEXT("Resource\\Shader\\Model_PBR.hlsli"), TEXT("VS_ANR"), TEXT("VS_Main"), TEXT("vs_5_0"), anrMacro);
	anrPixel->Initialize(TEXT("Resource\\Shader\\Model_PBR.hlsli"), TEXT("PS_ANR"), TEXT("PS_Main"), TEXT("ps_5_0"), anrMacro);

	anrVertex->CreateConstantBuffer<CB_PEROBJECT>(CBV_REGISTER::b1);
	anrVertex->UpdateSamplerState(Renderer::Get()->GetSamplerState()->GetWrapSamplerState().Get(), SAM_REGISTER::s0);

	anrPixel->CreateShaderResource(SRV_REGISTER::t0);
	anrPixel->CreateShaderResource(SRV_REGISTER::t1);
	anrPixel->CreateShaderResource(SRV_REGISTER::t2);
	anrPixel->CreateSamplerState(SAM_REGISTER::s0);
	anrPixel->UpdateSamplerState(Renderer::Get()->GetSamplerState()->GetWrapSamplerState().Get(), SAM_REGISTER::s0);

	_vertexShaders.insert({ anrVertex->GetName(), anrVertex });
	_pixelShaders.insert({ anrPixel->GetName(), anrPixel });

	// --------------------------------------------------------------------------
	// ------------- Use Albedo + Normal + Metalic + Roughness Static Shader ----
	// --------------------------------------------------------------------------
	D3D_SHADER_MACRO anmrMacro[8] = { "USE_ALBEDO", NULL, "USE_NORMAL", NULL, "USE_METALLIC", NULL, "USE_ROUGHNESS", NULL };

	std::shared_ptr<VertexShader> anmrVertex = std::make_shared<VertexShader>();
	std::shared_ptr<PixelShader> anmrPixel = std::make_shared<PixelShader>();

	anmrVertex->Initialize(TEXT("Resource\\Shader\\Model_PBR.hlsli"), TEXT("VS_ANMR"), TEXT("VS_Main"), TEXT("vs_5_0"), anmrMacro);
	anmrPixel->Initialize(TEXT("Resource\\Shader\\Model_PBR.hlsli"), TEXT("PS_ANMR"), TEXT("PS_Main"), TEXT("ps_5_0"), anmrMacro);

	anmrVertex->CreateConstantBuffer<CB_PEROBJECT>(CBV_REGISTER::b1);
	anmrVertex->UpdateSamplerState(Renderer::Get()->GetSamplerState()->GetWrapSamplerState().Get(), SAM_REGISTER::s0);

	anmrPixel->CreateShaderResource(SRV_REGISTER::t0);
	anmrPixel->CreateShaderResource(SRV_REGISTER::t1);
	anmrPixel->CreateShaderResource(SRV_REGISTER::t2);
	anmrPixel->CreateShaderResource(SRV_REGISTER::t3);
	anmrPixel->CreateSamplerState(SAM_REGISTER::s0);
	anmrPixel->UpdateSamplerState(Renderer::Get()->GetSamplerState()->GetWrapSamplerState().Get(), SAM_REGISTER::s0);

	_vertexShaders.insert({ anmrVertex->GetName(), anmrVertex });
	_pixelShaders.insert({ anmrPixel->GetName(), anmrPixel });

	// ----------------------------------------------------------------------------------------------
	// ------------- Use Albedo + Normal + Metalic + Roughness + Emissive Static Shader -------------
	// ----------------------------------------------------------------------------------------------
	D3D_SHADER_MACRO anmreMacro[10] = { "USE_ALBEDO", NULL, "USE_NORMAL", NULL, "USE_METALLIC", NULL, "USE_ROUGHNESS", NULL, "USE_EMISSIVE", NULL };

	std::shared_ptr<VertexShader> anmreVertex = std::make_shared<VertexShader>();
	std::shared_ptr<PixelShader> anmrePixel = std::make_shared<PixelShader>();

	anmreVertex->Initialize(TEXT("Resource\\Shader\\Model_PBR.hlsli"), TEXT("VS_ANMRE"), TEXT("VS_Main"), TEXT("vs_5_0"), anmreMacro);
	anmrePixel->Initialize(TEXT("Resource\\Shader\\Model_PBR.hlsli"), TEXT("PS_ANMRE"), TEXT("PS_Main"), TEXT("ps_5_0"), anmreMacro);

	anmreVertex->CreateConstantBuffer<CB_PEROBJECT>(CBV_REGISTER::b1);
	anmreVertex->UpdateSamplerState(Renderer::Get()->GetSamplerState()->GetWrapSamplerState().Get(), SAM_REGISTER::s0);

	anmrePixel->CreateShaderResource(SRV_REGISTER::t0);
	anmrePixel->CreateShaderResource(SRV_REGISTER::t1);
	anmrePixel->CreateShaderResource(SRV_REGISTER::t2);
	anmrePixel->CreateShaderResource(SRV_REGISTER::t3);
	anmrePixel->CreateShaderResource(SRV_REGISTER::t4);
	anmrePixel->CreateSamplerState(SAM_REGISTER::s0);
	anmrePixel->UpdateSamplerState(Renderer::Get()->GetSamplerState()->GetWrapSamplerState().Get(), SAM_REGISTER::s0);

	_vertexShaders.insert({ anmreVertex->GetName(), anmreVertex });
	_pixelShaders.insert({ anmrePixel->GetName(), anmrePixel });


	// -----------------------------------------------------------------------------------------------
	// ------------- Use Albedo + Normal + Metalic + Roughness + Rim Light Static Shader -------------
	// -----------------------------------------------------------------------------------------------
	D3D_SHADER_MACRO anmrRMacro[10] = { "USE_ALBEDO", NULL, "USE_NORMAL", NULL, "USE_METALLIC", NULL, "USE_ROUGHNESS", NULL, "USE_RIMLIGHT", NULL};

	std::shared_ptr<VertexShader> anmrRVertex = std::make_shared<VertexShader>();
	std::shared_ptr<PixelShader> anmrRPixel = std::make_shared<PixelShader>();

	anmrRVertex->Initialize(TEXT("Resource\\Shader\\Model_PBR.hlsli"), TEXT("VS_ANMR_Rim"), TEXT("VS_Main"), TEXT("vs_5_0"), anmrRMacro);
	anmrRPixel->Initialize(TEXT("Resource\\Shader\\Model_PBR.hlsli"), TEXT("PS_ANMR_Rim"), TEXT("PS_Main"), TEXT("ps_5_0"), anmrRMacro);

	anmrRVertex->CreateConstantBuffer<CB_PEROBJECT>(CBV_REGISTER::b1);
	anmrRVertex->UpdateSamplerState(Renderer::Get()->GetSamplerState()->GetWrapSamplerState().Get(), SAM_REGISTER::s0);

	anmrRPixel->CreateShaderResource(SRV_REGISTER::t0);
	anmrRPixel->CreateShaderResource(SRV_REGISTER::t1);
	anmrRPixel->CreateShaderResource(SRV_REGISTER::t2);
	anmrRPixel->CreateShaderResource(SRV_REGISTER::t3);
	anmrRPixel->CreateSamplerState(SAM_REGISTER::s0);
	anmrRPixel->UpdateSamplerState(Renderer::Get()->GetSamplerState()->GetWrapSamplerState().Get(), SAM_REGISTER::s0);

	_vertexShaders.insert({ anmrRVertex->GetName(), anmrRVertex });
	_pixelShaders.insert({ anmrRPixel->GetName(), anmrRPixel });


	// ------------------------------------------------------------------
	// ------------- Use Albedo + Normal + AO Static Shader -------------
	// ------------------------------------------------------------------
	D3D_SHADER_MACRO anaoMacro[6] = { "USE_ALBEDO", NULL, "USE_NORMAL", NULL, "USE_AMBIENTOCCLUSION", NULL };

	std::shared_ptr<VertexShader> anaoVertex = std::make_shared<VertexShader>();
	std::shared_ptr<PixelShader> anaoPixel = std::make_shared<PixelShader>();

	anaoVertex->Initialize(TEXT("Resource\\Shader\\Model_PBR.hlsli"), TEXT("VS_ANAO"), TEXT("VS_Main"), TEXT("vs_5_0"), anaoMacro);
	anaoPixel->Initialize(TEXT("Resource\\Shader\\Model_PBR.hlsli"), TEXT("PS_ANAO"), TEXT("PS_Main"), TEXT("ps_5_0"), anaoMacro);

	anaoVertex->CreateConstantBuffer<CB_PEROBJECT>(CBV_REGISTER::b1);
	anaoVertex->UpdateSamplerState(Renderer::Get()->GetSamplerState()->GetWrapSamplerState().Get(), SAM_REGISTER::s0);

	anaoPixel->CreateShaderResource(SRV_REGISTER::t0);
	anaoPixel->CreateShaderResource(SRV_REGISTER::t1);
	anaoPixel->CreateShaderResource(SRV_REGISTER::t2);
	anaoPixel->CreateSamplerState(SAM_REGISTER::s0);
	anaoPixel->UpdateSamplerState(Renderer::Get()->GetSamplerState()->GetWrapSamplerState().Get(), SAM_REGISTER::s0);

	_vertexShaders.insert({ anaoVertex->GetName(), anaoVertex });
	_pixelShaders.insert({ anaoPixel->GetName(), anaoPixel });

	// ----------------------------------------------------------------------------------------
	// ------------- Use Albedo + Normal + Metalic + Roughness + AO Static Shader -------------
	// ----------------------------------------------------------------------------------------
	D3D_SHADER_MACRO anmraoMacro[10] = { "USE_ALBEDO", NULL, "USE_NORMAL", NULL, "USE_METALLIC", NULL, 
		"USE_ROUGHNESS", NULL, "USE_AMBIENTOCCLUSION", NULL };

	std::shared_ptr<VertexShader> anmraoVertex = std::make_shared<VertexShader>();
	std::shared_ptr<PixelShader> anmraoPixel = std::make_shared<PixelShader>();

	anmraoVertex->Initialize(TEXT("Resource\\Shader\\Model_PBR.hlsli"), TEXT("VS_ANMRAO"), TEXT("VS_Main"), TEXT("vs_5_0"), anmraoMacro);
	anmraoPixel->Initialize(TEXT("Resource\\Shader\\Model_PBR.hlsli"), TEXT("PS_ANMRAO"), TEXT("PS_Main"), TEXT("ps_5_0"), anmraoMacro);

	anmraoVertex->CreateConstantBuffer<CB_PEROBJECT>(CBV_REGISTER::b1);
	anmraoVertex->UpdateSamplerState(Renderer::Get()->GetSamplerState()->GetWrapSamplerState().Get(), SAM_REGISTER::s0);

	anmraoPixel->CreateShaderResource(SRV_REGISTER::t0);
	anmraoPixel->CreateShaderResource(SRV_REGISTER::t1);
	anmraoPixel->CreateShaderResource(SRV_REGISTER::t2);
	anmraoPixel->CreateShaderResource(SRV_REGISTER::t3);
	anmraoPixel->CreateShaderResource(SRV_REGISTER::t4);
	anmraoPixel->CreateSamplerState(SAM_REGISTER::s0);
	anmraoPixel->UpdateSamplerState(Renderer::Get()->GetSamplerState()->GetWrapSamplerState().Get(), SAM_REGISTER::s0);

	_vertexShaders.insert({ anmraoVertex->GetName(), anmraoVertex });
	_pixelShaders.insert({ anmraoPixel->GetName(), anmraoPixel });

	// ----------------------------------------------------------------------------------------
	// ------------- Use Albedo + Normal + Metalic + Roughness + AO + Emissive Static Shader -------------
	// ----------------------------------------------------------------------------------------
	D3D_SHADER_MACRO anmraoeMacro[12] = { "USE_ALBEDO", NULL, "USE_NORMAL", NULL, "USE_METALLIC", NULL,
		"USE_ROUGHNESS", NULL, "USE_AMBIENTOCCLUSION", NULL, "USE_EMISSIVE", NULL };

	std::shared_ptr<VertexShader> anmraoeVertex = std::make_shared<VertexShader>();
	std::shared_ptr<PixelShader> anmraoePixel = std::make_shared<PixelShader>();

	anmraoeVertex->Initialize(TEXT("Resource\\Shader\\Model_PBR.hlsli"), TEXT("VS_ANMRAOE"), TEXT("VS_Main"), TEXT("vs_5_0"), anmraoeMacro);
	anmraoePixel->Initialize(TEXT("Resource\\Shader\\Model_PBR.hlsli"), TEXT("PS_ANMRAOE"), TEXT("PS_Main"), TEXT("ps_5_0"), anmraoeMacro);

	anmraoeVertex->CreateConstantBuffer<CB_PEROBJECT>(CBV_REGISTER::b1);
	anmraoeVertex->UpdateSamplerState(Renderer::Get()->GetSamplerState()->GetWrapSamplerState().Get(), SAM_REGISTER::s0);

	anmraoePixel->CreateShaderResource(SRV_REGISTER::t0);
	anmraoePixel->CreateShaderResource(SRV_REGISTER::t1);
	anmraoePixel->CreateShaderResource(SRV_REGISTER::t2);
	anmraoePixel->CreateShaderResource(SRV_REGISTER::t3);
	anmraoePixel->CreateShaderResource(SRV_REGISTER::t4);
	anmraoePixel->CreateShaderResource(SRV_REGISTER::t5);
	anmraoePixel->CreateSamplerState(SAM_REGISTER::s0);
	anmraoePixel->UpdateSamplerState(Renderer::Get()->GetSamplerState()->GetClampSamplerState().Get(), SAM_REGISTER::s0);

	_vertexShaders.insert({ anmraoeVertex->GetName(), anmraoeVertex });
	_pixelShaders.insert({ anmraoePixel->GetName(), anmraoePixel });


	// �ڡڡڡڡڡڡ� SKinned Mesh Object �ڡڡڡڡڡڡ�
	// -----------------------------------------------------------
	// ------------- Use Only Color Skinning Shader --------------
	// -----------------------------------------------------------
	D3D_SHADER_MACRO sMacro[2] = { "USE_SKINNING", NULL };

	std::shared_ptr<VertexShader> sVertex = std::make_shared<VertexShader>();
	std::shared_ptr<PixelShader> sPixel = std::make_shared<PixelShader>();

	sVertex->Initialize(TEXT("Resource\\Shader\\Model_PBR.hlsli"), TEXT("VS_ColorOnly_Skinning"), TEXT("VS_Main"), TEXT("vs_5_0"), sMacro);
	sPixel->Initialize(TEXT("Resource\\Shader\\Model_PBR.hlsli"), TEXT("PS_ColorOnly_Skinning"), TEXT("PS_Main"), TEXT("ps_5_0"), sMacro);

	sVertex->CreateConstantBuffer<CB_PEROBJECT>(CBV_REGISTER::b1);
	sVertex->UpdateSamplerState(Renderer::Get()->GetSamplerState()->GetWrapSamplerState().Get(), SAM_REGISTER::s0);

	_vertexShaders.insert({ sVertex->GetName(), sVertex });
	_pixelShaders.insert({ sPixel->GetName(), sPixel });

	// -----------------------------------------------------------------------
	// ------------- Use Only Color Skinning + Rim Light Shader --------------
	// -----------------------------------------------------------------------
	D3D_SHADER_MACRO srMacro[4] = { "USE_SKINNING", NULL, "USE_RIMLIGHT", NULL };

	std::shared_ptr<VertexShader> srVertex = std::make_shared<VertexShader>();
	std::shared_ptr<PixelShader> srPixel = std::make_shared<PixelShader>();

	srVertex->Initialize(TEXT("Resource\\Shader\\Model_PBR.hlsli"), TEXT("VS_ColorOnly_Skinning_Rim"), TEXT("VS_Main"), TEXT("vs_5_0"), srMacro);
	srPixel->Initialize(TEXT("Resource\\Shader\\Model_PBR.hlsli"), TEXT("PS_ColorOnly_Skinning_Rim"), TEXT("PS_Main"), TEXT("ps_5_0"), srMacro);

	srVertex->CreateConstantBuffer<CB_PEROBJECT>(CBV_REGISTER::b1);
	srVertex->UpdateSamplerState(Renderer::Get()->GetSamplerState()->GetWrapSamplerState().Get(), SAM_REGISTER::s0);

	_vertexShaders.insert({ srVertex->GetName(), srVertex });
	_pixelShaders.insert({ srPixel->GetName(), srPixel });

	// -----------------------------------------------------------
	// ------------- Use Albedo Texture Skinning Shader -------------
	// -----------------------------------------------------------
	D3D_SHADER_MACRO asMacro[4] = { "USE_SKINNING", NULL, "USE_ALBEDO", NULL };

	std::shared_ptr<VertexShader> aSVertex = std::make_shared<VertexShader>();
	std::shared_ptr<PixelShader> aSPixel = std::make_shared<PixelShader>();

	aSVertex->Initialize(TEXT("Resource\\Shader\\Model_PBR.hlsli"), TEXT("VS_A_Skinning"), TEXT("VS_Main"), TEXT("vs_5_0"), asMacro);
	aSPixel->Initialize(TEXT("Resource\\Shader\\Model_PBR.hlsli"), TEXT("PS_A_Skinning"), TEXT("PS_Main"), TEXT("ps_5_0"), asMacro);

	aSVertex->CreateConstantBuffer<CB_PEROBJECT>(CBV_REGISTER::b1);
	aSVertex->UpdateSamplerState(Renderer::Get()->GetSamplerState()->GetWrapSamplerState().Get(), SAM_REGISTER::s0);

	aSPixel->CreateShaderResource(SRV_REGISTER::t0);
	aSPixel->CreateSamplerState(SAM_REGISTER::s0);
	aSPixel->UpdateSamplerState(Renderer::Get()->GetSamplerState()->GetWrapSamplerState().Get(), SAM_REGISTER::s0);

	_vertexShaders.insert({ aSVertex->GetName(), aSVertex });
	_pixelShaders.insert({ aSPixel->GetName(), aSPixel });

	// -----------------------------------------------------------
	// ------------- Use Albedo Texture + Metallic + Roughness Skinning Shader -------------
	// -----------------------------------------------------------
	D3D_SHADER_MACRO amrsMacro[6] = { "USE_SKINNING", NULL, "USE_ALBEDO", NULL, "USE_METALLIC", NULL, "USE_ROUGHNESS", NULL };

	std::shared_ptr<VertexShader> amrSVertex = std::make_shared<VertexShader>();
	std::shared_ptr<PixelShader> amrSPixel = std::make_shared<PixelShader>();

	amrSVertex->Initialize(TEXT("Resource\\Shader\\Model_PBR.hlsli"), TEXT("VS_AMR_Skinning"), TEXT("VS_Main"), 
		TEXT("vs_5_0"), amrsMacro);
	amrSPixel->Initialize(TEXT("Resource\\Shader\\Model_PBR.hlsli"), TEXT("PS_AMR_Skinning"), TEXT("PS_Main"), 
TEXT("ps_5_0"), amrsMacro);

	amrSVertex->CreateConstantBuffer<CB_PEROBJECT>(CBV_REGISTER::b1);
	amrSVertex->UpdateSamplerState(Renderer::Get()->GetSamplerState()->GetWrapSamplerState().Get(), SAM_REGISTER::s0);

	amrSPixel->CreateShaderResource(SRV_REGISTER::t0);
	amrSPixel->CreateShaderResource(SRV_REGISTER::t2);
	amrSPixel->CreateShaderResource(SRV_REGISTER::t3);
	amrSPixel->CreateSamplerState(SAM_REGISTER::s0);
	amrSPixel->UpdateSamplerState(Renderer::Get()->GetSamplerState()->GetWrapSamplerState().Get(), SAM_REGISTER::s0);

	_vertexShaders.insert({ amrSVertex->GetName(), amrSVertex });
	_pixelShaders.insert({ amrSPixel->GetName(), amrSPixel });

	// --------------------------------------------------------------------------
	// ------------- Use Albedo Texture + Normal Map Skinning Shader -------------
	// --------------------------------------------------------------------------
	D3D_SHADER_MACRO ansMacro[6] = { "USE_SKINNING", NULL, "USE_ALBEDO", NULL, "USE_NORMAL", NULL };

	std::shared_ptr<VertexShader> anSVertex = std::make_shared<VertexShader>();
	std::shared_ptr<PixelShader> anSPixel = std::make_shared<PixelShader>();

	anSVertex->Initialize(TEXT("Resource\\Shader\\Model_PBR.hlsli"), TEXT("VS_AN_Skinning"), TEXT("VS_Main"), TEXT("vs_5_0"), ansMacro);
	anSPixel->Initialize(TEXT("Resource\\Shader\\Model_PBR.hlsli"), TEXT("PS_AN_Skinning"), TEXT("PS_Main"), TEXT("ps_5_0"), ansMacro);

	anSVertex->CreateConstantBuffer<CB_PEROBJECT>(CBV_REGISTER::b1);
	anSVertex->UpdateSamplerState(Renderer::Get()->GetSamplerState()->GetWrapSamplerState().Get(), SAM_REGISTER::s0);

	anSPixel->CreateShaderResource(SRV_REGISTER::t0);
	anSPixel->CreateShaderResource(SRV_REGISTER::t1);
	anSPixel->CreateSamplerState(SAM_REGISTER::s0);
	anSPixel->UpdateSamplerState(Renderer::Get()->GetSamplerState()->GetWrapSamplerState().Get(), SAM_REGISTER::s0);

	_vertexShaders.insert({ anSVertex->GetName(), anSVertex });
	_pixelShaders.insert({ anSPixel->GetName(), anSPixel });

	// --------------------------------------------------------------------------
	// ------------- Use Albedo + Normal + Metalic + Roughness Skinning Shader ----
	// --------------------------------------------------------------------------
	D3D_SHADER_MACRO anmrsMacro[10] = { "USE_SKINNING", NULL, "USE_ALBEDO", NULL, "USE_NORMAL", NULL, "USE_METALLIC", NULL, "USE_ROUGHNESS", NULL };

	std::shared_ptr<VertexShader> anmrSVertex = std::make_shared<VertexShader>();
	std::shared_ptr<PixelShader> anmrSPixel = std::make_shared<PixelShader>();

	anmrSVertex->Initialize(TEXT("Resource\\Shader\\Model_PBR.hlsli"), TEXT("VS_ANMR_Skinning"), TEXT("VS_Main"), TEXT("vs_5_0"), anmrsMacro);
	anmrSPixel->Initialize(TEXT("Resource\\Shader\\Model_PBR.hlsli"), TEXT("PS_ANMR_Skinning"), TEXT("PS_Main"), TEXT("ps_5_0"), anmrsMacro);

	anmrSVertex->CreateConstantBuffer<CB_PEROBJECT>(CBV_REGISTER::b1);
	anmrSVertex->UpdateSamplerState(Renderer::Get()->GetSamplerState()->GetWrapSamplerState().Get(), SAM_REGISTER::s0);

	anmrSPixel->CreateShaderResource(SRV_REGISTER::t0);
	anmrSPixel->CreateShaderResource(SRV_REGISTER::t1);
	anmrSPixel->CreateShaderResource(SRV_REGISTER::t2);
	anmrSPixel->CreateShaderResource(SRV_REGISTER::t3);
	anmrSPixel->CreateSamplerState(SAM_REGISTER::s0);
	anmrSPixel->UpdateSamplerState(Renderer::Get()->GetSamplerState()->GetWrapSamplerState().Get(), SAM_REGISTER::s0);

	_vertexShaders.insert({ anmrSVertex->GetName(), anmrSVertex });
	_pixelShaders.insert({ anmrSPixel->GetName(), anmrSPixel });


	// ----------------------------------------------------------------------------------------------------
	// ------------- Use Albedo + Normal + Metalic + Roughness + Rim Lighting Skinning Shader -------------
	// ----------------------------------------------------------------------------------------------------
	D3D_SHADER_MACRO anmrsrMacro[12] = { "USE_SKINNING", NULL, "USE_ALBEDO", NULL, "USE_NORMAL", NULL, "USE_METALLIC", NULL, "USE_ROUGHNESS", NULL, "USE_RIMLIGHT", NULL };

	std::shared_ptr<VertexShader> anmrSRVertex = std::make_shared<VertexShader>();
	std::shared_ptr<PixelShader> anmrSRPixel = std::make_shared<PixelShader>();

	anmrSRVertex->Initialize(TEXT("Resource\\Shader\\Model_PBR.hlsli"), TEXT("VS_ANMR_Skinning_Rim"), TEXT("VS_Main"), TEXT("vs_5_0"), anmrsrMacro);
	anmrSRPixel->Initialize(TEXT("Resource\\Shader\\Model_PBR.hlsli"), TEXT("PS_ANMR_Skinning_Rim"), TEXT("PS_Main"), TEXT("ps_5_0"), anmrsrMacro);

	anmrSRVertex->CreateConstantBuffer<CB_PEROBJECT>(CBV_REGISTER::b1);
	anmrSRVertex->UpdateSamplerState(Renderer::Get()->GetSamplerState()->GetWrapSamplerState().Get(), SAM_REGISTER::s0);

	anmrSRPixel->CreateShaderResource(SRV_REGISTER::t0);
	anmrSRPixel->CreateShaderResource(SRV_REGISTER::t1);
	anmrSRPixel->CreateShaderResource(SRV_REGISTER::t2);
	anmrSRPixel->CreateShaderResource(SRV_REGISTER::t3);
	anmrSRPixel->CreateSamplerState(SAM_REGISTER::s0);
	anmrSRPixel->UpdateSamplerState(Renderer::Get()->GetSamplerState()->GetWrapSamplerState().Get(), SAM_REGISTER::s0);

	_vertexShaders.insert({ anmrSRVertex->GetName(), anmrSRVertex });
	_pixelShaders.insert({ anmrSRPixel->GetName(), anmrSRPixel });

	// ------------------------------------------------------------------
	// ------------- Use Albedo + Normal + AO Skinning Shader -------------
	// ------------------------------------------------------------------
	D3D_SHADER_MACRO anaosMacro[8] = { "USE_SKINNING", NULL, "USE_ALBEDO", NULL, "USE_NORMAL", NULL, "USE_AMBIENTOCCLUSION", NULL };

	std::shared_ptr<VertexShader> anaoSVertex = std::make_shared<VertexShader>();
	std::shared_ptr<PixelShader> anaoSPixel = std::make_shared<PixelShader>();

	anaoSVertex->Initialize(TEXT("Resource\\Shader\\Model_PBR.hlsli"), TEXT("VS_ANAO_Skinning"), TEXT("VS_Main"), TEXT("vs_5_0"), anaosMacro);
	anaoSPixel->Initialize(TEXT("Resource\\Shader\\Model_PBR.hlsli"), TEXT("PS_ANAO_Skinning"), TEXT("PS_Main"), TEXT("ps_5_0"), anaosMacro);

	anaoSVertex->CreateConstantBuffer<CB_PEROBJECT>(CBV_REGISTER::b1);
	anaoSVertex->UpdateSamplerState(Renderer::Get()->GetSamplerState()->GetWrapSamplerState().Get(), SAM_REGISTER::s0);

	anaoSPixel->CreateShaderResource(SRV_REGISTER::t0);
	anaoSPixel->CreateShaderResource(SRV_REGISTER::t1);
	anaoSPixel->CreateShaderResource(SRV_REGISTER::t2);
	anaoSPixel->CreateSamplerState(SAM_REGISTER::s0);
	anaoSPixel->UpdateSamplerState(Renderer::Get()->GetSamplerState()->GetWrapSamplerState().Get(), SAM_REGISTER::s0);

	_vertexShaders.insert({ anaoSVertex->GetName(), anaoSVertex });
	_pixelShaders.insert({ anaoSPixel->GetName(), anaoSPixel });

	// ----------------------------------------------------------------------------------------
	// ------------- Use Albedo + Normal + Metalic + Roughness + AO Skinning Shader -------------
	// ----------------------------------------------------------------------------------------
	D3D_SHADER_MACRO anmraosMacro[12] = { "USE_SKINNING", NULL, "USE_ALBEDO", NULL, "USE_NORMAL", NULL, "USE_METALLIC", NULL,
		"USE_ROUGHNESS", NULL, "USE_AMBIENTOCCLUSION", NULL };

	std::shared_ptr<VertexShader> anmraoSVertex = std::make_shared<VertexShader>();
	std::shared_ptr<PixelShader> anmraoSPixel = std::make_shared<PixelShader>();

	anmraoSVertex->Initialize(TEXT("Resource\\Shader\\Model_PBR.hlsli"), TEXT("VS_ANMRAO_Skinning"), TEXT("VS_Main"), TEXT("vs_5_0"), anmraosMacro);
	anmraoSPixel->Initialize(TEXT("Resource\\Shader\\Model_PBR.hlsli"), TEXT("PS_ANMRAO_Skinning"), TEXT("PS_Main"), TEXT("ps_5_0"), anmraosMacro);

	anmraoSVertex->CreateConstantBuffer<CB_PEROBJECT>(CBV_REGISTER::b1);
	anmraoSVertex->UpdateSamplerState(Renderer::Get()->GetSamplerState()->GetWrapSamplerState().Get(), SAM_REGISTER::s0);

	anmraoSPixel->CreateShaderResource(SRV_REGISTER::t0);
	anmraoSPixel->CreateShaderResource(SRV_REGISTER::t1);
	anmraoSPixel->CreateShaderResource(SRV_REGISTER::t2);
	anmraoSPixel->CreateShaderResource(SRV_REGISTER::t3);
	anmraoSPixel->CreateShaderResource(SRV_REGISTER::t4);
	anmraoSPixel->CreateSamplerState(SAM_REGISTER::s0);
	anmraoSPixel->UpdateSamplerState(Renderer::Get()->GetSamplerState()->GetWrapSamplerState().Get(), SAM_REGISTER::s0);

	_vertexShaders.insert({ anmraoSVertex->GetName(), anmraoSVertex });
	_pixelShaders.insert({ anmraoSPixel->GetName(), anmraoSPixel });

	// ----------------------------------------------------------------------------------------
	// ------------- Use Albedo + Normal + Metalic + Roughness + AO + Rim Lighting Skinning Shader -------------
	// ----------------------------------------------------------------------------------------
	D3D_SHADER_MACRO anmraosRMacro[12] = { "USE_SKINNING", NULL, "USE_ALBEDO", NULL, "USE_NORMAL", NULL, "USE_METALLIC", NULL,
		"USE_ROUGHNESS", NULL, "USE_AMBIENTOCCLUSION", NULL, "USE_RIMLIGHT", NULL };

	std::shared_ptr<VertexShader> anmraoSRVertex = std::make_shared<VertexShader>();
	std::shared_ptr<PixelShader> anmraoSPRixel = std::make_shared<PixelShader>();

	anmraoSRVertex->Initialize(TEXT("Resource\\Shader\\Model_PBR.hlsli"), TEXT("VS_ANMRAO_Skinning_Rim"), TEXT("VS_Main"), TEXT("vs_5_0"), anmraosRMacro);
	anmraoSPRixel->Initialize(TEXT("Resource\\Shader\\Model_PBR.hlsli"), TEXT("PS_ANMRAO_Skinning_Rim"), TEXT("PS_Main"), TEXT("ps_5_0"), anmraosRMacro);

	anmraoSRVertex->CreateConstantBuffer<CB_PEROBJECT>(CBV_REGISTER::b1);
	anmraoSRVertex->UpdateSamplerState(Renderer::Get()->GetSamplerState()->GetWrapSamplerState().Get(), SAM_REGISTER::s0);

	anmraoSPRixel->CreateShaderResource(SRV_REGISTER::t0);
	anmraoSPRixel->CreateShaderResource(SRV_REGISTER::t1);
	anmraoSPRixel->CreateShaderResource(SRV_REGISTER::t2);
	anmraoSPRixel->CreateShaderResource(SRV_REGISTER::t3);
	anmraoSPRixel->CreateShaderResource(SRV_REGISTER::t4);
	anmraoSPRixel->CreateSamplerState(SAM_REGISTER::s0);
	anmraoSPRixel->UpdateSamplerState(Renderer::Get()->GetSamplerState()->GetWrapSamplerState().Get(), SAM_REGISTER::s0);

	_vertexShaders.insert({ anmraoSRVertex->GetName(), anmraoSRVertex });
	_pixelShaders.insert({ anmraoSPRixel->GetName(), anmraoSPRixel });

	// ----------------------------------------------------------------------------------------
	// ------------- Use Albedo + Normal + Metalic + Roughness + AO + Emissive + Rim Lighting Skinning Shader -------------
	// ----------------------------------------------------------------------------------------
	D3D_SHADER_MACRO anmraoesRMacro[12] = { "USE_SKINNING", NULL, "USE_ALBEDO", NULL, "USE_NORMAL", NULL, "USE_METALLIC", NULL,
		"USE_ROUGHNESS", NULL, "USE_AMBIENTOCCLUSION", NULL, "USE_EMISSIVE", NULL, "USE_RIMLIGHT", NULL };

	std::shared_ptr<VertexShader> anmraoeSRVertex = std::make_shared<VertexShader>();
	std::shared_ptr<PixelShader> anmraoeSPRixel = std::make_shared<PixelShader>();

	anmraoeSRVertex->Initialize(TEXT("Resource\\Shader\\Model_PBR.hlsli"), TEXT("VS_ANMRAOE_Skinning_Rim"), TEXT("VS_Main"), TEXT("vs_5_0"), anmraoesRMacro);
	anmraoeSPRixel->Initialize(TEXT("Resource\\Shader\\Model_PBR.hlsli"), TEXT("PS_ANMRAOE_Skinning_Rim"), TEXT("PS_Main"), TEXT("ps_5_0"), anmraoesRMacro);

	anmraoeSRVertex->CreateConstantBuffer<CB_PEROBJECT>(CBV_REGISTER::b1);
	anmraoeSRVertex->UpdateSamplerState(Renderer::Get()->GetSamplerState()->GetWrapSamplerState().Get(), SAM_REGISTER::s0);

	anmraoeSPRixel->CreateShaderResource(SRV_REGISTER::t0);
	anmraoeSPRixel->CreateShaderResource(SRV_REGISTER::t1);
	anmraoeSPRixel->CreateShaderResource(SRV_REGISTER::t2);
	anmraoeSPRixel->CreateShaderResource(SRV_REGISTER::t3);
	anmraoeSPRixel->CreateShaderResource(SRV_REGISTER::t4);
	anmraoeSPRixel->CreateShaderResource(SRV_REGISTER::t5);
	anmraoeSPRixel->CreateSamplerState(SAM_REGISTER::s0);
	anmraoeSPRixel->UpdateSamplerState(Renderer::Get()->GetSamplerState()->GetWrapSamplerState().Get(), SAM_REGISTER::s0);

	_vertexShaders.insert({ anmraoeSRVertex->GetName(), anmraoeSRVertex });
	_pixelShaders.insert({ anmraoeSPRixel->GetName(), anmraoeSPRixel });

#pragma endregion

		// ------------------------------------
		// -------- SkyBox Only Shader --------
		// ------------------------------------
		std::shared_ptr<VertexShader> skyBoxVertex = std::make_shared<VertexShader>();
		std::shared_ptr<PixelShader> skyBoxPixel = std::make_shared<PixelShader>();
		std::shared_ptr<PixelShader> skyBoxEquiPixel = std::make_shared<PixelShader>();

		skyBoxVertex->Initialize(TEXT("Resource\\Shader\\VS_SkyBox.hlsl"), TEXT("VS_SkyBox"), TEXT("VS_SkyBox"), TEXT("vs_5_0"));
		skyBoxPixel->Initialize(TEXT("Resource\\Shader\\PS_SkyBox.hlsl"), TEXT("PS_SkyBox"), TEXT("PS_SkyBox"), TEXT("ps_5_0"));
		skyBoxEquiPixel->Initialize(TEXT("Resource\\Shader\\PS_SkyBox_Equirectangular.hlsl"), 
			TEXT("PS_SkyBox_Equirectangular"), TEXT("PS_SkyBox_Equirectangular"), TEXT("ps_5_0"));

		skyBoxVertex->CreateConstantBuffer<CB_PEROBJECT>(CBV_REGISTER::b1);
		
		skyBoxPixel->CreateShaderResource(SRV_REGISTER::t0);
		skyBoxPixel->CreateSamplerState(SAM_REGISTER::s0);
		skyBoxPixel->UpdateSamplerState(Renderer::Get()->GetSamplerState()->GetWrapSamplerState().Get(), SAM_REGISTER::s0);

		skyBoxEquiPixel->CreateShaderResource(SRV_REGISTER::t0);
		skyBoxEquiPixel->CreateSamplerState(SAM_REGISTER::s0);
		skyBoxEquiPixel->UpdateSamplerState(Renderer::Get()->GetSamplerState()->GetWrapSamplerState().Get(), SAM_REGISTER::s0);

		_vertexShaders.insert({ skyBoxVertex->GetName(), skyBoxVertex });
		_pixelShaders.insert({ skyBoxPixel->GetName(), skyBoxPixel });
		_pixelShaders.insert({ skyBoxEquiPixel->GetName(), skyBoxEquiPixel });

		// ----------------------------------
		// ------- Deferred Rendering -------
		// ----------------------------------
		D3D_SHADER_MACRO useIBL[2] = { "USE_IBL", NULL };

		std::shared_ptr<VertexShader> deferredVertex = std::make_shared<VertexShader>();
		std::shared_ptr<PixelShader> deferredPixel = std::make_shared<PixelShader>();

		deferredVertex->Initialize(TEXT("Resource\\Shader\\VS_Quad.hlsl"), TEXT("VS_Quad"), TEXT("VS_Quad"), TEXT("vs_5_0"));
		
		// Legacy Lighting Model
		// deferredPixel->Initialize(TEXT("Resource\\Shader\\PS_Lighting.hlsl"), TEXT("PS_Lighting"), TEXT("PS_Lighting"), TEXT("ps_5_0"));

		// PBR Lighting Model
		deferredPixel->Initialize(TEXT("Resource\\Shader\\PS_Lighting_PBR.hlsl"), TEXT("PS_Lighting_PBR"), TEXT("PS_Lighting_PBR"), TEXT("ps_5_0"), useIBL);

		deferredPixel->CreateConstantBuffer<CB_PERTEXTURE>(CBV_REGISTER::b1);
		deferredPixel->CreateConstantBuffer<CB_LIGHTVIEWPROJ>(CBV_REGISTER::b2);
		deferredPixel->CreateConstantBuffer<CB_DIRLIGHTVIEWPROJ>(CBV_REGISTER::b3);
		deferredPixel->CreateShaderResource(SRV_REGISTER::t0);		// DMRAO
		deferredPixel->CreateShaderResource(SRV_REGISTER::t1);		// Normal
		deferredPixel->CreateShaderResource(SRV_REGISTER::t2);		// Position
		deferredPixel->CreateShaderResource(SRV_REGISTER::t3);		// Albedo
		deferredPixel->CreateShaderResource(SRV_REGISTER::t4);		// Emissive
		deferredPixel->CreateShaderResource(SRV_REGISTER::t5);		// Object ID
		deferredPixel->CreateShaderResource(SRV_REGISTER::t6);		// Irradiance
		deferredPixel->CreateShaderResource(SRV_REGISTER::t7);		// Specular_Pre_Filtered
		deferredPixel->CreateShaderResource(SRV_REGISTER::t8);		// Specular_BRDF_LUT
		deferredPixel->CreateShaderResource(SRV_REGISTER::t9);		// TEST : SSAO Map
		deferredPixel->CreateShaderResource(SRV_REGISTER::t10);		// TEST : Shadow Map
		deferredPixel->CreateShaderResource(SRV_REGISTER::t11);		// TEST : Cascade Shadow Map
		deferredPixel->CreateSamplerState(SAM_REGISTER::s0);
		deferredPixel->UpdateSamplerState(Renderer::Get()->GetSamplerState()->GetClampSamplerState().Get(), SAM_REGISTER::s0);
		deferredPixel->CreateSamplerState(SAM_REGISTER::s1);
		deferredPixel->UpdateSamplerState(Renderer::Get()->GetSamplerState()->GetComparisonSamplerState().Get(), SAM_REGISTER::s1);

		_vertexShaders.insert({ deferredVertex->GetName(), deferredVertex });
		_pixelShaders.insert({ deferredPixel->GetName(), deferredPixel });

		// --------------------------------------
		// ------- Sample Rendering Shader -------
		// --------------------------------------
		std::shared_ptr<PixelShader> samplePixel = std::make_shared<PixelShader>();

		samplePixel->Initialize(TEXT("Resource\\Shader\\PS_Sample.hlsl"), TEXT("PS_Sample"), TEXT("PS_Sample"), TEXT("ps_5_0"));
		
		samplePixel->CreateShaderResource(SRV_REGISTER::t0);
		samplePixel->CreateSamplerState(SAM_REGISTER::s0);
		samplePixel->UpdateSamplerState(Renderer::Get()->GetSamplerState()->GetClampSamplerState().Get(), SAM_REGISTER::s0);
		
		_pixelShaders.insert({ samplePixel->GetName(), samplePixel });

		std::shared_ptr<PixelShader> sampleColorPixel = std::make_shared<PixelShader>();

		sampleColorPixel->Initialize(TEXT("Resource\\Shader\\PS_Sample_Color.hlsl"), TEXT("PS_Sample_Color"), TEXT("PS_Sample_Color"), TEXT("ps_5_0"));
		
		sampleColorPixel->CreateConstantBuffer<CB_COLOR>(CBV_REGISTER::b1);
		sampleColorPixel->CreateShaderResource(SRV_REGISTER::t0);
		sampleColorPixel->CreateSamplerState(SAM_REGISTER::s0);
		sampleColorPixel->UpdateSamplerState(Renderer::Get()->GetSamplerState()->GetClampSamplerState().Get(), SAM_REGISTER::s0);

		_pixelShaders.insert({ sampleColorPixel->GetName(), sampleColorPixel });
		
		// --------------------------------------
		// -------- Gaussian Blur Shader --------
		// --------------------------------------
		std::shared_ptr<PixelShader> gaussianBlurX = std::make_shared<PixelShader>();
		std::shared_ptr<PixelShader> gaussianBlurY = std::make_shared<PixelShader>();

		gaussianBlurX->Initialize(TEXT("Resource\\Shader\\PS_GaussianBlur.hlsl"), TEXT("PS_GaussianBlur_X"), TEXT("PS_GaussianBlur_X"), TEXT("ps_5_0"));
		gaussianBlurY->Initialize(TEXT("Resource\\Shader\\PS_GaussianBlur.hlsl"), TEXT("PS_GaussianBlur_Y"), TEXT("PS_GaussianBlur_Y"), TEXT("ps_5_0"));

		gaussianBlurX->CreateConstantBuffer<CB_PERTEXTURE>(CBV_REGISTER::b1);
		gaussianBlurX->CreateShaderResource(SRV_REGISTER::t0);
		gaussianBlurX->CreateSamplerState(SAM_REGISTER::s0);
		gaussianBlurX->UpdateSamplerState(Renderer::Get()->GetSamplerState()->GetClampSamplerState().Get(), SAM_REGISTER::s0);

		gaussianBlurY->CreateConstantBuffer<CB_PERTEXTURE>(CBV_REGISTER::b1);
		gaussianBlurY->CreateShaderResource(SRV_REGISTER::t0);
		gaussianBlurY->CreateSamplerState(SAM_REGISTER::s0);
		gaussianBlurY->UpdateSamplerState(Renderer::Get()->GetSamplerState()->GetClampSamplerState().Get(), SAM_REGISTER::s0);
		
		_pixelShaders.insert({ gaussianBlurX->GetName(), gaussianBlurX });
		_pixelShaders.insert({ gaussianBlurY->GetName(), gaussianBlurY });

		// ---------------------------------------------
		// --------- Edge Preserve Blur Shader ---------
		// ---------------------------------------------
		std::shared_ptr<PixelShader> edgePreserveBlurX = std::make_shared<PixelShader>();
		std::shared_ptr<PixelShader> edgePreserveBlurY = std::make_shared<PixelShader>();

		edgePreserveBlurX->Initialize(TEXT("Resource\\Shader\\PS_EdgePreserveBlur.hlsl"), TEXT("PS_EdgePreserveBlur_X"), TEXT("PS_EdgePreserveBlur_X"), TEXT("ps_5_0"));
		edgePreserveBlurY->Initialize(TEXT("Resource\\Shader\\PS_EdgePreserveBlur.hlsl"), TEXT("PS_EdgePreserveBlur_Y"), TEXT("PS_EdgePreserveBlur_Y"), TEXT("ps_5_0"));

		edgePreserveBlurX->CreateConstantBuffer<CB_PERTEXTURE>(CBV_REGISTER::b1);
		edgePreserveBlurX->CreateShaderResource(SRV_REGISTER::t0);
		edgePreserveBlurX->CreateShaderResource(SRV_REGISTER::t1);
		edgePreserveBlurX->CreateShaderResource(SRV_REGISTER::t2);
		edgePreserveBlurX->CreateSamplerState(SAM_REGISTER::s0);
		edgePreserveBlurX->UpdateSamplerState(Renderer::Get()->GetSamplerState()->GetClampSamplerState().Get(), SAM_REGISTER::s0);

		edgePreserveBlurY->CreateConstantBuffer<CB_PERTEXTURE>(CBV_REGISTER::b1);
		edgePreserveBlurY->CreateShaderResource(SRV_REGISTER::t0);
		edgePreserveBlurY->CreateShaderResource(SRV_REGISTER::t1);
		edgePreserveBlurY->CreateShaderResource(SRV_REGISTER::t2);
		edgePreserveBlurY->CreateSamplerState(SAM_REGISTER::s0);
		edgePreserveBlurY->UpdateSamplerState(Renderer::Get()->GetSamplerState()->GetClampSamplerState().Get(), SAM_REGISTER::s0);

		_pixelShaders.insert({ edgePreserveBlurX->GetName(), edgePreserveBlurX });
		_pixelShaders.insert({ edgePreserveBlurY->GetName(), edgePreserveBlurY });

		// -------------------------------------
		// --------- Shadow Map Shader ---------
		// -------------------------------------
		D3D_SHADER_MACRO shadowMacro[2] = { "USE_SKINNING", NULL };

		std::shared_ptr<VertexShader> shadowVertex = std::make_shared<VertexShader>();
		std::shared_ptr<VertexShader> shadowVertexSkinned = std::make_shared<VertexShader>();

		shadowVertex->Initialize(TEXT("Resource\\Shader\\VS_Shadow.hlsl"), TEXT("VS_Shadow"), TEXT("VS_Shadow"), TEXT("vs_5_0"));
		shadowVertexSkinned->Initialize(TEXT("Resource\\Shader\\VS_Shadow.hlsl"), TEXT("VS_ShadowSkinning"), TEXT("VS_Shadow"), TEXT("vs_5_0"), shadowMacro);

		shadowVertex->CreateConstantBuffer<CB_PEROBJECT>(CBV_REGISTER::b1);
		shadowVertex->CreateConstantBuffer<CB_LIGHTVIEWPROJ>(CBV_REGISTER::b2);

		shadowVertexSkinned->CreateConstantBuffer<CB_PEROBJECT>(CBV_REGISTER::b1);
		shadowVertexSkinned->CreateConstantBuffer<CB_LIGHTVIEWPROJ>(CBV_REGISTER::b2);

		_vertexShaders.insert({ shadowVertex->GetName(), shadowVertex });
		_vertexShaders.insert({ shadowVertexSkinned->GetName(), shadowVertexSkinned });

		// -------------------------------
		// --------- SSAO Shader ---------
		// -------------------------------
		std::shared_ptr<VertexShader> ssaoVertex = std::make_shared<VertexShader>();
		std::shared_ptr<PixelShader> ssaoPixel = std::make_shared<PixelShader>();

		ssaoVertex->Initialize(TEXT("Resource\\Shader\\VS_SSAO.hlsl"), TEXT("VS_SSAO"), TEXT("VS_SSAO"), TEXT("vs_5_0"));
		ssaoPixel->Initialize(TEXT("Resource\\Shader\\PS_SSAO.hlsl"), TEXT("PS_SSAO"), TEXT("PS_SSAO"), TEXT("ps_5_0"));

		ssaoPixel->CreateShaderResource(SRV_REGISTER::t0);
		ssaoPixel->CreateShaderResource(SRV_REGISTER::t1);
		ssaoPixel->CreateShaderResource(SRV_REGISTER::t2);
		ssaoPixel->CreateSamplerState(SAM_REGISTER::s0);
		ssaoPixel->CreateSamplerState(SAM_REGISTER::s1);
		ssaoPixel->UpdateSamplerState(Renderer::Get()->GetSamplerState()->GetClampSamplerState().Get(), SAM_REGISTER::s0);
		ssaoPixel->UpdateSamplerState(Renderer::Get()->GetSamplerState()->GetWrapSamplerState().Get(), SAM_REGISTER::s1);

		_vertexShaders.insert({ ssaoVertex->GetName(), ssaoVertex });
		_pixelShaders.insert({ ssaoPixel->GetName(), ssaoPixel });

		// ---------------------------------------------
		// --------- Camera Motion Blur Shader ---------
		// ---------------------------------------------
		std::shared_ptr<PixelShader> cameraMotionBlurPixel = std::make_shared<PixelShader>();

		cameraMotionBlurPixel->Initialize(TEXT("Resource\\Shader\\PS_CameraMotionBlur.hlsl"), TEXT("PS_CameraMotionBlur"), 
			TEXT("PS_CameraMotionBlur"), TEXT("ps_5_0"));

		cameraMotionBlurPixel->CreateShaderResource(SRV_REGISTER::t0);
		cameraMotionBlurPixel->CreateShaderResource(SRV_REGISTER::t1);
		cameraMotionBlurPixel->CreateSamplerState(SAM_REGISTER::s0);
		cameraMotionBlurPixel->UpdateSamplerState(Renderer::Get()->GetSamplerState()->GetClampSamplerState().Get(), SAM_REGISTER::s0);

		_pixelShaders.insert({ cameraMotionBlurPixel->GetName(), cameraMotionBlurPixel });

		// ---------------------------------------------
		// ---------- HDR Tone Mapping Shader ----------
		// ---------------------------------------------
		D3D_SHADER_MACRO reinhardMacro[2] = { "USE_REINHARD", NULL };

		std::shared_ptr<PixelShader> reinhardPixel = std::make_shared<PixelShader>();

		reinhardPixel->Initialize(TEXT("Resource\\Shader\\PS_ToneMapping.hlsl"), TEXT("PS_ToneMapping_Reinhard"),
			TEXT("PS_ToneMapping"), TEXT("ps_5_0"), reinhardMacro);

		reinhardPixel->CreateConstantBuffer<CB_TONEMAPPING>(CBV_REGISTER::b1);
		reinhardPixel->CreateShaderResource(SRV_REGISTER::t0);
		reinhardPixel->CreateSamplerState(SAM_REGISTER::s0);
		reinhardPixel->UpdateSamplerState(Renderer::Get()->GetSamplerState()->GetClampSamplerState().Get(), SAM_REGISTER::s0);

		_pixelShaders.insert({ reinhardPixel->GetName(), reinhardPixel });

		D3D_SHADER_MACRO filmicMacro[2] = { "USE_FILMIC", NULL };

		std::shared_ptr<PixelShader> filmicPixel = std::make_shared<PixelShader>();

		filmicPixel->Initialize(TEXT("Resource\\Shader\\PS_ToneMapping.hlsl"), TEXT("PS_ToneMapping_Filmic"),
			TEXT("PS_ToneMapping"), TEXT("ps_5_0"), filmicMacro);

		filmicPixel->CreateConstantBuffer<CB_TONEMAPPING>(CBV_REGISTER::b1);
		filmicPixel->CreateShaderResource(SRV_REGISTER::t0);
		filmicPixel->CreateSamplerState(SAM_REGISTER::s0);
		filmicPixel->UpdateSamplerState(Renderer::Get()->GetSamplerState()->GetClampSamplerState().Get(), SAM_REGISTER::s0);

		_pixelShaders.insert({ filmicPixel->GetName(), filmicPixel });

		// --------------------------------------------
		// --------------- Bloom Shader ---------------
		// --------------------------------------------
		D3D_SHADER_MACRO method1Macro[2] = { "BLOOMCURVE_METHOD_1", NULL };

		std::shared_ptr<PixelShader> method1SamplePixel = std::make_shared<PixelShader>();

		method1SamplePixel->Initialize(TEXT("Resource\\Shader\\PS_BloomSampling.hlsl"), TEXT("PS_BloomSampling_1"),
			TEXT("PS_BloomSampling"), TEXT("ps_5_0"), method1Macro);

		method1SamplePixel->CreateShaderResource(SRV_REGISTER::t0);
		method1SamplePixel->CreateSamplerState(SAM_REGISTER::s0);
		method1SamplePixel->UpdateSamplerState(Renderer::Get()->GetSamplerState()->GetClampSamplerState().Get(), SAM_REGISTER::s0);

		_pixelShaders.insert({ method1SamplePixel->GetName(), method1SamplePixel });

		D3D_SHADER_MACRO method2Macro[2] = { "BLOOMCURVE_METHOD_2", NULL };

		std::shared_ptr<PixelShader> method2SamplePixel = std::make_shared<PixelShader>();

		method2SamplePixel->Initialize(TEXT("Resource\\Shader\\PS_BloomSampling.hlsl"), TEXT("PS_BloomSampling_2"),
			TEXT("PS_BloomSampling"), TEXT("ps_5_0"), method2Macro);

		method2SamplePixel->CreateShaderResource(SRV_REGISTER::t0);
		method2SamplePixel->CreateSamplerState(SAM_REGISTER::s0);
		method2SamplePixel->UpdateSamplerState(Renderer::Get()->GetSamplerState()->GetClampSamplerState().Get(), SAM_REGISTER::s0);

		_pixelShaders.insert({ method2SamplePixel->GetName(), method2SamplePixel });

		D3D_SHADER_MACRO method3Macro[2] = { "BLOOMCURVE_METHOD_3", NULL };

		std::shared_ptr<PixelShader> method3SamplePixel = std::make_shared<PixelShader>();

		method3SamplePixel->Initialize(TEXT("Resource\\Shader\\PS_BloomSampling.hlsl"), TEXT("PS_BloomSampling_3"),
			TEXT("PS_BloomSampling"), TEXT("ps_5_0"), method3Macro);

		method3SamplePixel->CreateShaderResource(SRV_REGISTER::t0);
		method3SamplePixel->CreateSamplerState(SAM_REGISTER::s0);
		method3SamplePixel->UpdateSamplerState(Renderer::Get()->GetSamplerState()->GetClampSamplerState().Get(), SAM_REGISTER::s0);

		_pixelShaders.insert({ method3SamplePixel->GetName(), method3SamplePixel });

		std::shared_ptr<PixelShader> addUpPixel = std::make_shared<PixelShader>();

		addUpPixel->Initialize(TEXT("Resource\\Shader\\PS_BloomAddUp.hlsl"), TEXT("PS_BloomAddUp"),
			TEXT("PS_BloomAddUp"), TEXT("ps_5_0"));

		addUpPixel->CreateShaderResource(SRV_REGISTER::t0);
		addUpPixel->CreateShaderResource(SRV_REGISTER::t1);
		addUpPixel->CreateSamplerState(SAM_REGISTER::s0);
		addUpPixel->UpdateSamplerState(Renderer::Get()->GetSamplerState()->GetClampSamplerState().Get(), SAM_REGISTER::s0);

		_pixelShaders.insert({ addUpPixel->GetName(), addUpPixel });

		// -------------------------------------------------------------
		// --------------- Cascade Shadow Mapping Shader ---------------
		// -------------------------------------------------------------
		D3D_SHADER_MACRO cascadeShadowMacro[2] = { "USE_SKINNING", NULL };

		std::shared_ptr<VertexShader> cascadeShadowVertex = std::make_shared<VertexShader>();
		std::shared_ptr<VertexShader> cascadeShadowVertexSkinned = std::make_shared<VertexShader>();

		cascadeShadowVertex->Initialize(TEXT("Resource\\Shader\\VS_CascadeShadow.hlsl"), TEXT("VS_CascadeShadow"), TEXT("VS_CascadeShadow"), TEXT("vs_5_0"));
		cascadeShadowVertexSkinned->Initialize(TEXT("Resource\\Shader\\VS_CascadeShadow.hlsl"), TEXT("VS_CascadeShadowSkinning"), TEXT("VS_CascadeShadow"), TEXT("vs_5_0"), cascadeShadowMacro);

		cascadeShadowVertex->CreateConstantBuffer<CB_PEROBJECT>(CBV_REGISTER::b1);
		cascadeShadowVertexSkinned->CreateConstantBuffer<CB_PEROBJECT>(CBV_REGISTER::b1);

		_vertexShaders.insert({ cascadeShadowVertex->GetName(), cascadeShadowVertex });
		_vertexShaders.insert({ cascadeShadowVertexSkinned->GetName(), cascadeShadowVertexSkinned });


		D3D_SHADER_MACRO cascadeGeoMacro[2] = { "USE_TRIANGLE", NULL };
		std::shared_ptr<GeometryShader> cascadeShadowGeometry = std::make_shared<GeometryShader>();
		cascadeShadowGeometry->Initialize(TEXT("Resource\\Shader\\GS_CascadeShadow.hlsl"), TEXT("GS_CascadeShadow"), TEXT("GS_CascadeShadow"), 
			TEXT("gs_5_0"), cascadeGeoMacro);

		cascadeShadowGeometry->CreateConstantBuffer<CB_DIRLIGHTVIEWPROJ>(CBV_REGISTER::b1);

		_geometryShaders.insert({ cascadeShadowGeometry->GetName(), cascadeShadowGeometry });

		D3D_SHADER_MACRO cascadeGeoMacro2[2] = { "USE_LINE", NULL };

		std::shared_ptr<GeometryShader> cascadeShadowGeometry2 = std::make_shared<GeometryShader>();
		cascadeShadowGeometry2->Initialize(TEXT("Resource\\Shader\\GS_CascadeShadow.hlsl"), TEXT("GS_CascadeShadowLine"), TEXT("GS_CascadeShadow"),
			TEXT("gs_5_0"), cascadeGeoMacro2);

		cascadeShadowGeometry2->CreateConstantBuffer<CB_DIRLIGHTVIEWPROJ>(CBV_REGISTER::b1);

		_geometryShaders.insert({ cascadeShadowGeometry2->GetName(), cascadeShadowGeometry2 });

		// -------------------------------------------
		// --------------- FXAA Shader ---------------
		// -------------------------------------------
		std::shared_ptr<PixelShader> fxaaPixel = std::make_shared<PixelShader>();
		fxaaPixel->Initialize(TEXT("Resource\\Shader\\PS_FXAA_NVIDIA.hlsl"), TEXT("PS_FXAA"), TEXT("PS_FXAA"),
			TEXT("ps_5_0"));

		fxaaPixel->CreateShaderResource(SRV_REGISTER::t0);
		fxaaPixel->CreateConstantBuffer<CB_PERTEXTURE>(CBV_REGISTER::b1);
		fxaaPixel->CreateSamplerState(SAM_REGISTER::s0);
		fxaaPixel->UpdateSamplerState(Renderer::Get()->GetSamplerState()->GetClampSamplerState(), SAM_REGISTER::s0);
		
		_pixelShaders.insert({ TEXT("PS_FXAA"), fxaaPixel });

		// ------------------------------------------------------
		// --------------- Equirec To Cube Shader ---------------
		// ------------------------------------------------------
		std::shared_ptr<ComputeShader> equiToCubeCom = std::make_shared<ComputeShader>();

		equiToCubeCom->Initialize(TEXT("Resource\\Shader\\CS_EquirectToCube.hlsl"), TEXT("CS_EquirectToCube"), TEXT("CS_EquirectToCube"),
			TEXT("cs_5_0"));

		equiToCubeCom->CreateShaderResource(SRV_REGISTER::t0);
		equiToCubeCom->CreateSamplerState(SAM_REGISTER::s0);
		equiToCubeCom->UpdateSamplerState(Renderer::Get()->GetSamplerState()->GetClampSamplerState(), SAM_REGISTER::s0);

		_computeShaders.insert({ equiToCubeCom->GetName(), equiToCubeCom });

		// -----------------------------------------------------
		// --------------- IBL Irradiance Shader ---------------
		// -----------------------------------------------------
		std::shared_ptr<ComputeShader> irradianceCom = std::make_shared<ComputeShader>();

		irradianceCom->Initialize(TEXT("Resource\\Shader\\CS_IBL_Irradiance.hlsl"), TEXT("CS_IBL_Irradiance"), TEXT("CS_IBL_Irradiance"),
			TEXT("cs_5_0"));

		irradianceCom->CreateShaderResource(SRV_REGISTER::t0);
		irradianceCom->CreateSamplerState(SAM_REGISTER::s0);
		irradianceCom->UpdateSamplerState(Renderer::Get()->GetSamplerState()->GetClampSamplerState(), SAM_REGISTER::s0);

		_computeShaders.insert({ irradianceCom->GetName(), irradianceCom });

		// ------------------------------------------------------------
		// --------------- IBL Specular BRDF LUT Shader ---------------
		// ------------------------------------------------------------
		std::shared_ptr<ComputeShader> specularBRDFLUTCom = std::make_shared<ComputeShader>();

		specularBRDFLUTCom->Initialize(TEXT("Resource\\Shader\\CS_IBL_Specular_BRDF_LUT.hlsl"), TEXT("CS_IBL_Specular_BRDF_LUT"), TEXT("CS_IBL_Specular_BRDF_LUT"),
			TEXT("cs_5_0"));

		_computeShaders.insert({ specularBRDFLUTCom->GetName(), specularBRDFLUTCom });

		// ------------------------------------------------------------------
		// --------------- IBL Specular Pre - Filtered Shader ---------------
		// ------------------------------------------------------------------
		std::shared_ptr<ComputeShader> specularPreCom = std::make_shared<ComputeShader>();

		specularPreCom->Initialize(TEXT("Resource\\Shader\\CS_IBL_Specular_Pre_Filtered.hlsl"), TEXT("CS_IBL_Specular_Pre_Filtered"),
			TEXT("CS_IBL_Specular_Pre_Filtered"), TEXT("cs_5_0"));

		specularPreCom->CreateConstantBuffer<CB_SPECMAPFILTER>(CBV_REGISTER::b1);
		specularPreCom->CreateShaderResource(SRV_REGISTER::t0);
		specularPreCom->CreateSamplerState(SAM_REGISTER::s0);
		specularPreCom->UpdateSamplerState(Renderer::Get()->GetSamplerState()->GetClampSamplerState(), SAM_REGISTER::s0);

		_computeShaders.insert({ specularPreCom->GetName(), specularPreCom });

		// -----------------------------------------------------
		// --------------- GBuffers Debug Shader ---------------
		// -----------------------------------------------------
		std::shared_ptr<PixelShader> debugPixel = std::make_shared<PixelShader>();

		debugPixel->Initialize(TEXT("Resource\\Shader\\PS_Debug.hlsl"), TEXT("PS_Debug"), TEXT("PS_Debug"), TEXT("ps_5_0"));

		debugPixel->CreateShaderResource(SRV_REGISTER::t0);
		debugPixel->CreateShaderResource(SRV_REGISTER::t1);
		debugPixel->CreateShaderResource(SRV_REGISTER::t2);
		debugPixel->CreateShaderResource(SRV_REGISTER::t3);
		debugPixel->CreateShaderResource(SRV_REGISTER::t4);
		debugPixel->CreateSamplerState(SAM_REGISTER::s0);
		debugPixel->UpdateSamplerState(Renderer::Get()->GetSamplerState()->GetClampSamplerState(), SAM_REGISTER::s0);

		_pixelShaders.insert({ debugPixel->GetName(), debugPixel });

		// -------------------------------------------------------
		// --------------- Particle System Shaders ---------------
		// -------------------------------------------------------

		// --------------------- Vertex Shaders ---------------------
		std::shared_ptr<VertexShader> streamOutVertex = std::make_shared<VertexShader>();
		std::shared_ptr<VertexShader> drawParticleVertex = std::make_shared<VertexShader>();

		streamOutVertex->Initialize(TEXT("Resource\\Shader\\VS_Particle_StreamOut.hlsl"), TEXT("VS_Particle_StreamOut"), 
			TEXT("VS_Particle_StreamOut"), TEXT("vs_5_0"));

		drawParticleVertex->Initialize(TEXT("Resource\\Shader\\VS_Particle_Draw.hlsl"), TEXT("VS_Particle_Draw"),
			TEXT("VS_Particle_Draw"), TEXT("vs_5_0"));

		drawParticleVertex->CreateConstantBuffer<CB_PARTICLE_DRAW>(CBV_REGISTER::b1);

		_vertexShaders.insert({ streamOutVertex->GetName(), streamOutVertex });
		_vertexShaders.insert({ drawParticleVertex->GetName(), drawParticleVertex });

		// --------------------- Pixel Shaders ---------------------
		std::shared_ptr<PixelShader> drawParticlePixel = std::make_shared<PixelShader>();

		drawParticlePixel->Initialize(TEXT("Resource\\Shader\\PS_Particle_Draw.hlsl"), TEXT("PS_Particle_Draw"),
			TEXT("PS_Particle_Draw"), TEXT("ps_5_0"));

		drawParticlePixel->CreateConstantBuffer<CB_PARTICLE_TIMER>(CBV_REGISTER::b1);
		drawParticlePixel->CreateConstantBuffer<CB_PARTICLE_COLOR>(CBV_REGISTER::b2);
		drawParticlePixel->CreateShaderResource(SRV_REGISTER::t0);
		drawParticlePixel->CreateShaderResource(SRV_REGISTER::t1);
		drawParticlePixel->CreateSamplerState(SAM_REGISTER::s0);
		drawParticlePixel->UpdateSamplerState(Renderer::Get()->GetSamplerState()->GetClampSamplerState(), SAM_REGISTER::s0);

		_pixelShaders.insert({ drawParticlePixel->GetName(), drawParticlePixel });

		// --------------------- Geometry Shaders ---------------------
		std::shared_ptr<GeometryShader> streamOutCircleGeo = std::make_shared<GeometryShader>();
		std::shared_ptr<GeometryShader> streamOutSphereGeo = std::make_shared<GeometryShader>();
		std::shared_ptr<GeometryShader> streamOutOnlyDelete = std::make_shared<GeometryShader>();
		std::shared_ptr<GeometryShader> drawParticleGeo = std::make_shared<GeometryShader>();

		D3D_SHADER_MACRO circleMacro[2] = { "USE_CIRCLE", NULL };
		D3D_SHADER_MACRO sphereMacro[2] = { "USE_SPHERE", NULL };

		streamOutCircleGeo->Initialize(TEXT("Resource\\Shader\\GS_Particle_StreamOut.hlsl"), TEXT("GS_Particle_StreamOut_Circle"),
			TEXT("GS_Particle_StreamOut"), TEXT("gs_5_0"), circleMacro);

		streamOutSphereGeo->Initialize(TEXT("Resource\\Shader\\GS_Particle_StreamOut.hlsl"), TEXT("GS_Particle_StreamOut_Sphere"),
			TEXT("GS_Particle_StreamOut"), TEXT("gs_5_0"), sphereMacro);

		streamOutOnlyDelete->Initialize(TEXT("Resource\\Shader\\GS_Particle_StreamOut_OnlyDelete.hlsl"), TEXT("GS_Particle_StreamOut_OnlyDelete"),
			TEXT("GS_Particle_StreamOut_OnlyDelete"), TEXT("gs_5_0"));

		drawParticleGeo->Initialize(TEXT("Resource\\Shader\\GS_Particle_Draw.hlsl"), TEXT("GS_Particle_Draw"),
			TEXT("GS_Particle_Draw"), TEXT("gs_5_0"));

		streamOutCircleGeo->CreateConstantBuffer<CB_PARTICLE_STREAMOUT>(CBV_REGISTER::b1);
		streamOutCircleGeo->CreateConstantBuffer<CB_PARTICLE_TIMER>(CBV_REGISTER::b2);
		streamOutCircleGeo->CreateShaderResource(SRV_REGISTER::t0);
		streamOutCircleGeo->CreateSamplerState(SAM_REGISTER::s0);
		streamOutCircleGeo->UpdateSamplerState(Renderer::Get()->GetSamplerState()->GetClampSamplerState(), SAM_REGISTER::s0);

		streamOutSphereGeo->CreateConstantBuffer<CB_PARTICLE_STREAMOUT>(CBV_REGISTER::b1);
		streamOutSphereGeo->CreateConstantBuffer<CB_PARTICLE_TIMER>(CBV_REGISTER::b2);
		streamOutSphereGeo->CreateShaderResource(SRV_REGISTER::t0);
		streamOutSphereGeo->CreateSamplerState(SAM_REGISTER::s0);
		streamOutSphereGeo->UpdateSamplerState(Renderer::Get()->GetSamplerState()->GetClampSamplerState(), SAM_REGISTER::s0);

		streamOutOnlyDelete->CreateConstantBuffer<CB_PARTICLE_STREAMOUT>(CBV_REGISTER::b1);
		streamOutOnlyDelete->CreateConstantBuffer<CB_PARTICLE_TIMER>(CBV_REGISTER::b2);
		streamOutOnlyDelete->CreateShaderResource(SRV_REGISTER::t0);
		streamOutOnlyDelete->CreateSamplerState(SAM_REGISTER::s0);
		streamOutOnlyDelete->UpdateSamplerState(Renderer::Get()->GetSamplerState()->GetClampSamplerState(), SAM_REGISTER::s0);

		drawParticleGeo->CreateShaderResource(SRV_REGISTER::t0);
		drawParticleGeo->CreateSamplerState(SAM_REGISTER::s0);
		drawParticleGeo->UpdateSamplerState(Renderer::Get()->GetSamplerState()->GetClampSamplerState(), SAM_REGISTER::s0);

		_geometryShaders.insert({ streamOutCircleGeo->GetName(), streamOutCircleGeo });
		_geometryShaders.insert({ streamOutSphereGeo->GetName(), streamOutSphereGeo });
		_geometryShaders.insert({ streamOutOnlyDelete->GetName(), streamOutOnlyDelete });
		_geometryShaders.insert({ drawParticleGeo->GetName(), drawParticleGeo });

		// ---------------------------------------------------------
		// --------------- Vignetting Effect Shaders ---------------
		// ---------------------------------------------------------
		std::shared_ptr<PixelShader> vignettingPixel = std::make_shared<PixelShader>();

		vignettingPixel->Initialize(TEXT("Resource\\Shader\\PS_Vignetting.hlsl"), TEXT("PS_Vignetting"),
			TEXT("PS_Vignetting"), TEXT("ps_5_0"));

		vignettingPixel->CreateShaderResource(SRV_REGISTER::t0);
		vignettingPixel->CreateSamplerState(SAM_REGISTER::s0);
		vignettingPixel->UpdateSamplerState(Renderer::Get()->GetSamplerState()->GetClampSamplerState(), SAM_REGISTER::s0);

		_pixelShaders.insert({ vignettingPixel->GetName(), vignettingPixel });
	}

	void ShaderManager::Release()
	{
		for (auto& vertexShader : _vertexShaders)
			if (vertexShader.second != nullptr)
				vertexShader.second->Release();

		for (auto& pixelShader : _pixelShaders)
			if (pixelShader.second != nullptr)
				pixelShader.second->Release();

		for (auto& geometryShader: _geometryShaders)
			if (geometryShader.second != nullptr)
				geometryShader.second->Release();

		for (auto& computeShader : _computeShaders)
			if (computeShader.second != nullptr)
				computeShader.second->Release();
	}

	std::shared_ptr<VertexShader> ShaderManager::GetVS(const tstring& name)
	{
		return _vertexShaders.find(name) != _vertexShaders.end() ? _vertexShaders[name] : nullptr;
	}

	std::shared_ptr<PixelShader> ShaderManager::GetPS(const tstring& name)
	{
		return _pixelShaders.find(name) != _pixelShaders.end() ? _pixelShaders[name] : nullptr;
	}

	std::shared_ptr<GraphicsEngine::GeometryShader> ShaderManager::GetGS(const tstring& name)
	{
		return _geometryShaders.find(name) != _geometryShaders.end() ? _geometryShaders[name] : nullptr;
	}

	std::shared_ptr<GraphicsEngine::ComputeShader> ShaderManager::GetCS(const tstring& name)
	{
		return _computeShaders.find(name) != _computeShaders.end() ? _computeShaders[name] : nullptr;
	}
}