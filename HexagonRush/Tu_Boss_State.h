#pragma once

#include "StageClearScene.h"
#include "Crosshiar_Prefab.h"

namespace Tu_BossState
{
	class Idle : public IState<Boss>
	{
	public:
		virtual void OnEnter(std::shared_ptr<Boss> boss)
		{
			this->_character = boss;
			this->_character.lock()->GetAnimater()->Play("boss_idle");
		}
		virtual void OnStay()
		{
			if (!this->_character.lock()->GetAnimater()->GetisPlay())
				this->_character.lock()->GetAnimater()->Play("boss_idle");
		}
		virtual void OnExit()
		{
			this->_character.lock()->GetAnimater()->Stop();
		}
	};

	class Attack1 : public IState<Boss>
	{
	public:
		virtual void OnEnter(std::shared_ptr<Boss> boss)
		{
			this->_character = boss;
			this->_character.lock()->GetAnimater()->Play("attack1_down");
			std::make_shared<Crosshiar_Prefab>()->GetGameObject()->GetTransform()->SetPosition(Vector3(9.3f, 4.0f, -4.5f));
		}
		virtual void OnStay()
		{
			if (!this->_character.lock()->GetAnimater()->GetisPlay())
				this->_character.lock()->SetState(Boss::IDLE);
		}
		virtual void OnExit()
		{
			this->_character.lock()->GetAnimater()->Stop();
		}
	};

	class Attack2 : public IState<Boss>
	{
	public:
		virtual void OnEnter(std::shared_ptr<Boss> boss)
		{
			this->_character = boss;
			this->_character.lock()->GetAnimater()->Play("attack2_down_slid");
			std::make_shared<Crosshiar_Prefab>()->GetGameObject()->GetTransform()->SetPosition(Vector3(9.3f, 4.0f, -4.5f));
		}
		virtual void OnStay()
		{
			if (!this->_character.lock()->GetAnimater()->GetisPlay())
				this->_character.lock()->SetState(Boss::IDLE);
		}
		virtual void OnExit()
		{
			this->_character.lock()->GetAnimater()->Stop();
		}
	};
	class Attack3 : public IState<Boss>
	{
	public:
		virtual void OnEnter(std::shared_ptr<Boss> boss)
		{
			this->_character = boss;
			this->_character.lock()->GetAnimater()->Play("attack3_shake");
			auto crosshair = std::make_shared<Crosshiar_Prefab>()->GetGameObject();
			crosshair->GetTransform()->SetPosition(Vector3(8.0f, 4.0f, 1.5f));
			crosshair->GetComponent<Crosshair>()->SetSize(1.5f);
		}
		virtual void OnStay()
		{
			if (!this->_character.lock()->GetAnimater()->GetisPlay())
				this->_character.lock()->SetState(Boss::IDLE);
		}
		virtual void OnExit()
		{
			this->_character.lock()->GetAnimater()->Stop();
		}
	};
	class Attack4 : public IState<Boss>
	{
	public:
		virtual void OnEnter(std::shared_ptr<Boss> boss)
		{
			this->_character = boss;
			this->_character.lock()->GetAnimater()->Play("attack4_missile");
		}
		virtual void OnStay()
		{
			if (!this->_character.lock()->GetAnimater()->GetisPlay())
				this->_character.lock()->SetState(Boss::IDLE);
		}
		virtual void OnExit()
		{
			this->_character.lock()->GetAnimater()->Stop();
		}
	};

	class Attack6 : public IState<Boss>
	{
	public:
		virtual void OnEnter(std::shared_ptr<Boss> boss)
		{
			this->_character = boss;
			this->_character.lock()->GetAnimater()->Play("attack6_laser");
		}
		virtual void OnStay()
		{
			if (!this->_character.lock()->GetAnimater()->GetisPlay())
				this->_character.lock()->SetState(Boss::IDLE);
		}
		virtual void OnExit()
		{
			this->_character.lock()->GetAnimater()->Stop();
		}
	};
	class Attack7 : public IState<Boss>
	{
	public:
		virtual void OnEnter(std::shared_ptr<Boss> boss)
		{
			this->_character = boss;
			this->_character.lock()->GetAnimater()->Play("attack7_fire_release");
		}
		virtual void OnStay()
		{
			if (!this->_character.lock()->GetAnimater()->GetisPlay())
				this->_character.lock()->SetState(Boss::IDLE);
		}
		virtual void OnExit()
		{
			this->_character.lock()->GetAnimater()->Stop();
		}
	};
	class Attack8 : public IState<Boss>
	{
	public:
		virtual void OnEnter(std::shared_ptr<Boss> boss)
		{
			this->_character = boss;
			this->_character.lock()->GetAnimater()->Play("attack8_fire_radiate");
		}
		virtual void OnStay()
		{
			if (!this->_character.lock()->GetAnimater()->GetisPlay())
				this->_character.lock()->SetState(Boss::IDLE);
		}
		virtual void OnExit()
		{
			this->_character.lock()->GetAnimater()->Stop();
		}
	};
	class Attack9 : public IState<Boss>
	{
	public:
		virtual void OnEnter(std::shared_ptr<Boss> boss)
		{
			this->_character = boss;
			this->_character.lock()->GetAnimater()->Play("attack9_down_right");
			std::make_shared<Crosshiar_Prefab>()->GetGameObject()->GetTransform()->SetPosition(Vector3(5.0f, 4.0f, 2.0f));
		}
		virtual void OnStay()
		{
			if (!this->_character.lock()->GetAnimater()->GetisPlay())
				this->_character.lock()->SetState(Boss::IDLE);
		}
		virtual void OnExit()
		{
			this->_character.lock()->GetAnimater()->Stop();
		}
	};

	class Attack10 : public IState<Boss>
	{
	public:
		virtual void OnEnter(std::shared_ptr<Boss> boss)
		{
			this->_character = boss;
			this->_character.lock()->GetAnimater()->Play("attack10_down_slid_right");
			std::make_shared<Crosshiar_Prefab>()->GetGameObject()->GetTransform()->SetPosition(Vector3(5.0f, 4.0f, 2.0f));
		}
		virtual void OnStay()
		{
			if (!this->_character.lock()->GetAnimater()->GetisPlay())
				this->_character.lock()->SetState(Boss::IDLE);
		}
		virtual void OnExit()
		{
			this->_character.lock()->GetAnimater()->Stop();
		}
	};

	class Hit : public IState<Boss>
	{
	public:
		virtual void OnEnter(std::shared_ptr<Boss> boss)
		{
			this->_character = boss;
			this->_character.lock()->GetAnimater()->Play("boss_die");
			
			// 원래는 애니메이션 없이 바로 넘어가야 하는건가..? (이혜성)
			// this->_character.lock()->SetState(Boss::IDLE);
		}

		virtual void OnStay()
		{
			if (!this->_character.lock()->GetAnimater()->GetisPlay())
				this->_character.lock()->SetState(Boss::IDLE);
		}

		virtual void OnExit()
		{
			this->_character.lock()->GetAnimater()->Stop();
		}
	};

	class Die : public IState<Boss>
	{
	public:
		virtual void OnEnter(std::shared_ptr<Boss> boss)
		{
			this->_character = boss;
			this->_character.lock()->GetAnimater()->Play("boss_die");


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