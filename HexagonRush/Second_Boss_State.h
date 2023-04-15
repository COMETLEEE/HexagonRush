#pragma once

namespace Second_BossState
{
	class Idle : public IState<Second_Boss>
	{
	public:
		virtual void OnEnter(std::shared_ptr<Second_Boss> boss)
		{
			this->_character = boss;
			this->_character.lock()->GetAnimater()->Play("2_Boss_idle_00");
		}
		virtual void OnStay()
		{
			if (!this->_character.lock()->GetAnimater()->GetisPlay())
				this->_character.lock()->GetAnimater()->Play("2_Boss_idle_00");


			this->_character.lock()->GetGameObject()->GetTransform()->SetLook(XMVectorLerp(
				this->_character.lock()->GetGameObject()->GetTransform()->GetLook(),
				Vector3(1, 0, 0),
				Muscle::CTime::GetGameDeltaTime()));

		}
		virtual void OnExit()
		{
			this->_character.lock()->GetAnimater()->Stop();
		}
	};

	class Attack1 : public IState<Second_Boss>
	{
	private:
		bool _isReturnAnimPlay = false;
	public:
		virtual void OnEnter(std::shared_ptr<Second_Boss> boss)
		{
			this->_character = boss;
			this->_character.lock()->GetAnimater()->Play("2_Boss_Jump_01");
		}
		virtual void OnStay()
		{
			if (!this->_character.lock()->GetAnimater()->GetisPlay())
			{
				if (_isReturnAnimPlay)
				{
					this->_character.lock()->SetState(Second_Boss::IDLE);
					return;
				}
				else
				{
					this->_character.lock()->GetAnimater()->Play("2_Boss_Stomp_02");
					_isReturnAnimPlay = true;
				}
			}
		}

		virtual void OnExit()
		{
			this->_character.lock()->GetAnimater()->Stop();
		}
	};

	class Attack2 : public IState<Second_Boss>
	{
	private:
		int _count = 0;
		bool _isReturnAnimPlay = false;
	public:
		virtual void OnEnter(std::shared_ptr<Second_Boss> boss)
		{
			this->_character = boss;
			this->_character.lock()->GetAnimater()->Play("2_Boss_Jump_01");
		}
		virtual void OnStay()
		{

			if (!this->_character.lock()->GetAnimater()->GetisPlay())
			{
				if (_isReturnAnimPlay)
				{
					this->_character.lock()->SetState(Second_Boss::IDLE);
					return;
				}
				if (_count < 5)
				{
					this->_character.lock()->GetAnimater()->Play("2_Boss_Jump_01");
					_count++;
				}
				else
				{
					this->_character.lock()->GetAnimater()->Play("2_Boss_Stomp_02");
					_isReturnAnimPlay = true;
					return;
				}

			}

		}
		virtual void OnExit()
		{
			this->_character.lock()->GetAnimater()->Stop();
		}
	};
	class Attack3 : public IState<Second_Boss> // 2_Boss_thrusting_03
	{
	private:
		bool _isMoveComplete = false;
		bool _isReturn = false;
	public:
		virtual void OnEnter(std::shared_ptr<Second_Boss> boss)
		{
			this->_character = boss;
			this->_character.lock()->GetAnimater()->Play("2_Boss_Jump_01");
		}
		virtual void OnStay()
		{
			if (!_isReturn)
			{
				if (!_isMoveComplete)
				{
					if (!this->_character.lock()->GetAnimater()->GetisPlay())
					{
						this->_character.lock()->GetAnimater()->Play("2_Boss_thrusting_03");
						_isMoveComplete = true;
						return;
					}

				}
				else
				{
					if (!this->_character.lock()->GetAnimater()->GetisPlay())
					{
						this->_character.lock()->GetAnimater()->Play("2_Boss_Stomp_02");
						_isReturn = true;
						return;
					}

					if (this->_character.lock()->GetAnimater()->GetEventKey("Attack3")) return;

					Vector3 targetLook = this->_character.lock()->GetTarget()->GetGameObject()->GetTransform()->GetPosition()
						- this->_character.lock()->GetGameObject()->GetTransform()->GetPosition();
					targetLook.y = 0;
					targetLook.Normalize();

					Vector3 bossLook = this->_character.lock()->GetGameObject()->GetTransform()->GetLook();
					Vector3 lerp = XMVectorLerp(bossLook, targetLook, Muscle::CTime::GetGameDeltaTime() * 2);
					this->_character.lock()->GetGameObject()->GetTransform()->SetLook(lerp);
				}
			}
			else
			{
				if (!this->_character.lock()->GetAnimater()->GetisPlay())
				{
					this->_character.lock()->SetState(Second_Boss::IDLE);
					return;
				}
			}

		}
		virtual void OnExit()
		{
			this->_character.lock()->GetAnimater()->Stop();
		}
	};
	class Attack4 : public IState<Second_Boss>
	{
	private:
		int _index = 0;
		// 미사일 발사 횟수.
		int _targetIndex = 2; // 미사일 목표 발사 횟수.

