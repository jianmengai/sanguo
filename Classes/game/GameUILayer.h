#pragma once
#include "cocos2d.h"
#include "MsgDisplay.h"

class GameUILayer : cocos2d::Layer
{
public:
	GameUILayer();
	~GameUILayer();
	CC_SYNTHESIZE_READONLY(MsgDisplay*, _msgDisplay, MsgDisplay);
};
