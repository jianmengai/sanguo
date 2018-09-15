#include "DataManager.h"
#include "GameSceneManager.h"
#include "UIDefine.h"

enum DATA_TYPE
{
	DATA_TYPE_MAP,
	DATA_TYPE_UI_COMMON_SUDOKU_1
};

DataManager::DataManager()
{

}

DataManager::~DataManager()
{
}

void DataManager::loadRes()
{
	m_waitInitDataQueue.push(DATA_TYPE_MAP);
	m_waitInitDataQueue.push(DATA_TYPE_UI_COMMON_SUDOKU_1);
	GameSceneManager::getInstance().getLoadingLayer()->addTotalSize(m_waitInitDataQueue.size());
}

bool DataManager::loadResTick(float dt)
{
	static int frame1 = 0;
	static int frame2 = 0;
	if (m_waitInitDataQueue.size() == 0)
	{
		++frame1;
		if (frame1 >= 5)
		{
			frame1 = 0;
			return true;
		}
		return false;
	}
	++frame2;
	if (frame2 < 2)
	{
		return false;
	}
	frame2 = 0;

	int dataType = m_waitInitDataQueue.front();
	switch (dataType)
	{
		case DATA_TYPE_MAP:
		{
			//donothing
		}
		break;
		case DATA_TYPE_UI_COMMON_SUDOKU_1:
		{
			PlistManager::getInstance().addUISpriteFrames(UI_COMMON_SUDOKU_1_PLIST);
			break;
		}
	}
	GameSceneManager::getInstance().getLoadingLayer()->addCurreSize(1);
	m_waitInitDataQueue.pop();
	return false;
}