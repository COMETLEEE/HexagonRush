#pragma once

/// <summary>
/// 현재 머슬 클라이언트의 프리팹이란 게임 오브젝트를 하나 들고 있는
/// 
/// 독립적인 객체. 그럼 그 클라이언트에 컨트롤러 등의 스크립트를 넣어주면
/// 
/// 내부의 컴포넌트 등을 통제할 수 있다.
/// </summary>
class Bullet_Prefab : public IPrefab
{
public:
	Bullet_Prefab();

	virtual ~Bullet_Prefab();
};