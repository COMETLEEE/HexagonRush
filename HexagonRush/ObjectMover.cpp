#include "pch.h"
#include "ObjectMover.h"
#include <list>

std::shared_ptr<ObjectMover> ObjectMover::_Instance = nullptr;

bool MoveInfo::Update()
{

	if (_delay > _startTime)
	{
		_delay -= Muscle::CTime::GetGameDeltaTime();
		return false;
	}
	if (_startTime > _time)
	{
		if (_lineType == MoveInfo::Shake)
			_Transform->SetWorldPosition(_intialPos);

		return true;
	}

	t = _startTime / _time;

	s = 1 - t;

	switch (_lineType)
	{
	case MoveInfo::Linear:
		_NowPos = LinearLine();
		break;
	case MoveInfo::Bezier:
		_NowPos = BeizerCurve();
		break;
	case MoveInfo::Shake:
		_NowPos = ShakeMove();
		break;
	default:
		break;
	}

	_Transform->SetWorldPosition(_NowPos);

	t = (_startTime + 0.16) / _time; // 1프레임 뒤의 위치를 LookAt하기 위함.
	s = 1 - t;

	switch (_lineType)
	{
	case MoveInfo::Linear:
		_NowPos = LinearLine();
		break;
	case MoveInfo::Bezier:
		_NowPos = BeizerCurve();
		break;
	case MoveInfo::Shake:
		_NowPos = ShakeMove();
		break;
	default:
		break;
	}

	if (_LookTarget)
	{
		_Transform->LookAt(_LookTarget->GetGameObject());
	}
	else
	{
		switch (_LookType)
		{
		case MoveInfo::Up:
			if (_isOrientToPath)
				_Transform->UpAt(_NowPos);
			if (_vectorTarget != Vector3())
				_Transform->UpAt(_vectorTarget);
			break;
		case MoveInfo::Look:
			if (_isOrientToPath)
				_Transform->LookAt(_NowPos);
			if (_vectorTarget != Vector3())
				_Transform->LookAt(_vectorTarget);
			break;
		default:
			break;
		}
	}

	_startTime += Muscle::CTime::GetGameDeltaTime();

	_isStart = true;

	return false;
}


Vector3 MoveInfo::BeizerCurve()
{

	Vector3 V1, V2, V3, V4;

	switch (_moveType)
	{
	case MoveInfo::MoveBy:
		if (!_isStart)
			_vectorFrom = _Transform->GetWorldPosition();
		V1 = _vectorFrom;
		V4 = _vectorFrom + _vectorBy;
		V2 = V1 + _bezierVector1;
		V3 = V4 + _bezierVector2;
		break;
	case MoveInfo::MoveTo:
		if (!_isStart)
			_vectorFrom = _Transform->GetWorldPosition();
		V1 = _vectorFrom;
		V4 = _vectorTo;
		V2 = V1 + _bezierVector1;
		V3 = V4 + _bezierVector2;
		break;
	case MoveInfo::MoveFormBy:
		V1 = _vectorFrom;
		V4 = _vectorFrom + _vectorBy;
		V2 = V1 + _bezierVector1;
		V3 = V4 + _bezierVector2;
		break;
	case MoveInfo::MoveFromTo:
		V1 = _vectorFrom;
		V4 = _vectorTo;
		V2 = V1 + _bezierVector1;
		V3 = V4 + _bezierVector2;
		break;
	default:
		break;
	}

	auto A = pow(s, 3) * V1;
	auto B = 3 * (pow(s, 2) * t) * V2;
	auto C = 3 * (pow(t, 2) * s) * V3;
	auto D = pow(t, 3) * V4;

	return A + B + C + D;
}

Vector3 MoveInfo::LinearLine()
{

	Vector3 V1, V2;


	switch (_moveType)
	{
	case MoveInfo::MoveBy:
		if (!_isStart)
			_vectorFrom = _Transform->GetWorldPosition();
		V1 = _vectorFrom;
		V2 = _vectorFrom + _vectorBy;
		break;
	case MoveInfo::MoveTo:
		if (!_isStart)
			_vectorFrom = _Transform->GetWorldPosition();
		V1 = _vectorFrom;
		V2 = _vectorTo;
		break;
	case MoveInfo::MoveFormBy:
		V1 = _vectorFrom;
		V2 = _vectorFrom + _vectorBy;
		break;
	case MoveInfo::MoveFromTo:
		V1 = _vectorFrom;
		V2 = _vectorTo;
		break;
	default:
		break;
	}


	return XMVectorLerp(V1, V2, t);
}

DirectX::SimpleMath::Vector3 MoveInfo::ShakeMove()
{
	if (!_isRetureInit)
		_intialPos = _Transform->GetWorldPosition();
	auto V1 = _intialPos;
	V1 += _Transform->GetUp() * Muscle::CTime::GetFloatRand<float>(-_power, _power) * Muscle::CTime::GetGameSpeed();
	V1 += _Transform->GetRight() * Muscle::CTime::GetFloatRand<float>(-_power, _power) * Muscle::CTime::GetGameSpeed();

	return V1;
}

