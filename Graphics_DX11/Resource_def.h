#pragma once

enum class RESOURCE_TYPE
{
	// Load한 일반 메쉬
	MESH,

	// Load한 일반 텍스처
	TEXTURE,
	
	// Load한 일반 폰트
	FONT,

	// Load한 일반 Shader
	SHADER,

	// 렌더 타겟으로 사용해 그림을 직접 그려서 사용하는 텍스처 .. 이것이 리얼 타임 렌더링의 정수
	RENDER_TARGET_TEXTURE,
};

enum class TEXTURE_TYPE
{
	DEFAULT,

	ENVIRONMENT,

	USE_MIPMAP
};