#pragma once

struct MoveInfo
{
	enum MoveType
	{
		MoveBy,
		MoveTo,
		MoveFormBy,
		MoveFromTo
	};
	enum LineType
	{
		Linear,
		Bezier,
		Shake
	};

	enum LookType
	{
		Up,
		Look
	};

	std::shared_ptr<Muscle::Transform> _Transform; // 움직일 녀석.

	Vector3 _vectorBy = Vector3::Zero;
	Vector3 _vectorTo = Vector3::Zero;
	Vector3 _vectorFrom = Vector3::Zero;

	/// 베지어 곡선 V2 = V1+_bezierVector1, V3 = V4+_bezierVector2
	Vector3 _bezierVector1 = Vector3::Zero;
	Vector3 _bezierVector2 = Vector3::Zero;

	Vector3 _vectorTarget;
	bool _isOrientToPath = false; // 진행방향으로 Look한다..!


	MoveType _moveType = MoveType::MoveBy;
	LineType _lineType = LineType::Linear;
	LookType _LookType = LookType::Look;

	float _time = 0;
	float _delay = 0;
	float _power = 0.1f;

	bool Update();

	Vector3 _intialPos = Vector3::Zero;

	bool _isRetureInit = false;

	std::shared_ptr<Muscle::Transform> _LookTarget; // Look할 녀석.

private:
	Vector3 BeizerCurve();
	Vector3 LinearLine();
	Vector3 ShakeMove();

	Vector3 _NowPos = Vector3::Zero;

	float _startTime = 0;
	float s = 0;
	float t = 0;

	bool _isStart = false;

	


};

struct LookInfo
{
	std::shared_ptr<Muscle::Transform> _Transform; // 움직일 녀석.

	Vector3 _LookTargetVector;

	std::shared_ptr<Muscle::Transform> _LookTarget; // Look할 녀석.

	float _speed = 1;

	float _time = 0;

	float _delay = 0;

	Vector3 _offset = Vector3::Zero;

	bool Update();
private:
	float _startTime = 0;

	float t = 0;

	bool _isStart = false;

	Vector3 _startLook = Vector3::Zero;
};
class ObjectMover : public Muscle::IComponents
{
public:
	ObjectMover(std::shared_ptr<Muscle::GameObject> gameObject);

	virtual ~ObjectMover();

private:
	static std::shared_ptr<ObjectMover> _Instance;

	std::list<MoveInfo> _moveInfo; // 리스트인 이유는 도중에 삭제와 추가를 자유롭게 하기위함!.

	std::list<LookInfo> _LookInfo; // 리스트인 이유는 도중에 삭제와 추가를 자유롭게 하기위함!.

public:
	static std::shared_ptr<ObjectMover> Get();

	void MoveBy(std::shared_ptr<Muscle::Transform> transform, Vector3 vectorBy, float time, float delay = 0, MoveInfo::LineType _lineType = MoveInfo::Linear, Vector3 bezierVector1 = Vector3(0.0f, 10.0f, 0.0f), Vector3 bezierVector2 = Vector3(0.0f, 10.0f, 0.0f), MoveInfo::LookType looktype = MoveInfo::Look, bool isOrientToPath = false, Vector3 vectorTarget = Vector3(), std::shared_ptr<Muscle::Transform> lookTarget = nullptr); // 물체를 기존의 위치에서 더하거나 빼서 이동시킨다.
	void MoveTo(std::shared_ptr<Muscle::Transform> transform, Vector3 vectorTo, float time, float delay = 0, MoveInfo::LineType _lineType = MoveInfo::Linear, Vector3 bezierVector1 = Vector3(0.0f, 10.0f, 0.0f), Vector3 bezierVector2 = Vector3(0.0f, 10.0f, 0.0f), MoveInfo::LookType looktype = MoveInfo::Look, bool isOrientToPath = false, Vector3 vectorTarget = Vector3(), std::shared_ptr<Muscle::Transform> lookTarget = nullptr); // 물체를 목표 지점까지 이동시킨다.
	void MoveFromBy(std::shared_ptr<Muscle::Transform> transform, Vector3 vectorFrom, Vector3 vectorBy, float time, float delay = 0, MoveInfo::LineType _lineType = MoveInfo::Linear, Vector3 bezierVector1 = Vector3(0.0f, 10.0f, 0.0f), Vector3 bezierVector2 = Vector3(0.0f, 10.0f, 0.0f), MoveInfo::LookType looktype = MoveInfo::Look, bool isOrientToPath = false, Vector3 vectorTarget = Vector3(), std::shared_ptr<Muscle::Transform> lookTarget = nullptr); // 물체를 시작 지점부터 이동시킨다.
	void MoveFromTo(std::shared_ptr<Muscle::Transform> transform, Vector3 vectorFrom, Vector3 vectorTo, float time, float delay = 0, MoveInfo::LineType _lineType = MoveInfo::Linear, Vector3 bezierVector1 = Vector3(0.0f, 10.0f, 0.0f), Vector3 bezierVector2 = Vector3(0.0f, 10.0f, 0.0f), MoveInfo::LookType looktype = MoveInfo::Look, bool isOrientToPath = false, Vector3 vectorTarget = Vector3(), std::shared_ptr<Muscle::Transform> lookTarget = nullptr); // 물체를 시작 지점부터 목표 지점까지 이동시킨다.

	void Shake(std::shared_ptr<Muscle::Transform> transform, float time, float power = 0.2f, float delay = 0, bool isReturnInit = false);

	void LinearLook(std::shared_ptr<Muscle::Transform> transform, float time, std::shared_ptr<Muscle::Transform> lookTarget = nullptr, Vector3 lookTargetVector = Vector3::Zero, float _speed = 1, float delay = 0, Vector3 offset = Vector3::Zero);

	void DeleteObject(std::shared_ptr<Muscle::GameObject> gameObject); // 게임 매니저에서 오브젝트를 삭제하면 무버의 오브젝트도 삭제해야한다..

	virtual void LateUpdate(); // 모든 Object Udpate돌릴 때 (ObjectManager) 오브젝트가 Components를 가지고 있으면 같이 업데이트 해주기.

	virtual void Finalize();
};

