#pragma once

/// <summary>
/// 구조체 같은 친구..
/// </summary>
/// 
/// 
enum class PlatStatus
{
	Idle,
	Select,
	Delete,
	Respawn,
	Drop,
	PermanentlyDelete
};
class Platform : public Muscle::IComponents
{
public:
	Platform(std::shared_ptr<Muscle::GameObject> gameObj);

	virtual ~Platform();

private:
	StateColor _color;

	PlatStatus _status;

	PlatStatus _preStatus; // 이전 상태.

	std::shared_ptr<Muscle::Collider> _collider; // 캐싱해서 사용..!

	Vector3 _initialPosition; // 초기 위치.

	std::shared_ptr<Platform> _dropPostion; // 타일이 떨어질 위치.

	std::shared_ptr<RenderingData_3D> _renderingData; //캐싱해서 사용..!

	std::shared_ptr<Muscle::ParticleRenderer> _particleRenderer;	// 파티클 렌더러

	std::shared_ptr<RenderingData_Particle> _particleRenderingData;	// 헤응

public:
	const StateColor& GetColor() { return _color; } // 바닥 색상 지정

	void SetColor(StateColor color); // 바닥 색상 지정

	void SetPosition(Vector3 initialPosition); // 바닥 초기 위치 지정

	void DeletePlatform(); // 플랫폼 사용시 파괴.

	void CreatePlatform(); // 플랫폼 파괴 후 재생성.

	void SelectPlatform(); // 플레이어가 플랫폼을 선택 했을 때.

	void SelectCanclePlatform(); // 플레이어가 플랫폼을 선택을 취소 했을 때.

	void PermanentlyDelete(); // 플레이어가 플랫폼을 선택을 취소 했을 때.

	void Drop(std::shared_ptr<Platform> platform); // 타일을 위에서 떨어트린다. //풀에 있는 친구들이 사용할 함수..

	void DropReset(); // 타일을 다시 복귀시킨다. //풀에 있는 친구들이 사용할 함수..

	Vector3 GetInitialPosition() { return _initialPosition; }

	PlatStatus GetState() { return _status; }

public:
	virtual void Start();

	virtual void Update(); // 모든 Object Udpate돌릴 때 (ObjectManager) 오브젝트가 Components를 가지고 있으면 같이 업데이트 해주기.
};