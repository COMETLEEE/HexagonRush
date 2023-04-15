#pragma once

class CannonBall_Prefab : public IPrefab
{
public:
	enum class CANNON_BALL_TYPE
	{
		OIL,
		ELECTRONIC,
		LANDMINE,
		PANICBLUE
	};

public:
	CannonBall_Prefab(CANNON_BALL_TYPE type);

	virtual ~CannonBall_Prefab();
};