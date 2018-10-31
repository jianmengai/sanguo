#include "cocostudio/ActionTimeline/CSLoader.h"
#include "GameUILayer.h"
#include "MapManager.h"
#include "GameObjectManager.h"
#include "SoundManager.h"
#include "GameConfig.h"
#include "GameBattle.h"
#include "FixUI.h"
#include "WarFogLayer.h"

GameUILayer::GameUILayer()
{

}

GameUILayer::~GameUILayer()
{

}

bool GameUILayer::init()
{
	if (!cocos2d::Node::init())
	{
		return false;
	}
	m_gameUI = cocos2d::CSLoader::createNode("ui/MainScene.csb");
	auto designSize = m_gameUI->getContentSize();
	/*FixUI::init(designSize);
	FixUI::setFixScale(m_gameUI);
	FixUI::fixScene(m_gameUI);*/
	this->addChild(m_gameUI);
	m_gameStartTime = time(nullptr);


	initMiniMap();
	initCreateButton();

	auto dispatcher = cocos2d::Director::getInstance()->getEventDispatcher();
	auto touchOneByOneListener = cocos2d::EventListenerTouchOneByOne::create();
	//touchOneByOneListener->setSwallowTouches(true);
	touchOneByOneListener->onTouchBegan = CC_CALLBACK_2(GameUILayer::onTouchBegan, this);
	touchOneByOneListener->onTouchMoved = CC_CALLBACK_2(GameUILayer::onTouchMoved, this);
	touchOneByOneListener->onTouchEnded = CC_CALLBACK_2(GameUILayer::onTouchEnded, this);
	dispatcher->addEventListenerWithSceneGraphPriority(touchOneByOneListener, this);

	scheduleUpdate();

	return true;
}

bool GameUILayer::initMiniMap()
{
	auto gameMainPanel = m_gameUI->getChildByName("Panel_Creating");
	if (gameMainPanel == nullptr)
	{
		return false;
	}
	m_miniMapImgView = gameMainPanel->getChildByName<cocos2d::ui::ImageView*>("Image_MiniMap");
	m_miniMapDrawNode = cocos2d::DrawNode::create();
	m_miniMapImgView->addChild(m_miniMapDrawNode);
	m_miniMapImgView->addTouchEventListener(CC_CALLBACK_2(GameUILayer::onMinimapTouched, this));
}

bool GameUILayer::initCreateButton()
{
	auto gameMainPanel = m_gameUI->getChildByName("Panel_Creating");
	auto createBuildingPanel = gameMainPanel->getChildByName("Panel_CreateBuilding");
	auto createSoldierPanel = gameMainPanel->getChildByName("Panel_CreateSoldier");
	//主城
	auto createMainTown = createBuildingPanel->getChildByName<cocos2d::ui::Button*>("Button_Town");
	createMainTown->addTouchEventListener(CC_CALLBACK_2(GameUILayer::onCreateObject, this));
	m_createCallback[createMainTown] = CC_CALLBACK_0(GameUILayer::createBuilding, this, BuildingType::MainTown);
	//兵营
	auto createBarrack = createBuildingPanel->getChildByName<cocos2d::ui::Button*>("Button_Barrack");
	createBarrack->addTouchEventListener(CC_CALLBACK_2(GameUILayer::onCreateObject, this));
	m_createCallback[createBarrack] = CC_CALLBACK_0(GameUILayer::createBuilding, this, BuildingType::Barrack);
	//箭塔
	auto createDefenceTower = createBuildingPanel->getChildByName<cocos2d::ui::Button*>("Button_DefenceTower");
	createDefenceTower->addTouchEventListener(CC_CALLBACK_2(GameUILayer::onCreateObject, this));
	m_createCallback[createDefenceTower] = CC_CALLBACK_0(GameUILayer::createBuilding, this, BuildingType::DefenceTower);
	//步兵
	auto createInfantry = createSoldierPanel->getChildByName<cocos2d::ui::Button*>("Button_Infantry");
	createInfantry->addTouchEventListener(CC_CALLBACK_2(GameUILayer::onCreateObject, this));
	m_createCallback[createInfantry] = CC_CALLBACK_0(GameUILayer::createSoldier, this, SoldierType::Infantry);
	//骑兵
	auto createCavalry = createSoldierPanel->getChildByName<cocos2d::ui::Button*>("Button_Cavalry");
	createCavalry->addTouchEventListener(CC_CALLBACK_2(GameUILayer::onCreateObject, this));
	m_createCallback[createCavalry] = CC_CALLBACK_0(GameUILayer::createSoldier, this, SoldierType::Cavalry);
	//弓箭手
	auto createArcher = createSoldierPanel->getChildByName<cocos2d::ui::Button*>("Button_Archer");
	createArcher->addTouchEventListener(CC_CALLBACK_2(GameUILayer::onCreateObject, this));
	m_createCallback[createArcher] = CC_CALLBACK_0(GameUILayer::createSoldier, this, SoldierType::Archer);

	return true;
}

void GameUILayer::onMinimapTouched(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType touchType)
{
	cocos2d::log("minimap touched");
	if (touchType == cocos2d::ui::Widget::TouchEventType::MOVED)
	{
		
		auto widget = static_cast<cocos2d::ui::Widget*>(sender);
		auto touchStart = widget->convertToNodeSpace(widget->getTouchBeganPosition());
		auto touchMove = widget->convertToNodeSpace(widget->getTouchMovePosition());
		auto deltaPos = touchMove - touchStart;
		deltaPos.negate();
		/*auto touchPosition = widget->convertToNodeSpace(widget->getTouchEndPosition());
		auto widgetSize = widget->getContentSize();

		float mapScale = MapManager::getInstance()->getMapScale();
		auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();

		cocos2d::Vec2 tileMapNewPosition(touchPosition.x / widgetSize.width * MapManager::getInstance()->getTileSize().width * mapScale - visibleSize.width / 2.0f,
			touchPosition.y / widgetSize.height * MapManager::getInstance()->getTileSize().height * mapScale - visibleSize.height / 2.0f);
*/
		MapManager::getInstance()->setPosition(deltaPos, true);
		WarFogLayer::getInstance()->setPosition(deltaPos);

		if (cocos2d::Director::getInstance()->isPaused())
		{
			updateMiniMap();
		}
	}
}

