#include "DebugInfoLayer.h"
#include "Mapmanager.h"

bool DebugInfoLayer::init()
{
	if (!cocos2d::Layer::init())
	{
		return false;
	}
	m_tileTableDrawNode = cocos2d::DrawNode::create();
}

void DebugInfoLayer::drawTileTable()
{
	auto tileNodeTable = MapManager::getInstance()->getTileNodeTable();
	for (auto )
	{

	}
}