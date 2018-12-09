#pragma once
#include "GameDefine.h"
#include "GameConfig.h"

class SoundManager
{
public:
	static SoundManager* getInstance();
	void playUIEffect(UIEffectType type);
	int playBuildingEffect(BuildingSoundEffectType type, bool loop = false);
	void playNpcEffect(SoldierType soldierType, NpcSoundEffectType type);
	void stop(int audioId);
private:
	bool canPlay(const std::string& fileName);
private:
	BuildingSoundEffectData* m_buildingSoundEffectData = nullptr;
	SoldierSoundEffectData* m_soldierSoundEffectData;
	float m_effectVolume = 1.0f;
};