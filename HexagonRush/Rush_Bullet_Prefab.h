#pragma once

// 러쉬용 탄환 오브젝트
// 생성될 때 그냥 바로 보스의 위치를 향해서 날아간다 !
class Rush_Bullet_Prefab : public IPrefab
{
public:
	Rush_Bullet_Prefab();

	virtual ~Rush_Bullet_Prefab();
};