#pragma once
#include "GameDefine.h"
#include "GameConfig.h"

class SoundManager
{
public:
	static SoundManager* getInstance();
	void playUIEffect(UIEffectType type);
	void playBuildingEffect(BuildingSoundEffectType type);

private:
	bool canPlay(const std::string& fileName);
private:
	BuildingSoundEffectData* m_buildingSoundEffectData = nullptr;
	float m_effectVolume = 1.0f;
};