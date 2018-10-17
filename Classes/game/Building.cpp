#include "Building.h"
#include "GameConfig.h"
#include "MapManager.h"
#include "GameUtils.h"


Building* Building::create(BuildingType type, const cocos2d::Vec2& position)
{
	auto building = new Building();
	if (building && building->init(type, position))
	{
		building->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(building);
	}

	return building;
}

bool Building::init(const BuildingType type, const cocos2d::Vec2& position)
{
	if (!GameObject::init())
	{
		return false;
	}
	
	m_buildingStatus = BuildingStatus::Working;
	setPosition(position);
	initBuildingStatusSprites(type);
	initBottomGridSprites(type);

	m_uniqId = GameUtils::getLastestUniqId();

	return true;
}

void Building::initBuildingStatusSprites(const BuildingType type)
{
	createBuildingStatusSprite(type, BuildingStatus::PrepareToBuild, 180);
	createBuildingStatusSprite(type, BuildingStatus::BeingBuilt);
	createBuildingStatusSprite(type, BuildingStatus::Working);
	createBuildingStatusSprite(type, BuildingStatus::Destory);
}



cocos2d::Sprite* Building::createBuildingStatusSprite(const BuildingType type, BuildingStatus buildingStatus, int opacity /* = 255 */)
{
	auto buildingConf = GameConfig::getInstance()->getBuildingConf(type);
	if (buildingConf == nullptr)
	{
		return nullptr;
	}
	float shadowYPosition = 0.0f;
	std::string spriteTextureName;
	bool hasShadow = false;

	switch (buildingStatus)
	{
	case BuildingStatus::PrepareToBuild:
		spriteTextureName = buildingConf->prepareToBuildStatusTextureName;
		break;
	case BuildingStatus::BeingBuilt:
		shadowYPosition = buildingConf->shadowYPositionInBeingBuiltStatus;
		spriteTextureName = buildingConf->beingBuiltStatusTextureName;
		hasShadow = true;
		break;
	case BuildingStatus::Working:
		shadowYPosition = buildingConf->shadowYPositionInWorkingStatus;
		spriteTextureName = buildingConf->workingStatusTextureName.c_str();
		hasShadow = true;
		break;
	case BuildingStatus::Destory:
		shadowYPosition = buildingConf->shadowYPositionInDestroyStatus;
		spriteTextureName = buildingConf->destroyStatusTextureName;
		hasShadow = true;
		break;
	default:    
		break;
	}

	auto buildingStatusSpriteFrame = cocos2d::SpriteFrameCache::getInstance()->getSpriteFrameByName(spriteTextureName);
	auto buildingStatusSprite = Sprite::create();
	//Sprite::createWithSpriteFrameName();
	buildingStatusSprite->setSpriteFrame(buildingStatusSpriteFrame);
	buildingStatusSprite->setOpacity(opacity);
	this->addChild(buildingStatusSprite);
	m_buildingStatusSpriteMap[buildingStatus] = buildingStatusSprite;

	if (hasShadow)
	{
		auto buildStatusSpriteSize = buildingStatusSprite->getContentSize();

		auto shadowSprite = Sprite::create();
		auto shadowSpriteFrame = cocos2d::SpriteFrameCache::getInstance()->getSpriteFrameByName(buildingConf->shadowTextureName);
		shadowSprite->setSpriteFrame(shadowSpriteFrame);
		shadowSprite->setPosition(cocos2d::Vec2(buildStatusSpriteSize.width / 2.0f, shadowYPosition));
		buildingStatusSprite->addChild(shadowSprite, -1);
	}

	return buildingStatusSprite;
}


void Building::initBottomGridSprites(const BuildingType type)
{
	auto buildingConf = GameConfig::getInstance()->getBuildingConf(type);
	if (buildingConf == nullptr)
	{
		return ;
	}

	auto& tileSize = MapManager::getInstance()->getTileSize();

	auto prepareToBuildSprite = m_buildingStatusSpriteMap[BuildingStatus::PrepareToBuild];
	auto prepareToBuildSpriteSize = prepareToBuildSprite->getContentSize();

	cocos2d::Vec2 centerBottomGridPosition(prepareToBuildSprite->getContentSize().width / 2.0f, buildingConf->centerBottomGridYPosition);
	cocos2d::Vec2 firstQueryGridPosition(centerBottomGridPosition.x, centerBottomGridPosition.y + tileSize.height);
	m_bottomGridsPlaneCenterPositionInLocalSpace = centerBottomGridPosition;

	for (int i = 0; i < buildingConf->bottomGridRowCount; ++i)
	{
		cocos2d::Vec2 tempGridPostion;
		tempGridPostion.x = firstQueryGridPosition.x - i * tileSize.width / 2.0f;
		tempGridPostion.y = firstQueryGridPosition.y - i * tileSize.height / 2.0f;

		for (int j = 0; j < buildingConf->bottomGridColumnCount; ++j)
		{
			auto gridSprite = Sprite::create();
			auto gridSpriteFrame = cocos2d::SpriteFrameCache::getInstance()->getSpriteFrameByName(ENABLE_BUILD_GRID_FILE_NAME);
			gridSprite->setSpriteFrame(gridSpriteFrame);
			gridSprite->setPosition(tempGridPostion);
			prepareToBuildSprite->addChild(gridSprite, -1);

			m_bottomGridSpritesList.push_back(gridSprite);

			tempGridPostion.x += tileSize.width / 2.0f;
			tempGridPostion.y -= tileSize.height / 2.0f;
		}
	}
}

const cocos2d::Size& Building::getContentSize()
{
	auto& prepareToBuildSprite = m_buildingStatusSpriteMap[BuildingStatus::PrepareToBuild];
	return prepareToBuildSprite->getContentSize();
}