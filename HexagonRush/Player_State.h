#pragma once

#include "DieScene.h"
#include "Rush_BulletController.h"
#include "ResourceManager.h"

// Z버튼
#define RUSH_SHOOTING_KEY_KEABOARD 0x005A

#define RUSH_SHOOTING_KEY_KEABOARD2 0x43

#define PLAYER_AnimSpeed 2
/// <summary>
/// Ch_Attack
/// Ch_Attack_Rush
/// Ch_Dead
/// Ch_Hurt
/// Ch_idle
/// Ch_Run
/// </summary>
namespace PlayerState
{
	class Idle : public IState<Player>
	{
	public:
		virtual void OnEnter(std::shared_ptr<Player> player)
		{
			player->GetAnimater()->SetAnimSpeed(0.5f, 10000.0f);
			this->_character = player;
			this->_character.lock()->GetAnimater()->Play("Ch_idle");
		}

		virtual void OnStay()
		{
			if (!this->_character.lock()->GetAnimater()->GetisPlay())
				this->_character.lock()->GetAnimater()->Play("Ch_idle");
		}
		 
		virtual void OnExit()
		{
			this->_character.lock()->GetAnimater()->Stop();
		}
	};

	class Move : public IState<Player>
	{
	public:
		virtual void OnEnter(std::shared_ptr<Player> player)
		{
			player->GetAnimater()->SetAnimSpeed(PLAYER_AnimSpeed, 10000.0f);
			this->_character = player;
			this->_character.lock()->GetAnimater()->Play("Ch_Run");
		}

		virtual void OnStay()
		{
			if (!this->_character.lock()->GetAnimater()->GetisPlay())
				this->_character.lock()->GetAnimater()->Play("Ch_Run");

			if (!Muscle::KeyBoard::Get()->KeyPress(VK_UP) &&
				!Muscle::KeyBoard::Get()->KeyPress(VK_RIGHT) &&
				!Muscle::KeyBoard::Get()->KeyPress(VK_LEFT) &&
				!Muscle::KeyBoard::Get()->KeyPress(VK_DOWN) &&
				!Muscle::XPad::Get()->IsPadStickMove(Muscle::XPadInput::Up) &&
				!Muscle::XPad::Get()->IsPadStickMove(Muscle::XPadInput::Right) &&
				!Muscle::XPad::Get()->IsPadStickMove(Muscle::XPadInput::Left) &&
				!Muscle::XPad::Get()->IsPadStickMove(Muscle::XPadInput::Down))
			{
				this->_character.lock()->SetState(Player::IDLE);
			}
		}

		virtual void OnExit()
		{
			this->_character.lock()->GetAnimater()->Stop();
		}
	};

	class Attack : public IState<Player>
	{
	public:
		virtual void OnEnter(std::shared_ptr<Player> player)
		{
			player->GetAnimater()->SetAnimSpeed(PLAYER_AnimSpeed, 10000.0f);
			GetSoundManager()->Play("Attack", IPlayMode::Effect);
			this->_character = player;
			this->_character.lock()->GetAnimater()->Play("Ch_Attack");
		}

		virtual void OnStay()
		{
			if (!this->_character.lock()->GetAnimater()->GetisPlay())
				this->_character.lock()->SetState(Player::IDLE);
		}

		virtual void OnExit()
		{
			this->_character.lock()->GetAnimater()->Stop();
		}
	};

	class Hit : public IState<Player>
	{
		std::shared_ptr<Player> _player;

	public:
		virtual void OnEnter(std::shared_ptr<Player> player)
		{
			_player = player;

			player->GetAnimater()->SetAnimSpeed(PLAYER_AnimSpeed, 10000.0f);
			this->_character = player;
			this->_character.lock()->GetAnimater()->Play("Ch_Hurt");

			GetSoundManager()->Play("Hit", IPlayMode::Effect);

			// 맞을 때는 선택을 취소한다.
			TileManager::Get()->SelectCancel();

			// 맞았을 때 무적 만들어주자 ..
			player->SetInvincible();
		}

		virtual void OnStay()
		{
			if (!this->_character.lock()->GetAnimater()->GetisPlay())
				this->_character.lock()->SetState(Player::IDLE);
		}