ObjectMover::ObjectMover(std::shared_ptr<Muscle::GameObject> gameObject) :IComponents(gameObject)
{

}

ObjectMover::~ObjectMover()
{

}

std::shared_ptr<ObjectMover> ObjectMover::Get()
{
	if (!_Instance)
		_Instance = Muscle::CreateGameObject()->AddComponent<ObjectMover>();

	return _Instance;
}

void ObjectMover::MoveBy(std::shared_ptr<Muscle::Transform> transform, Vector3 vectorBy, float time, float delay, MoveInfo::LineType _lineType, Vector3 bezierVector1, Vector3 bezierVector2, MoveInfo::LookType looktype, bool isOrientToPath, Vector3 vectorTarget, std::shared_ptr<Muscle::Transform> lookTarget)
{
	MoveInfo moveInfo;
	moveInfo._Transform = transform;
	moveInfo._vectorBy = vectorBy;
	moveInfo._time = time;
	moveInfo._delay = delay;
	moveInfo._lineType = _lineType;
	moveInfo._isOrientToPath = isOrientToPath;
	moveInfo._moveType = MoveInfo::MoveBy;
	moveInfo._vectorTarget = vectorTarget;
	moveInfo._bezierVector1 = bezierVector1;
	moveInfo._bezierVector2 = bezierVector2;
	moveInfo._LookType = looktype;
	moveInfo._LookTarget = lookTarget;
	_moveInfo.push_back(moveInfo);

}

void ObjectMover::MoveTo(std::shared_ptr<Muscle::Transform> transform, Vector3 vectorTo, float time, float delay /*= 0*/, MoveInfo::LineType _lineType /*= MoveInfo::Linear*/, Vector3 bezierVector1, Vector3 bezierVector2, MoveInfo::LookType looktype, bool isOrientToPath /*= false*/, Vector3 vectorTarget, std::shared_ptr<Muscle::Transform> lookTarget /*= Vector3()*/)
{
	MoveInfo moveInfo;
	moveInfo._Transform = transform;
	moveInfo._vectorTo = vectorTo;
	moveInfo._time = time;
	moveInfo._delay = delay;
	moveInfo._lineType = _lineType;
	moveInfo._isOrientToPath = isOrientToPath;
	moveInfo._moveType = MoveInfo::MoveTo;
	moveInfo._vectorTarget = vectorTarget;
	moveInfo._bezierVector1 = bezierVector1;
	moveInfo._bezierVector2 = bezierVector2;
	moveInfo._LookType = looktype;
	moveInfo._LookTarget = lookTarget;
	_moveInfo.push_back(moveInfo);
}

void ObjectMover::MoveFromBy(std::shared_ptr<Muscle::Transform> transform, Vector3 vectorFrom, Vector3 vectorBy, float time, float delay /*= 0*/, MoveInfo::LineType _lineType /*= MoveInfo::Linear*/, Vector3 bezierVector1, Vector3 bezierVector2, MoveInfo::LookType looktype, bool isOrientToPath /*= false*/, Vector3 vectorTarget, std::shared_ptr<Muscle::Transform> lookTarget /*= Vector3()*/)
{
	MoveInfo moveInfo;
	moveInfo._Transform = transform;
	moveInfo._vectorFrom = vectorFrom;
	moveInfo._vectorBy = vectorBy;
	moveInfo._time = time;
	moveInfo._delay = delay;
	moveInfo._lineType = _lineType;
	moveInfo._isOrientToPath = isOrientToPath;
	moveInfo._moveType = MoveInfo::MoveFormBy;
	moveInfo._vectorTarget = vectorTarget;
	moveInfo._bezierVector1 = bezierVector1;
	moveInfo._bezierVector2 = bezierVector2;
	moveInfo._LookType = looktype;
	moveInfo._LookTarget = lookTarget;
	_moveInfo.push_back(moveInfo);
}

void ObjectMover::MoveFromTo(std::shared_ptr<Muscle::Transform> transform, Vector3 vectorFrom, Vector3 vectorTo, float time, float delay /*= 0*/, MoveInfo::LineType _lineType /*= MoveInfo::Linear*/, Vector3 bezierVector1, Vector3 bezierVector2, MoveInfo::LookType looktype, bool isOrientToPath /*= false*/, Vector3 vectorTarget, std::shared_ptr<Muscle::Transform> lookTarget/*= Vector3()*/)
{
	MoveInfo moveInfo;
	moveInfo._Transform = transform;
	moveInfo._vectorFrom = vectorFrom;
	moveInfo._vectorTo = vectorTo;
	moveInfo._time = time;
	moveInfo._delay = delay;
	moveInfo._lineType = _lineType;
	moveInfo._isOrientToPath = isOrientToPath;
	moveInfo._moveType = MoveInfo::MoveFromTo;
	moveInfo._vectorTarget = vectorTarget;
	moveInfo._bezierVector1 = bezierVector1;
	moveInfo._bezierVector2 = bezierVector2;
	moveInfo._LookType = looktype;
	moveInfo._LookTarget = lookTarget;
	_moveInfo.push_back(moveInfo);
}

