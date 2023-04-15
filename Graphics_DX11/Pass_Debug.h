#pragma once
#include "Pass_Interface.h"

namespace GraphicsEngine
{
	class Mesh;

	class VertexShader;

	class PixelShader;

	class RenderTarget;

	class Pass_Deferred;

	// G Buffer를 화면에 뿌린다. => Final에 있을 예정
	class Pass_Debug : public Pass_Interface
	{
	public:
		void Initialize(const uint32& screenWidth, const uint32& screenHeight) override;

		void OnResize(const uint32& screenWidth, const uint32& screenHeight) override;

		void Release() override;
		
		void Render();

	public:
		std::shared_ptr<Pass_Deferred> _deferredPassRef;

		std::shared_ptr<Mesh> _debugMesh;

		std::shared_ptr<VertexShader> _vsQuad;

		std::shared_ptr<PixelShader> _psDebug;
	};
}