		virtual void OnExit()
		{
			this->_character.lock()->GetAnimater()->Stop();

			// 1.5초 후 무적을 풀어주자.
			std::function<void(std::shared_ptr<Muscle::GameObject>)> func1 = [](std::shared_ptr<Muscle::GameObject> gameObj) {
				gameObj->GetComponent<Player>()->SetNotInvincible();
			};

			Muscle::CTime::Invoke(func1, 1.5f, _player->GetGameObject());
		}
	};

	class Evasion : public IState<Player>
	{
	private:
		Vector3 _currEvasionWay;

		std::shared_ptr<Player> _currPlayer;

	public:
		virtual void OnEnter(std::shared_ptr<Player> player)
		{
			player->GetAnimater()->SetAnimSpeed(PLAYER_AnimSpeed, 10000.0f);

			GetSoundManager()->Play("Evade", IPlayMode::Effect);

			_currPlayer = player;

			this->_character = player;

			this->_character.lock()->GetAnimater()->Play("Ch_evasion");

			std::shared_ptr<Muscle::Transform> transform = this->_character.lock()->GetGameObject()->GetTransform();

			// 피격 판정용 콜라이더 OFF ..!
			this->_character.lock()->GetComponent<Muscle::Collider>()->SetIsEnable(false);

			// 현재 믹사모 모델은 오프셋이 있어서 Up이 Look이다.
			_currEvasionWay = -transform->GetLook();

			_currEvasionWay.y = 0.f;

			// 애니메이션 타임 동안 이동 => 오브젝트 무버로 이동하면 뚫어버림
			// ObjectMover::Get()->MoveBy(transform, moveWay * player->GetSpeed(), 1.5f);

			// 회피할 때는 선택을 취소한다.
			TileManager::Get()->SelectCancel();
		}

		virtual void OnStay()
		{
			if (!this->_character.lock()->GetAnimater()->GetisPlay())
			{
				this->_character.lock()->SetState(Player::IDLE);

				return;
			}

			std::shared_ptr<Muscle::Transform> transform = this->_character.lock()->GetGameObject()->GetTransform();

			// 현재 믹사모 모델은 오프셋이 있어서 Up이 Look이다.
			Vector3 currPos = transform->GetPosition();

			transform->SetPosition(currPos + _currEvasionWay * Muscle::CTime::GetGameDeltaTime() * _currPlayer->GetSpeed() * 2);
		}

		virtual void OnExit()
		{
			// 피격 판정용 콜라이더 ON !
			this->_character.lock()->GetComponent<Muscle::Collider>()->SetIsEnable(true);

			this->_character.lock()->GetAnimater()->Stop();
		}
	};

	constexpr float RUSH_FULL_TIME = 5.f;

	constexpr float RUSH_GAME_SPEED = 0.2f;

	/// <summary>
/// Ch_Attack
/// Ch_Attack_Rush
/// Ch_Dead
/// Ch_Hurt
/// Ch_idle
/// Ch_Run
/// </summary>

	class Rush : public IState<Player>
	{
	private:
		float _rushTime;

		// 그냥 캐싱하고 사용할래 !
		std::shared_ptr<Player> _player;

	public:
		virtual void OnEnter(std::shared_ptr<Player> player)
		{
			player->GetAnimater()->SetAnimSpeed(PLAYER_AnimSpeed * 10, 10000.0f); // 러쉬는 다른 애니메이션보다 5배 빠르게 하자,

			_player = player;

			this->_character = player;

			// 혹시 모르니 Offset 2 sec
			Muscle::CTime::SetGameSpeed(RUSH_GAME_SPEED, RUSH_FULL_TIME + 2.f);

			_rushTime = 0.f;

			// 러쉬 애니메이션 있으면 실시
			this->_character.lock()->GetAnimater()->Play("Ch_Attack_Rush");

			// 카메라 무브
			CameraMovementManager::Get()->PlayerRushMove();

			GetSoundManager()->SetSoundSpeed(Muscle::CTime::GetGameSpeed(), IPlayMode::Effect);
			GetSoundManager()->SetSoundSpeed(Muscle::CTime::GetGameSpeed(), IPlayMode::BGM);

			this->_character.lock()->GetRushButtonUI()->GetGameObject()->SetIsEnable(true);
			auto uiData = this->_character.lock()->GetRushButtonUI()->GetUIData();

			uiData->_drawInfo->_textureID = ResourceManager::Get()->GetTextureID(TEXT("ButtonUp"));

			Muscle::XPad::Get()->StartVibration();
		}

