#pragma once
#include "GameDefine.h"

class SoundManager
{
public:
	static SoundManager* getInstance();
	void playUIEffect(UIEffectType type);
};