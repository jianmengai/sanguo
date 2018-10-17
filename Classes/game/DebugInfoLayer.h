#pragma once

#include "cocos2d.h"

class DebugInfoLayer : public cocos2d::Layer
{
public:
	CREATE_FUNC(DebugInfoLayer);

	void drawTileTable();
private:
	bool init() override;

private:
	cocos2d::DrawNode* m_tileTableDrawNode = nullptr;
};