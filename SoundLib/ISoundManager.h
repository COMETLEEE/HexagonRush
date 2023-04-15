#pragma once
#include <string>

/// <summary>
/// 사운드 매니저를 추상화
/// 
/// 2022.02.19
/// </summary>
enum class IPlayMode
{
	BGM,
	Effect
};
class ISoundManager abstract
{
public:
	virtual ~ISoundManager() {};
public:
	virtual void Initialize() abstract;
	virtual void LoadSoundFile(std::string name, std::string filePath, unsigned int mode) abstract;

	virtual void Update() abstract;
	virtual void Play(std::string name, IPlayMode mode) abstract;
	virtual void Delay(float delayTime, IPlayMode mode) abstract;

	virtual void Pause(IPlayMode mode, bool isPause) abstract;

	virtual void SetSoundSpeed(float speed, IPlayMode mode) abstract;
	virtual void SetSoundVoulume(float volume, IPlayMode mode) abstract;
	virtual void SetEffectSoundVoulume(float volume) abstract;

	virtual void Finalize() abstract;

};