void ObjectMover::Shake(std::shared_ptr<Muscle::Transform> transform, float time, float power, float delay /*= 0*/, bool isReturnInit)
{
	MoveInfo moveInfo;
	moveInfo._Transform = transform;
	moveInfo._time = time;
	moveInfo._delay = delay;
	moveInfo._lineType = MoveInfo::Shake;
	moveInfo._intialPos = transform->GetWorldPosition();
	moveInfo._power = power;
	moveInfo._isRetureInit = isReturnInit;
	_moveInfo.push_back(moveInfo);

	std::function<void(std::shared_ptr<Muscle::GameObject>)> func1 = [](std::shared_ptr<Muscle::GameObject> obj) {
		Muscle::XPad::Get()->StartVibration(16000, 16000);
	};

	std::function<void(std::shared_ptr<Muscle::GameObject>)> func2 = [](std::shared_ptr<Muscle::GameObject> obj) {
		Muscle::XPad::Get()->StopVibration();
	};

	Muscle::CTime::Invoke(func1, delay, GetGameObject());

	Muscle::CTime::Invoke(func2, delay + time, GetGameObject());
}

void ObjectMover::LinearLook(std::shared_ptr<Muscle::Transform> transform, float time, std::shared_ptr<Muscle::Transform> lookTarget, Vector3 lookTargetVector, float _speed, float delay, Vector3 offset)
{
	LookInfo lookInfo;
	lookInfo._Transform = transform;
	lookInfo._time = time;
	lookInfo._LookTarget = lookTarget;
	lookInfo._LookTargetVector = lookTargetVector;
	lookInfo._speed = _speed;
	lookInfo._delay = delay;
	lookInfo._offset = offset;

	_LookInfo.push_back(lookInfo);
}

void ObjectMover::DeleteObject(std::shared_ptr<Muscle::GameObject> gameObject)
{
	for (auto iter = _LookInfo.begin(); iter != _LookInfo.end(); iter++)
	{
		if (iter->_Transform == gameObject->GetTransform())
		{
			_LookInfo.erase(iter);
			break;
		}
	}
	for (auto iter = _LookInfo.begin(); iter != _LookInfo.end(); iter++)
	{
		if (iter->_Transform == gameObject->GetTransform())
		{
			_LookInfo.erase(iter);
			break;
		}
	}
}

void ObjectMover::LateUpdate()
{
	if (GameProcess::Get()->GetIsPausePanelUp())
		return;

	if (!_moveInfo.empty())
	{
		for (auto iter = _moveInfo.begin(); iter != _moveInfo.end(); iter++)
		{
			for (auto& iter1 : Muscle::IGameEngine::Get()->GetObjManager()->GetDeleteObjects())
			{
				if (iter1 == iter->_Transform->GetGameObject())
					iter = _moveInfo.erase(iter);
				if (iter == _moveInfo.end())
					break;
			}
			if (iter == _moveInfo.end())
				break;
			else
			{
				if (iter->Update())
					iter = _moveInfo.erase(iter);
			}
			if (iter == _moveInfo.end())
				break;
		}
	}

	if (!_LookInfo.empty())
	{
		for (auto iter = _LookInfo.begin(); iter != _LookInfo.end(); iter++)
		{
			for (auto& iter1 : Muscle::IGameEngine::Get()->GetObjManager()->GetDeleteObjects())
			{
				if (iter1 == iter->_Transform->GetGameObject())
					iter = _LookInfo.erase(iter);
				if (iter == _LookInfo.end())
					break;
			}
			if (iter == _LookInfo.end())
				break;
			else
			{
				if (iter->Update())
					iter = _LookInfo.erase(iter);
			}
			if (iter == _LookInfo.end())
				break;
		}
	}

}

void ObjectMover::Finalize()
{
	_Instance.reset();
}

bool LookInfo::Update()
{
	if (!_isStart)
		_startLook = _Transform->GetLook();
	if (_delay > _startTime)
	{
		_delay -= Muscle::CTime::GetGameDeltaTime();
		return false;
	}
	if (_startTime > _time)
	{

		return true;
	}

	t = _startTime / _time;
	Vector3 targetLook;
	t = t * _speed > 1 ? 1 : t * _speed;
	if (_LookTarget)
	{
		targetLook = _LookTarget->GetWorldPosition() + _offset - _Transform->GetWorldPosition();
		targetLook.Normalize();
		targetLook = XMVectorLerp(_startLook, targetLook, t);
	}
	else
	{
		targetLook = _LookTargetVector - _Transform->GetWorldPosition();
		targetLook.Normalize();
		targetLook = XMVectorLerp(_startLook, targetLook, t);
	}
	targetLook.Normalize();
	_Transform->SetLook(targetLook);

	_startTime += Muscle::CTime::GetGameDeltaTime();

	_isStart = true;

	return false;


}
