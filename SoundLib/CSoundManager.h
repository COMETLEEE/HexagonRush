#pragma once

#include "ISoundManager.h"
#include "fmod.hpp"
#include <map>
#define Effect_Sound_Count 5
/// <summary>
/// ����Ŵ���
/// ������ FMOD�� wrapping�ؼ�, �ε�� ��� ������ �����Ѵ�.
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
	/// ä���� �ϳ��� �θ� BGM�� ��ġ�� ���װ� �־ ä���� �ΰ��� ������ ���
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
	// ��� ���带 �̸��� Ű�� ����
	std::map<std::string, FMOD::Sound*> m_SoundList;
	
private:
	// FMOD�� System�� Update�ϱ� ���ؼ�
	// ���������� �𸣳� �� ������ ������Ʈ�� �ϸ� ���ø�����Ʈ��
	// ������ �� ó�� ���尡 ������.
	float m_ElapsedTime;
};
