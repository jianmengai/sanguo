#include "cocostudio/ActionTimeline/CSLoader.h"
#include "GameUILayer.h"
#include "MapManager.h"
#include "GameObjectManager.h"
#include "SoundManager.h"
#include "GameConfig.h"
#include "GameBattle.h"
#include "WarFogLayer.h"
#include "GameBattle.h"

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
	this->addChild(m_gameUI);
	m_gameStartTime = time(nullptr);

	initMiniMap();
	initCreateButton();

	initMediumMap();
	initTeamButton();

	auto dispatcher = cocos2d::Director::getInstance()->getEventDispatcher();
	auto touchOneByOneListener = cocos2d::EventListenerTouchOneByOne::create();
	//touchOneByOneListener->setSwallowTouches(true);
	touchOneByOneListener->onTouchBegan = CC_CALLBACK_2(GameUILayer::onTouchBegan, this);
	touchOneByOneListener->onTouchMoved = CC_CALLBACK_2(GameUILayer::onTouchMoved, this);
	touchOneByOneListener->onTouchEnded = CC_CALLBACK_2(GameUILayer::onTouchEnded, this);
	dispatcher->addEventListenerWithSceneGraphPriority(touchOneByOneListener, this);

	scheduleUpdate();

	cocos2d::log("gameui init done.");

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
	
	return true;
}

bool GameUILayer::initMediumMap()
{
	m_mediumMapImgView = m_gameUI->getChildByName<cocos2d::ui::ImageView*>("Image_MediumMap");
	if (m_mediumMapImgView == nullptr)
	{
		return false;
	}
	m_mediumMapDrawNode = cocos2d::DrawNode::create();
	m_mediumMapImgView->addChild(m_mediumMapDrawNode);
	m_mediumMapImgView->addTouchEventListener(CC_CALLBACK_2(GameUILayer::onMediumMapTouched, this));
	return true;
}

bool GameUILayer::initCreateButton()
{
	cocos2d::log("start init create button");
	auto gameMainPanel = m_gameUI->getChildByName("Panel_Creating");
	auto createBuildingPanel = gameMainPanel->getChildByName("Panel_CreateBuilding");
	auto createSoldierPanel = gameMainPanel->getChildByName("Panel_CreateSoldier");
	cocos2d::log("get button done");
	//����
	auto createMainTown = createBuildingPanel->getChildByName<cocos2d::ui::Button*>("Button_Town");
	createMainTown->addTouchEventListener(CC_CALLBACK_2(GameUILayer::onCreateObject, this));
	m_createCallback[createMainTown] = CC_CALLBACK_0(GameUILayer::createBuilding, this, BuildingType::MainTown);
	//��Ӫ
	auto createBarrack = createBuildingPanel->getChildByName<cocos2d::ui::Button*>("Button_Barrack");
	createBarrack->addTouchEventListener(CC_CALLBACK_2(GameUILayer::onCreateObject, this));
	m_createCallback[createBarrack] = CC_CALLBACK_0(GameUILayer::createBuilding, this, BuildingType::Barrack);
	//����
	auto createDefenceTower = createBuildingPanel->getChildByName<cocos2d::ui::Button*>("Button_DefenceTower");
	createDefenceTower->addTouchEventListener(CC_CALLBACK_2(GameUILayer::onCreateObject, this));
	m_createCallback[createDefenceTower] = CC_CALLBACK_0(GameUILayer::createBuilding, this, BuildingType::DefenceTower);
	//����
	auto createInfantry = createSoldierPanel->getChildByName<cocos2d::ui::Button*>("Button_Infantry");
	createInfantry->addTouchEventListener(CC_CALLBACK_2(GameUILayer::onCreateObject, this));
	m_createCallback[createInfantry] = CC_CALLBACK_0(GameUILayer::createSoldier, this, SoldierType::Infantry);
	//���
	auto createCavalry = createSoldierPanel->getChildByName<cocos2d::ui::Button*>("Button_Cavalry");
	createCavalry->addTouchEventListener(CC_CALLBACK_2(GameUILayer::onCreateObject, this));
	m_createCallback[createCavalry] = CC_CALLBACK_0(GameUILayer::createSoldier, this, SoldierType::Cavalry);
	//������
	auto createArcher = createSoldierPanel->getChildByName<cocos2d::ui::Button*>("Button_Archer");
	createArcher->addTouchEventListener(CC_CALLBACK_2(GameUILayer::onCreateObject, this));
	m_createCallback[createArcher] = CC_CALLBACK_0(GameUILayer::createSoldier, this, SoldierType::Archer);

	return true;
}

