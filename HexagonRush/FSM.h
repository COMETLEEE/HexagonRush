#pragma once

/// <summary>
/// 이 정도는 모두 공통이지 않을까? 라는 생각으로 템플릿으로 만듬!
/// 하지만 플레이어만 쓰겠지..
/// 2022.08.09 신성현
/// </summary>
template<class T>
class FSM : public std::enable_shared_from_this<T>
{
public:
	FSM() = default;

	~FSM() 
	{
		int a = 0;
	};

public:
	void Finalize()
	{
		_currentStatus.reset();
	}

protected:
	std::shared_ptr<IState<T>> _currentStatus;

	void SetState(std::shared_ptr<IState<T>> state)
	{
		// FSM에 들어오기 전, state가 Prev와 Curr가 같으면 return한다. 따라서, 여기 들어왔다는 것은
		// State의 Transition이 일어난다는 것 ..!! => 따라서 OnExit 발생 !
		if (_currentStatus)
		{
			_currentStatus->OnExit();

			_currentStatus.reset();
		}

		_currentStatus = state;

		_currentStatus->OnEnter(this->shared_from_this());
	};
};