		bool _isFire = false;

		bool _scaleReturn = false;

		float _timer = 0;

		std::shared_ptr<Muscle::GameObject> _drill;
	public:
		virtual void OnEnter(std::shared_ptr<Second_Boss> boss)
		{
			this->_character = boss;
			this->_character.lock()->GetAnimater()->Play("2_Boss_Dril_launch_04");
			_drill = this->_character.lock()->GetDrill();

			switch (boss->GetPhase())
			{
			case Second_Boss::Phase1:
				break;
			case Second_Boss::Phase2:
				_targetIndex = 2;
				break;
			case Second_Boss::Phase3:
				_targetIndex = 3;
				break;
			case Second_Boss::Phase4:
				_targetIndex = 4;
				break;
			default:
				break;
			}

		}
		virtual void OnStay()
		{

			if (_isFire)
			{
				_timer += Muscle::CTime::GetGameDeltaTime();
				if (_timer > 3.5f * _targetIndex)
				{
					if (!_scaleReturn)
					{

						this->_character.lock()->GetAnimater()->SetAnimSpeed(1, 0);
						_drill->GetTransform()->SetScale(0.0f, 0.0f, 0.0f);
						_scaleReturn = true;
					}
					_drill->GetTransform()->SetPosition(0.0f, 0.0f, 0.0f);

					_drill->GetTransform()->SetXMLocalTM(Matrix::CreateScale(_drill->GetTransform()->GetScale() += Vector3(1.0f, 1.0f, 1.0f) * Muscle::CTime::GetGameDeltaTime()));
					_drill->GetTransform()->Pitch(3.14f / 2.0f);

					this->_character.lock()->GetAnimater()->SetAnimSpeed(1, 0);
					if (!this->_character.lock()->GetAnimater()->GetisPlay())
					{
						if (_drill->GetTransform()->GetScale().x >= 3.0f)
						{
							_drill->GetTransform()->SetScale(3.0f, 3.0f, 3.0f);
							this->_character.lock()->SetState(Second_Boss::IDLE);
						}
						return;
					}
				}
			}
			else
			{
				if (!this->_character.lock()->GetAnimater()->GetEventKey("Attack4")) return;

				auto func = [](std::shared_ptr<Muscle::GameObject> gameObject) {
					// 시작위치.
					// 동 0
					// 서 1
					// 남 2
					// 북 3
					const float legth = 20.0f;

					Vector3 from = Vector3(0, 0, 0); // 월드 좌표로 계산하고싶어~!
					Vector3 to = Vector3(0, 0, 0);
					int _case = Muscle::CTime::GetRand<int>(0, 3);
					switch (_case)
					{
					case 0: // 동
					{
						from = Vector3(-legth * 4, 15.0f, 0.0f);
						from.z = Muscle::CTime::GetFloatRand<float>(-legth, legth);

						to = Vector3(legth * 4, 15.0f, 0.0f);
						to.z = Muscle::CTime::GetFloatRand<float>(-legth, legth);
						break;
					}
					case 1: // 서
					{
						from = Vector3(legth * 4, 15.0f, 0.0f);
						from.z = Muscle::CTime::GetFloatRand<float>(-legth, legth);

						to = Vector3(-legth * 4, 15.0f, 0.0f);
						to.z = Muscle::CTime::GetFloatRand<float>(-legth, legth);
						break;
					}
					case 2: // 남
					{
						from = Vector3(0.0f, 15.0f, -legth * 4);
						from.x = Muscle::CTime::GetFloatRand<float>(-legth, legth);

						to = Vector3(0.0f, 15.0f, legth * 4);
						to.x = Muscle::CTime::GetFloatRand<float>(-legth, legth);
						break;

					}
					case 3: // 북
					{
						from = Vector3(0.0f, 15.0f, legth * 4);
						from.x = Muscle::CTime::GetFloatRand<float>(-legth, legth);

						to = Vector3(0.0f, 15.0f, -legth * 4);
						to.x = Muscle::CTime::GetFloatRand<float>(-legth, legth);
						break;
					}

					default:
						break;
					}
					GetSoundManager()->Play("DrillMove", IPlayMode::Effect);
					//gameObject->GetTransform()->SetWorldPosition(from);
					ObjectMover::Get()->MoveFromTo(gameObject->GetTransform(), from, to, 1.5f, 2.0f, MoveInfo::Linear, Vector3::Zero, Vector3::Zero
						, MoveInfo::Up, true);


					from.x *= 0.8f;
					from.y = 0.0f;
					from.z *= 0.8f;
					to.x *= 0.8f;
					to.y = 0.0f;
					to.z *= 0.8f;

					for (int i = 0; i < 10; i++)
					{
						auto createDrillFunc = [=](std::shared_ptr<Muscle::GameObject> _gameObject) {
							Vector3 look = from - to;
							look.Normalize();
							Vector3 lerp = XMVectorLerp(from, to, (float)i / 9.0f);
							auto gameObject = std::make_shared<Drill_Sign_Prefab>()->GetGameObject();
							gameObject->GetTransform()->SetPosition(lerp);
							gameObject->GetTransform()->SetLook(look);
							gameObject->GetTransform()->RotateY(-3.14f / 2);
						};
						Muscle::CTime::Invoke(createDrillFunc, 0.1f * i, nullptr);

					}
				};

				for (int i = 0; i < _targetIndex; i++)
				{
					Muscle::CTime::Invoke(func, 2.0f + 2.0f * i, _drill);
					_isFire = true;
				}


				ObjectMover::Get()->MoveBy(_drill->GetTransform(), Vector3(_drill->GetTransform()->GetWorldUp().x, 0, _drill->GetTransform()->GetWorldUp().y) * 200, 1.5f, 0, MoveInfo::Linear, Vector3::Zero, Vector3::Zero
					, MoveInfo::Up, true);
			}
		}
		virtual void OnExit()
		{
			this->_character.lock()->GetAnimater()->Stop();
		}
	};
	class Attack5 : public IState<Second_Boss>
	{
	public:
		virtual void OnEnter(std::shared_ptr<Second_Boss> boss)
		{
			this->_character = boss;
			this->_character.lock()->GetAnimater()->Play("2_Boss_thrusting_DrilUp_05");
		}
		virtual void OnStay()
		{
			if (!this->_character.lock()->GetAnimater()->GetisPlay())
				this->_character.lock()->SetState(Second_Boss::IDLE);
		}
		virtual void OnExit()
		{
			this->_character.lock()->GetAnimater()->Stop();
		}
	};

