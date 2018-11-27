#pragma once
#include <string>
#include <unordered_map>
#include "ui/UILoadingBar.h"
#include "GameObject.h"



class Building : public GameObject
{
public:
	static Building* create(ForceType forceType, BuildingType type , const cocos2d::Vec2& position);
	const cocos2d::Size& getContentSize();
	BuildingType getBuildingType();
	BuildingStatus getBuildingStatus();
	std::vector<cocos2d::Vec2>& getBottonGirdPos();
	
	void update(float deltaTime);

	bool isReadyToRemove();

private:
	bool init(ForceType forceType, const BuildingType type, const cocos2d::Vec2& position);
	cocos2d::Sprite* Building::createBuildingStatusSprite(const BuildingType type, BuildingStatus buildingStatus, int opacity = 255);
	void initBuildingStatusSprites(const BuildingType type);
	void initBottomGridSprites(const BuildingType type);
	void initData(const BuildingType type);

	
	void onPrepareToRemove();
	void initHpBar();

	void initBeingBuiltProgressBar();
	void showBeingBuiltProgressBar();
	void hideBeingBuiltProgressBar();
	void updateBeingBuiltProgressBar(float deltaTime);

	void onConstructionComplete();
	void updateStatus(BuildingStatus buildingStatus);
	void addToRemoveQueue();

private:
	BuildingStatus m_buildingStatus = BuildingStatus::Invalid;
	std::unordered_map<BuildingStatus, cocos2d::Sprite*> m_buildingStatusSpriteMap;
	cocos2d::Vec2 m_bottomGridsPlaneCenterPositionInLocalSpace;
	std::vector<cocos2d::Sprite*> m_bottomGridSpritesList;
	BuildingType m_buildingType = BuildingType::Invalid;
	std::vector<cocos2d::Vec2> m_bottonGridPos;
	cocos2d::ui::LoadingBar* m_beingBuildProgressBar = nullptr;
	float m_buildingTimeBySecond = 0;
	float m_passTimeBySecondInBeingBuiltStatus = 0;
};