bool GameUILayer::initTeamButton()
{
	cocos2d::log("start init team button");
	auto teamPanel = m_gameUI->getChildByName("Panel_Team");
	
	/*
	auto teamOneButton = teamPanel->getChildByName<cocos2d::ui::Button*>("Button_TeamOne");
	teamOneButton->addTouchEventListener(CC_CALLBACK_2(GameUILayer::onTeam, this));
	m_createCallback[teamOneButton] = CC_CALLBACK_0(GameUILayer::teamOne, this);
	
	auto teamTwoButton = teamPanel->getChildByName<cocos2d::ui::Button*>("Button_TeamTwo");
	teamTwoButton->addTouchEventListener(CC_CALLBACK_2(GameUILayer::onTeam, this));
	m_createCallback[teamTwoButton] = CC_CALLBACK_0(GameUILayer::teamTwo, this);
	
	auto teamThreeButton = teamPanel->getChildByName<cocos2d::ui::Button*>("Button_TeamThree");
	teamThreeButton->addTouchEventListener(CC_CALLBACK_2(GameUILayer::onTeam, this));
	m_createCallback[teamThreeButton] = CC_CALLBACK_0(GameUILayer::teamThree, this);
	
	auto teamFourButton = teamPanel->getChildByName<cocos2d::ui::Button*>("Button_TeamFour");
	teamThreeButton->addTouchEventListener(CC_CALLBACK_2(GameUILayer::onTeam, this));
	m_createCallback[teamThreeButton] = CC_CALLBACK_0(GameUILayer::teamFour, this);
	*/
	auto pathSwitchButton = teamPanel->getChildByName<cocos2d::ui::Button*>("Button_PathSwitch");
	pathSwitchButton->addTouchEventListener(CC_CALLBACK_2(GameUILayer::onPathSwitch, this));
	//m_createCallback[pathSwitchButton] = CC_CALLBACK_0(GameUILayer::createSoldier, this, SoldierType::Cavalry);
	
	auto pathOkButton = teamPanel->getChildByName<cocos2d::ui::Button*>("Button_PathOk");
	pathOkButton->addTouchEventListener(CC_CALLBACK_2(GameUILayer::onPathOk, this));
	//m_createCallback[pathOkButton] = CC_CALLBACK_0(GameUILayer::createSoldier, this, SoldierType::Archer);

	auto pathCancelButton = teamPanel->getChildByName<cocos2d::ui::Button*>("Button_PathCancel");
	pathCancelButton->addTouchEventListener(CC_CALLBACK_2(GameUILayer::onPathCancel, this));
	//m_createCallback[pathOkButton] = CC_CALLBACK_0(GameUILayer::createSoldier, this, SoldierType::Archer);
	return true;
	return false;
}

void GameUILayer::onMinimapTouched(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType touchType)
{
	if (touchType == cocos2d::ui::Widget::TouchEventType::MOVED)
	{
		auto widget = static_cast<cocos2d::ui::Widget*>(sender);
		auto touchStart = widget->convertToNodeSpace(widget->getTouchBeganPosition());
		auto touchMove = widget->convertToNodeSpace(widget->getTouchMovePosition());
		auto deltaPos = touchMove - touchStart;
		deltaPos.negate();
		
		MapManager::getInstance()->setPosition(deltaPos, true);
	
		if (cocos2d::Director::getInstance()->isPaused())
		{
			updateMiniMap();
		}
	}
}

void GameUILayer::onMediumMapTouched(cocos2d::Ref * sender, cocos2d::ui::Widget::TouchEventType touchType)
{
	if (!m_mediumMapImgView->isVisible())
	{
		return;
	}
	if (touchType == cocos2d::ui::Widget::TouchEventType::ENDED)
	{
		cocos2d::Color4F color;
		color = cocos2d::Color4F(248.0f / 255.0f, 200.0f / 255.0f, 40.0f / 255.0f, 1.0f);
		auto widget = static_cast<cocos2d::ui::Widget*>(sender);
		auto touchPos = widget->convertToNodeSpace(widget->getTouchEndPosition());
		m_mediumMapDrawNode->drawSolidRect(touchPos - cocos2d::Vec2(1, 1) , touchPos + cocos2d::Vec2(1,1), color);
		if (!m_pathList.empty())
		{
			//����ֱ��
			m_mediumMapDrawNode->drawLine(m_pathList.back(), touchPos, color);
		}

		m_pathList.push_back(touchPos);
	}
}

void GameUILayer::update(float dt)
{
	updateMiniMap();
	updateGameTime();
	//updateButtonStatus();
	updateTechPoint();
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
		//����λ�������ǵ�ͼ�е�����λ�ã�������Ե�ͼ������û�����ŵ�
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

	//��Ļչʾ�������ź�ĵ�ͼ���������λ�õ�ʱ��Ҫ��������
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

void GameUILayer::updateTechPoint()
{
	auto gameMainPanel = m_gameUI->getChildByName("Panel_Creating");
	auto techPointLabel = gameMainPanel->getChildByName<cocos2d::ui::Text*>("Text_TechPoint");
	techPointLabel->setString(cocos2d::StringUtils::format("%d", GameBattle::getInstance()->getPlayerTechPoint()));
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

void GameUILayer::onPathSwitch(cocos2d::Ref * sender, cocos2d::ui::Widget::TouchEventType touchType)
{
	if (touchType == cocos2d::ui::Widget::TouchEventType::ENDED)
	{
		//��
		if (m_pathSwitch)
		{
			m_mediumMapImgView->setVisible(false);
			m_pathSwitch = false;
		}
		else
		{
			m_mediumMapImgView->setVisible(true);
			m_pathSwitch = true;
		}
		m_currentTeam = 0;
	}
}

void GameUILayer::onPathOk(cocos2d::Ref * sender, cocos2d::ui::Widget::TouchEventType touchType)
{
	if (m_currentTeam != 0)
	{
		//��ȡ���Ƶ�·��
		std::vector<cocos2d::Vec2> pathList;
		GameBattle::getInstance()->setPath(m_currentTeam, pathList);
	}
}

void GameUILayer::onPathCancel(cocos2d::Ref * sender, cocos2d::ui::Widget::TouchEventType touchType)
{
	//���
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


