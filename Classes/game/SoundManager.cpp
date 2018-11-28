#include  <io.h>
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


void SoundManager::playBuildingEffect(BuildingSoundEffectType type)
{
	if (m_buildingSoundEffectData == nullptr)
	{
		m_buildingSoundEffectData = GameConfig::getInstance()->getBuildingSoundEffectConf();
	}
	if (m_buildingSoundEffectData == nullptr)
	{
		return;
	}
	if (type == BuildingSoundEffectType::Construct)
	{
		
		if (canPlay(m_buildingSoundEffectData->constructName))
		{
			cocos2d::experimental::AudioEngine::play2d(m_buildingSoundEffectData->constructName, false, m_effectVolume);
		}
	}
	else
	{
		if (canPlay(m_buildingSoundEffectData->destroyedName))
		{
			cocos2d::experimental::AudioEngine::play2d(m_buildingSoundEffectData->destroyedName, false, m_effectVolume);
		}
	}
}


bool SoundManager::canPlay(const std::string& fileName)
{
	bool result = false;

	auto fullPath = cocos2d::FileUtils::getInstance()->fullPathForFilename(fileName);
	if (_access(fullPath.c_str(), 0) != -1)
	{
		result = true;
	}

	return result;
}