	class Attack6 : public IState<Second_Boss>
	{
	public:
		virtual void OnEnter(std::shared_ptr<Second_Boss> boss)
		{
			this->_character = boss;
			this->_character.lock()->GetAnimater()->Play("2_Boss_oilBlack_Bob_06");
		}
		virtual void OnStay()
		{
			if (!this->_character.lock()->GetAnimater()->GetisPlay())
				this->_character.lock()->SetState(Second_Boss::IDLE);
		}
		virtual void OnExit()
		{
			this->_character.lock()->GetAnimater()->Stop();
		}
	};
	class Attack7 : public IState<Second_Boss>
	{
	public:
		virtual void OnEnter(std::shared_ptr<Second_Boss> boss)
		{
			this->_character = boss;
			this->_character.lock()->GetAnimater()->Play("2_Boss_electronicYellow_Bob_07");
		}
		virtual void OnStay()
		{
			if (!this->_character.lock()->GetAnimater()->GetisPlay())
				this->_character.lock()->SetState(Second_Boss::IDLE);
		}
		virtual void OnExit()
		{
			this->_character.lock()->GetAnimater()->Stop();
		}
	};
	class Attack8 : public IState<Second_Boss>
	{
	public:
		virtual void OnEnter(std::shared_ptr<Second_Boss> boss)
		{
			this->_character = boss;
			this->_character.lock()->GetAnimater()->Play("2_Boss_LandmineRed_Bob_08");
		}
		virtual void OnStay()
		{
			if (!this->_character.lock()->GetAnimater()->GetisPlay())
				this->_character.lock()->SetState(Second_Boss::IDLE);
		}
		virtual void OnExit()
		{
			this->_character.lock()->GetAnimater()->Stop();
		}
	};
	class Attack9 : public IState<Second_Boss>
	{
	public:
		virtual void OnEnter(std::shared_ptr<Second_Boss> boss)
		{
			this->_character = boss;
			this->_character.lock()->GetAnimater()->Play("2_Boss_PanicBlue_Bob_09");
		}
		virtual void OnStay()
		{
			if (!this->_character.lock()->GetAnimater()->GetisPlay())
				this->_character.lock()->SetState(Second_Boss::IDLE);
		}
		virtual void OnExit()
		{
			this->_character.lock()->GetAnimater()->Stop();
		}
	};

	class Die : public IState<Second_Boss>
	{
	public:
		virtual void OnEnter(std::shared_ptr<Second_Boss> boss)
		{
			this->_character = boss;
			this->_character.lock()->GetAnimater()->Play("2_Boss_Dead_10");


			// 다음 스테이지의 이름을 저장한다. => 일단 타이틀씬 .. (스테이지 2가 없다 ..!)
			StageClearScene::SetNextSceneName("TitleScene");
		}
		virtual void OnStay()
		{

		}
		virtual void OnExit()
		{
			this->_character.lock()->GetAnimater()->Stop();
		}
	};
}

