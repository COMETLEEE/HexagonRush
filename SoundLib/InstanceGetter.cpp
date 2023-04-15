#include "InstanceGetter.h"
#include "CSoundManager.h"

ISoundManager* g_newSoundManagerInstance = nullptr;

ISoundManager* GetSoundManager()
{
	if (g_newSoundManagerInstance == nullptr)
	{
		g_newSoundManagerInstance = new CSoundManager();
	}

	return g_newSoundManagerInstance;
}
