#include "SoundManager.h"

SoundManager* SoundManager::getInstance()
{
	static SoundManager soundManagerInstance;
	return &soundManagerInstance;
}

void SoundManager::playUIEffect(UIEffectType type)
{

}


void SoundManager::playBuildingEffect(BuildingSoundEffectType type)
{
	if (type == BuildingSoundEffectType::Construct)
	{
		if (canPlay(_buildingSoundEffectData.constructName))
		{
			AudioEngine::play2d(_buildingSoundEffectData.constructName, false, _effectVolume);
		}
	}
	else
	{
		if (canPlay(_buildingSoundEffectData.destroyedName))
		{
			AudioEngine::play2d(_buildingSoundEffectData.destroyedName, false, _effectVolume);
		}
	}
}