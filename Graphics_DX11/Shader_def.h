#pragma once
#include "Graphics_DX11_typedef.h"

enum class SHADER_TYPE
{
	VERTEX_SHADER,
	GEOMETRY_SHADER,
	PIXEL_SHADER,
	COMPUTE_SHADER
};

enum class CBV_REGISTER : uint32
{
	b0 = 0,			// For Per-Frame Data
	b1 = 1,
	b2 = 2,
	b3 = 3,
	b4 = 4,
	b5 = 5,
	b6 = 6,
	b7 = 7
};

enum class SRV_REGISTER : uint32
{
	t0 = 0,
	t1 = 1,
	t2 = 2,
	t3 = 3,
	t4 = 4,
	t5 = 5,
	t6 = 6,
	t7 = 7,
	t8 = 8,
	t9 = 9,
	t10 = 10,
	t11 = 11,
	t12 = 12,
};

enum class UAV_REGISTER : uint32
{
	u0 = 0,
	u1 = 1,
	u2 = 2,
	u3 = 3,
	u4 = 4,
	u5 = 5,
	u6 = 6,
	u7 = 7,
};

enum class SAM_REGISTER : uint32
{
	s0 = 0,
	s1 = 1,
	s2 = 2,
	s3 = 3,
	s4 = 4,
	s5 = 5,
	s6 = 6,
	s7 = 7,
};

enum
{
	SWAP_CHAIN_BUFFER_COUNT = 2,
};