void GameUILayer::update(float dt)
{
	updateMiniMap();
	updateGameTime();
	//updateButtonStatus();
}

void GameUILayer::updateMiniMap()
{
	m_miniMapDrawNode->clear();
	auto mapPosition = MapManager::getInstance()->getPosition();
	auto mapScale = MapManager::getInstance()->getMapScale();
	auto visibileSize = cocos2d::Director::getInstance()->getVisibleSize();
	auto mapSize = MapManager::getInstance()->getContentSize();

	auto& tileMapSize = MapManager::getInstance()->getTileSize();
	auto& miniMapSize = m_miniMapImgView->getContentSize();
	auto& gameObjectMap = GameObjectManager::getInstance()->getGameObjectMap();
	for (auto& gameObjectIter : gameObjectMap)
	{
		cocos2d::Color4F color;
		auto gameObject = gameObjectIter.second;
		if (gameObject->getForceType() == ForceType::Player)
		{
			if (gameObject->isSelected())
			{
				color = cocos2d::Color4F(1.0f, 1.0f, 1.0f, 1.0f);
			}
			else
			{
				color = cocos2d::Color4F(248.0f / 255.0f, 200.0f / 255.0f, 40.0f / 255.0f, 1.0f);
			}
		}
		else
		{
			color = cocos2d::Color4F(164.0f / 255.0f, 72.0f / 255.0f, 192.0f / 255.0f, 1.0f);
		}
		//物体位置坐标是地图中的坐标位置，这里相对地图而言是没有缩放的
		auto gameObjectInTileMapPosition = gameObject->getPosition();
		auto gameObjectInMinimapPosition = cocos2d::Vec2(gameObjectInTileMapPosition.x * miniMapSize.width / mapSize.width,
			gameObjectInTileMapPosition.y * miniMapSize.height / mapSize.height);
		if (gameObject->getGameObjectType() == GameObjectType::Building)
		{
			m_miniMapDrawNode->drawSolidRect(cocos2d::Vec2(gameObjectInMinimapPosition.x - 2.0f, gameObjectInMinimapPosition.y - 2.0f),
				cocos2d::Vec2(gameObjectInMinimapPosition.x + 2.0f, gameObjectInMinimapPosition.y + 2.0f),
				color
			);
		}
		else if (gameObject->getGameObjectType() == GameObjectType::Soldier)
		{
			m_miniMapDrawNode->drawDot(gameObjectInMinimapPosition, 1.0f, color);
		}
	}

	//屏幕展示的是缩放后的地图，这里计算位置的时候要考虑缩放
	cocos2d::Size minimapScreenBoxSize(visibileSize.width * miniMapSize.width / (mapSize.width * mapScale),
		visibileSize.height * miniMapSize.height / (mapSize.height * mapScale));

	cocos2d::Vec2 minimapScreenBoxPosition(-mapPosition.x * miniMapSize.width / (mapSize.width * mapScale),
		-mapPosition.y  * miniMapSize.height / (mapSize.height * mapScale));

	m_miniMapDrawNode->drawRect(minimapScreenBoxPosition,
		minimapScreenBoxPosition + minimapScreenBoxSize,
		cocos2d::Color4F(1.0f, 1.0f, 1.0f, 1.0f));
}

void GameUILayer::updateGameTime()
{
	time_t now = time(nullptr);
	time_t passTime = now - m_gameStartTime;
	tm* passTimeTm = gmtime(&passTime);
	auto gameMainPanel = m_gameUI->getChildByName("Panel_Creating");
	auto passTimeLabel = gameMainPanel->getChildByName<cocos2d::ui::Text*>("Text_GameTime");
	passTimeLabel->setString(cocos2d::StringUtils::format("%02d:%02d:%02d", passTimeTm->tm_hour, passTimeTm->tm_min, passTimeTm->tm_sec));
}

void GameUILayer::updateButtonStatus()
{

}

void GameUILayer::onCreateObject(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType touchType)
{
	if (touchType == cocos2d::ui::Widget::TouchEventType::ENDED)
	{
		auto it = m_createCallback.find(sender);
		if (it != m_createCallback.end())
		{
			if (it->second())
			{
				SoundManager::getInstance()->playUIEffect(UIEffectType::ButtonClick);
			}
		}
		
	}
}

bool GameUILayer::createBuilding(BuildingType type)
{
	cocos2d::log("create building, type:%d", type);
	
	return GameBattle::getInstance()->createBuilding(ForceType::Player, type, m_touchPos);
}

bool GameUILayer::createSoldier(SoldierType type)
{
	cocos2d::log("create soldier, type:%d", type);
	return GameBattle::getInstance()->createSoldier(ForceType::Player, type);
}

bool GameUILayer::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event)
{
	m_touchPos = touch->getLocation();
	//cocos2d::log("GameUILayer::onTouchBegan, x:%0.1f, y:%0.1f", m_touchPos.x, m_touchPos.y);
	return true;
}

void GameUILayer::onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event)
{

}

void GameUILayer::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event)
{

}