		virtual void OnStay()
		{
			// 시간초를 새면서 러쉬 타임이 끝날 때까지 러쉬를 진행한다.
			// 러쉬 타임이 끝나면 ..
			_rushTime += Muscle::CTime::GetDeltaTime();
			auto uiData = this->_character.lock()->GetRushButtonUI()->GetUIData();

			uiData->_drawInfo->_textureID = ResourceManager::Get()->GetTextureID(TEXT("ButtonUp"));

			// 잔여 러쉬 시간
			_player->SetRushRemainingTime(RUSH_FULL_TIME - _rushTime);

			if (_rushTime >= RUSH_FULL_TIME)
			{
				this->_character.lock()->SetState(Player::IDLE);

				return;
			}


			auto CreateBullet = [&]()
			{
				// 탄환 오브젝트 생성
				std::shared_ptr<Muscle::GameObject> rushBullet = _player->GetRushBullet();

				// Stage 1 보스 코어 쪽으로 날아갈 수 있는 하드 코딩
				Vector3 dest = _player->GetGameObject()->GetTransform()->GetWorldPosition() + (_player->GetTarget()->GetGameObject()->GetTransform()->GetWorldPosition() - _player->GetGameObject()->GetTransform()->GetWorldPosition())
					+ _player->GetTarget()->GetGameObject()->GetTransform()->GetUp() * 8;

				// 발사 !
				rushBullet->GetComponent<Rush_BulletController>()->LaunchTo(dest);

				uiData->_drawInfo->_textureID = ResourceManager::Get()->GetTextureID(TEXT("ButtonDown"));
				ObjectMover::Get()->Shake(Muscle::IGameEngine::Get()->GetMainCamera()->GetTransform(), 0.01f);

				GetSoundManager()->Play("Rush", IPlayMode::Effect);

				this->_character.lock()->GetAnimater()->RePlay("Ch_Attack_Rush");
			};


#pragma region USE_XPAD
			//  보스로 원을 조절점으로 하는 배지어 곡선을 그리며 날아가는 탄환 오브젝트 발사 (Rush_BulletController)
			if (Muscle::XPad::Get()->IsPadCommand(Muscle::XPadInput::LTRIGGER, Muscle::XPadCommand::Push) ||
				Muscle::XPad::Get()->IsPadCommand(Muscle::XPadInput::RTRIGGER, Muscle::XPadCommand::Push))
			{
				CreateBullet();
			}
#pragma endregion

#pragma region USE_KEYBOARD
			// 보스로 원을 조절점으로 하는 배지어 곡선을 그리며 날아가는 탄환 오브젝트 발사 (Rush_BulletController)
			if (Muscle::KeyBoard::Get()->KeyDown(RUSH_SHOOTING_KEY_KEABOARD) || Muscle::KeyBoard::Get()->KeyDown(RUSH_SHOOTING_KEY_KEABOARD2))
			{
				CreateBullet();
			}

#pragma endregion
		}

		virtual void OnExit()
		{
			// 메모리 풀 상태 Reset
			_player->ResetRushBulletPrefabIndex();

			Muscle::CTime::SetGameSpeed(1.f, 0.f);

			this->_character.lock()->GetRushButtonUI()->GetGameObject()->SetIsEnable(false);

			// 느려진 속도 복구
			GetSoundManager()->SetSoundSpeed(Muscle::CTime::GetGameSpeed(), IPlayMode::Effect);
			GetSoundManager()->SetSoundSpeed(Muscle::CTime::GetGameSpeed(), IPlayMode::BGM);

			// 모터 종료
			Muscle::XPad::Get()->StopVibration();
		}
	};

	class Die : public IState<Player>
	{
	public:
		virtual void OnEnter(std::shared_ptr<Player> player)
		{
			player->GetAnimater()->SetAnimSpeed(PLAYER_AnimSpeed, 10000.0f);

			this->_character = player;
			this->_character.lock()->GetAnimater()->Play("Ch_Dead");

			// 죽을 때는 선택을 취소한다.
			TileManager::Get()->SelectCancel();

			// 죽기 직전의 씬 이름을 저장 (For Retry)
			DieScene::SetRetrySceneName(Muscle::IGameEngine::Get()->GetSceneManager()->GetCurrentSceneName());
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