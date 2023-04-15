#pragma once

#include "ISoundManager.h"
#include "fmod.hpp"
#include <map>
#define Effect_Sound_Count 5
/// <summary>
/// 사운드매니저
/// 간단히 FMOD를 wrapping해서, 로드와 출력 정도를 수행한다.
/// 
/// 2022.02.18 LeHideHome
/// </summary>
class CSoundManager : public ISoundManager
{
public:
	virtual void Initialize() override;
	virtual void LoadSoundFile(std::string name, std::string filePath, unsigned int mode) override;

	virtual void Update() override;
	virtual void Play(std::string name, IPlayMode mode) override;
	virtual void Delay(float delayTime, IPlayMode mode) override;

	virtual void Pause(IPlayMode mode, bool isPause) override;

	/// <summary>
	/// 채널은 하나로 두면 BGM이 겹치는 버그가 있어서 채널을 두개로 나눠서 재생
	/// </summary>
	void PlayBGM(std::string name);
	void PlayEffect(std::string name);

	virtual void SetSoundSpeed(float speed, IPlayMode mode);

	virtual void SetSoundVoulume(float volume, IPlayMode mode);
	virtual void SetEffectSoundVoulume(float volume);

	virtual void Finalize() override;

private:
	FMOD::System* system;
	FMOD::Channel* bgmChannel = 0;
	FMOD::Channel* effectChannel[Effect_Sound_Count] = {0,0,0,0,0};
	FMOD_RESULT result;
	void* extradriverdata = 0;

	float delayTime = 1;
	float m_EffectSoundVolume = 1.f;
	// 모든 사운드를 이름을 키로 보관
	std::map<std::string, FMOD::Sound*> m_SoundList;
	
private:
	// FMOD의 System을 Update하기 위해서
	// 버그인지는 모르나 매 프레임 업데이트를 하면 샘플링레이트가
	// 낮아진 것 처럼 사운드가 깨진다.
	float m_ElapsedTime;
};

