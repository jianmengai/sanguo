#include "cocos2d.h"
#include "SoundManager.h"
#include "audio/include/AudioEngine.h"

SoundManager* SoundManager::getInstance()
{
	static SoundManager soundManagerInstance;
	return &soundManagerInstance;
}

void SoundManager::playUIEffect(UIEffectType type)
{

}


int SoundManager::playBuildingEffect(BuildingSoundEffectType type, bool loop)
{
	if (m_buildingSoundEffectData == nullptr)
	{
		m_buildingSoundEffectData = GameConfig::getInstance()->getBuildingSoundEffectConf();
	}
	if (m_buildingSoundEffectData == nullptr)
	{
		return 0;
	}
	if (type == BuildingSoundEffectType::Construct)
	{
		
		if (canPlay(m_buildingSoundEffectData->constructName))
		{
			return cocos2d::experimental::AudioEngine::play2d(m_buildingSoundEffectData->constructName, loop, m_effectVolume);
		}
	}
	else
	{
		if (canPlay(m_buildingSoundEffectData->destroyedName))
		{
			return cocos2d::experimental::AudioEngine::play2d(m_buildingSoundEffectData->destroyedName, loop, m_effectVolume);
		}
	}
}

void SoundManager::playNpcEffect(SoldierType soldierType, NpcSoundEffectType type)
{
	m_soldierSoundEffectData = GameConfig::getInstance()->getSoldierSoundEffectConf(soldierType);
	if (m_soldierSoundEffectData == nullptr)
	{
		return;
	}
	std::string effectName;
	switch (type)
	{
	case NpcSoundEffectType::Attack:
	{
		effectName = m_soldierSoundEffectData->attackName;
	}
	break;
	case NpcSoundEffectType::Death:
	{
		effectName = m_soldierSoundEffectData->deathName;
	}
	break;
	case NpcSoundEffectType::Move:
	{
		effectName = m_soldierSoundEffectData->moveName;
	}
	break;
	default:
		break;
	}
	if ((!effectName.empty()) && canPlay(effectName))
	{
		cocos2d::experimental::AudioEngine::play2d(effectName, false, m_effectVolume);
	}
}

void SoundManager::stop(int audioId)
{
	cocos2d::experimental::AudioEngine::stop(audioId);
}


bool SoundManager::canPlay(const std::string& fileName)
{
	bool result = false;

	auto fullPath = cocos2d::FileUtils::getInstance()->fullPathForFilename(fileName);
	if (cocos2d::FileUtils::getInstance()->isFileExist(fullPath))
	{
		result = true;
	}

	return result;
}