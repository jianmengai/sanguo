#pragma once
#include <string>
#include <unordered_map>
#include "GameObject.h"



class Building : public GameObject
{
public:
	static Building* create(ForceType forceType, BuildingType type , const cocos2d::Vec2& position);
	const cocos2d::Size& getContentSize();
	BuildingType getBuildingType();
	std::vector<cocos2d::Vec2>& getBottonGirdPos();
	
	void update(float deltaTime);

private:
	bool init(ForceType forceType, const BuildingType type, const cocos2d::Vec2& position);
	cocos2d::Sprite* Building::createBuildingStatusSprite(const BuildingType type, BuildingStatus buildingStatus, int opacity = 255);
	void initBuildingStatusSprites(const BuildingType type);
	void initBottomGridSprites(const BuildingType type);
	void initData(const BuildingType type);

	bool isReadyToRemove();
	void onPrepareToRemove();
	void initHpBar();

	void updateStatus(BuildingStatus buildingStatus);
	void addToRemoveQueue();

private:
	BuildingStatus m_buildingStatus = BuildingStatus::Invalid;
	std::unordered_map<BuildingStatus, cocos2d::Sprite*> m_buildingStatusSpriteMap;
	cocos2d::Vec2 m_bottomGridsPlaneCenterPositionInLocalSpace;
	std::vector<cocos2d::Sprite*> m_bottomGridSpritesList;
	BuildingType m_buildingType = BuildingType::Invalid;
	std::vector<cocos2d::Vec2> m_bottonGridPos;
};