#pragma once
/// <summary>
/// 디버그 텍스트 관련된 더러운 일들을 처리하는 친구! 더럽게 짜도록 하자!
/// 컴포넌트를 상속받은 이유는 게임 씬이 시작되고 게임 오브젝트에 접근하기 위해서..!!
/// </summary>


class DebugObjectManager : public Muscle::IComponents, public std::enable_shared_from_this<DebugObjectManager>
{
public:
	DebugObjectManager(std::shared_ptr<Muscle::GameObject> gameObj);

	virtual ~DebugObjectManager();

private:
	static std::shared_ptr<DebugObjectManager> _instance;

	std::shared_ptr<Muscle::TextRenderer> _timeText;

	std::shared_ptr<Muscle::TextRenderer> _cameraInfo;

	std::shared_ptr<Muscle::TextRenderer> _objectInfo;

	std::shared_ptr<Muscle::TextRenderer> _objectCountInfo;

	std::shared_ptr<Muscle::RendererBase> _renderbase;

	std::shared_ptr<Muscle::TextRenderer> _bossData;

	std::shared_ptr<Character> _boss; // 보스의 정보를 출력하기 위해 캐싱..

	int _index;

	std::shared_ptr<Muscle::GameObject> _gameobject; // 피킹 오브젝트!

private:
	void CameraInfoUpdate();

	void ObjectInfoUpdate();

	void ObjectCountUpdate();

	void BossInfoUpdate();

public:
	static std::shared_ptr<DebugObjectManager> Get();

	void PickGameObject(std::shared_ptr<Muscle::GameObject> gameobj);

	virtual void Start() override;

	virtual void Update() override;

	virtual void LateUpdate() override; // 모든 Object Udpate돌릴 때 (ObjectManager) 오브젝트가 Components를 가지고 있으면 같이 업데이트 해주기.

	virtual void Finalize() override;

};

