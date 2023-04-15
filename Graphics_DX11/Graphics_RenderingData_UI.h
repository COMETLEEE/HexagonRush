#pragma once
#include "Graphics_DX11_typedef.h"

struct DrawInfo
{
	DrawInfo() : _textureID(ULLONG_MAX), _sortOrder(5),
		_color(Vector4(1.f, 1.f, 1.f, 1.f)), _useAlphaBlend(true)
	{
	}

	uint64 _textureID;

	uint32 _sortOrder;

	Vector4 _color;

	bool _useAlphaBlend;
};

// NDC 공간에서의 LeftTop, Right Bottom
// (-1, 1)
struct NDCInfo
{
	Vector2 _leftTop;

	Vector2 _rightBottom;
};

// 화면 해상도 변화에 대해 좌표에 빠르게 대응하기 위해 NDC 공간 사용
struct RenderingData_UI
{
	RenderingData_UI() : _drawInfo(std::make_shared<DrawInfo>()), _ndcInfo(std::make_shared<NDCInfo>())
	{}

	std::shared_ptr<DrawInfo> _drawInfo;

	std::shared_ptr<NDCInfo> _ndcInfo;
};

// 화면 해상도에 맞춰서 앵커에 따라서 부모가 늘어나면 나도 늘어나야한다.

// 파티클 쉐이더 만들어야하니까 빠른 개발 메타로 간다.