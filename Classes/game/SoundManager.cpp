#include "SoundManager.h"

SoundManager* SoundManager::getInstance()
{
	static SoundManager soundManagerInstance;
	return &soundManagerInstance;
}

void SoundManager::playUIEffect(UIEffectType type)
{

}
