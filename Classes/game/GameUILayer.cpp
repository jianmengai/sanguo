#include "cocostudio/ActionTimeline/CSLoader.h"
#include "GameUILayer.h"
#include "MapManager.h"
#include "GameObjectManager.h"
#include "SoundManager.h"
#include "GameConfig.h"
#include "GameBattle.h"
#include "WarFogLayer.h"
#include "GameBattle.h"
#include "TeamManager.h"

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

	initPathSettingMap();
	initTeamButton();

	initTeamMemSelect();

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

bool GameUILayer::initPathSettingMap()
{
	m_pathSettingPanel = m_gameUI->getChildByName("Panel_PathSetting");
	m_mediumMapImgView = m_pathSettingPanel->getChildByName<cocos2d::ui::ImageView*>("Image_MediumMap");
	if (m_mediumMapImgView == nullptr)
	{
		return false;
	}
	m_mediumMapDrawNode = cocos2d::DrawNode::create();
	m_mediumMapImgView->addChild(m_mediumMapDrawNode);
	m_mediumMapImgView->addTouchEventListener(CC_CALLBACK_2(GameUILayer::onMediumMapTouched, this));

	auto pathOkButton = m_pathSettingPanel->getChildByName<cocos2d::ui::Button*>("Button_PathOk");
	pathOkButton->addTouchEventListener(CC_CALLBACK_2(GameUILayer::onPathOk, this));

	auto pathCancelButton = m_pathSettingPanel->getChildByName<cocos2d::ui::Button*>("Button_PathCancel");
	pathCancelButton->addTouchEventListener(CC_CALLBACK_2(GameUILayer::onPathCancel, this));

	auto width = m_mediumMapImgView->getContentSize().width;
	auto height = m_mediumMapImgView->getContentSize().height;

	auto mapContentSize = MapManager::getInstance()->getContentSize();
	float scalX = width / mapContentSize.width;
	float scalY = height / mapContentSize.height;
	m_scale = std::min(scalX, scalY);


	m_pathSettingPanel->setVisible(false);
	return true;
}

bool GameUILayer::initCreateButton()
{
	cocos2d::log("start init create button");
	auto gameMainPanel = m_gameUI->getChildByName("Panel_Creating");
	auto createBuildingPanel = gameMainPanel->getChildByName("Panel_CreateBuilding");
	auto createSoldierPanel = gameMainPanel->getChildByName("Panel_CreateSoldier");
	cocos2d::log("get button done");
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

bool GameUILayer::initTeamButton()
{
	cocos2d::log("start init team button");
	auto teamPanel = m_gameUI->getChildByName("Panel_Team");
	
	auto teamOneButton = teamPanel->getChildByName<cocos2d::ui::Button*>("Button_TeamOne");
	teamOneButton->addTouchEventListener(CC_CALLBACK_2(GameUILayer::onTeam, this));
	m_teamCallback[teamOneButton] = CC_CALLBACK_0(GameUILayer::selectTeam, this, TeamNo::One);
	m_teamButtons.push_back(teamOneButton);

	auto teamTwoButton = teamPanel->getChildByName<cocos2d::ui::Button*>("Button_TeamTwo");
	teamTwoButton->addTouchEventListener(CC_CALLBACK_2(GameUILayer::onTeam, this));
	m_teamCallback[teamTwoButton] = CC_CALLBACK_0(GameUILayer::selectTeam, this, TeamNo::Two);
	m_teamButtons.push_back(teamTwoButton);
	
	auto teamThreeButton = teamPanel->getChildByName<cocos2d::ui::Button*>("Button_TeamThree");
	teamThreeButton->addTouchEventListener(CC_CALLBACK_2(GameUILayer::onTeam, this));
	m_teamCallback[teamThreeButton] = CC_CALLBACK_0(GameUILayer::selectTeam, this, TeamNo::Three);
	m_teamButtons.push_back(teamThreeButton);
	
	
	auto teamFourButton = teamPanel->getChildByName<cocos2d::ui::Button*>("Button_TeamFour");
	teamFourButton->addTouchEventListener(CC_CALLBACK_2(GameUILayer::onTeam, this));
	m_teamCallback[teamFourButton] = CC_CALLBACK_0(GameUILayer::selectTeam, this, TeamNo::Four);
	m_teamButtons.push_back(teamFourButton);
	
	
	auto pathSwitchButton = teamPanel->getChildByName<cocos2d::ui::Button*>("Button_PathSwitch");
	pathSwitchButton->addTouchEventListener(CC_CALLBACK_2(GameUILayer::onPathSwitch, this));
	
	auto temMemSelectButton = teamPanel->getChildByName<cocos2d::ui::Button*>("Button_TeamMemSelect");
	temMemSelectButton->addTouchEventListener(CC_CALLBACK_2(GameUILayer::onTeamMemSelect, this));
	return true;
}

bool GameUILayer::initTeamMemSelect()
{
	m_teamMemSelectPanel = m_gameUI->getChildByName("Panel_TeamMemSelect");
	m_scrollView = m_teamMemSelectPanel->getChildByName<cocos2d::ui::ScrollView*>("ScrollView_TeamMemSelect");
	m_scrollView->setDirection(cocos2d::ui::ScrollView::Direction::VERTICAL);

	auto size = m_scrollView->getChildrenCount();
	for (auto i = 0; i < size; ++i)
	{
		char szBuffer[16] = {'\0'};
		snprintf(szBuffer, sizeof(szBuffer), "CheckBox_%d", i);
		auto checkBox = dynamic_cast<cocos2d::ui::CheckBox*>(m_scrollView->getChildByName(szBuffer));
		checkBox->addEventListener(CC_CALLBACK_2(GameUILayer::onCheckBoxSelect, this));
		m_checkBoxIndex[checkBox] = i;
		CheckBoxValue checkBoxValue;
		checkBoxValue.checkBox = checkBox;
		m_teamMemCheckBox.push_back(checkBoxValue);
	}
	
	auto teamMemOkButton = m_teamMemSelectPanel->getChildByName<cocos2d::ui::Button*>("Button_TeamMemOk");
	teamMemOkButton->addTouchEventListener(CC_CALLBACK_2(GameUILayer::onTeamMemOk, this));

	m_teamMemSelectPanel->setVisible(false);
	return true;
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
	if (!m_pathSettingPanel->isVisible())
	{
		return;
	}
	if (touchType == cocos2d::ui::Widget::TouchEventType::ENDED)
	{
		cocos2d::Color4F color;
		color = cocos2d::Color4F(248.0f / 255.0f, 200.0f / 255.0f, 40.0f / 255.0f, 1.0f);
		auto widget = static_cast<cocos2d::ui::Widget*>(sender);
		auto touchPos = widget->convertToNodeSpace(widget->getTouchEndPosition());
		
		if (!m_pathList.empty())
		{
			//绘制直线
			m_mediumMapDrawNode->drawLine(m_pathList.back(), touchPos, color);
		}
		//
		else
		{
			/*if (m_pathStartPos == cocos2d::Vec2::ZERO)
			{
				return;
			}*/
			m_mediumMapDrawNode->drawLine(m_pathStartPos, touchPos, color);
		}
		m_mediumMapDrawNode->drawSolidRect(touchPos - cocos2d::Vec2(1, 1), touchPos + cocos2d::Vec2(1, 1), color);
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

void GameUILayer::onTeam(cocos2d::Ref * sender, cocos2d::ui::Widget::TouchEventType touchType)
{
	for (auto bt : m_teamButtons)
	{
		cocos2d::ui::Button* button = dynamic_cast<cocos2d::ui::Button*>(bt);
		if (bt == sender)
		{
			button->setHighlighted(true);
		}
		else
		{
			button->setHighlighted(false);
		}
	}
	if (touchType == cocos2d::ui::Widget::TouchEventType::ENDED)
	{
		auto it = m_teamCallback.find(sender);
		if (it != m_teamCallback.end())
		{
			it->second();
		}
	}
}

//路径设置开关
void GameUILayer::onPathSwitch(cocos2d::Ref * sender, cocos2d::ui::Widget::TouchEventType touchType)
{
	if (touchType == cocos2d::ui::Widget::TouchEventType::ENDED)
	{
		if (m_pathSettingPanel->isVisible())
		{
			m_pathSettingPanel->setVisible(false);
		}
		else
		{
			if (m_teamMemSelectPanel->isVisible())
			{
				m_teamMemSelectPanel->setVisible(false);
			}
			m_pathSettingPanel->setVisible(true);
			if (m_pathStartPos != cocos2d::Vec2::ZERO)
			{
				cocos2d::Color4F color = cocos2d::Color4F(248.0f / 255.0f, 200.0f / 255.0f, 40.0f / 255.0f, 1.0f);
				m_mediumMapDrawNode->drawSolidRect(m_pathStartPos - cocos2d::Vec2(1, 1), m_pathStartPos + cocos2d::Vec2(1, 1), color);
			}
			
		}
		
	}
}

void GameUILayer::onPathOk(cocos2d::Ref * sender, cocos2d::ui::Widget::TouchEventType touchType)
{
	if (touchType != cocos2d::ui::Widget::TouchEventType::ENDED)
	{
		return;
	}
	if (m_currentTeamNo != TeamNo::Invalid)
	{
		//获取绘制的路径
		std::list<cocos2d::Vec2> pathList;
		for (auto pos : m_pathList)
		{
			cocos2d::Vec2 mapPos = pos / m_scale;
			pathList.push_back(mapPos);
		}
		GameBattle::getInstance()->setPlayerTeamPath(m_currentTeamNo, pathList);
		m_pathList.clear();
		m_mediumMapDrawNode->clear();
	}
}

void GameUILayer::onPathCancel(cocos2d::Ref * sender, cocos2d::ui::Widget::TouchEventType touchType)
{
	//清除
	m_mediumMapDrawNode->clear();
	m_pathList.clear();
}

//编队开关
void GameUILayer::onTeamMemSelect(cocos2d::Ref * sender, cocos2d::ui::Widget::TouchEventType touchType)
{
	if (touchType == cocos2d::ui::Widget::TouchEventType::ENDED)
	{
		if (m_teamMemSelectPanel->isVisible())
		{
			m_teamMemSelectPanel->setVisible(false);
		}
		else
		{
			if (m_pathSettingPanel->isVisible())
			{
				m_pathSettingPanel->setVisible(false);
			}
			m_teamMemSelectPanel->setVisible(true);
			showTeamMemList();
		}
	}
}

void GameUILayer::onCheckBoxSelect(cocos2d::Ref * sender, cocos2d::ui::CheckBox::EventType eventType)
{
	auto it = m_checkBoxIndex.find(sender);
	if (it == m_checkBoxIndex.end())
	{
		return;
	}
	int index = it->second;
	auto checkBox = m_teamMemCheckBox[index].checkBox;
	switch (eventType)
	{
	case cocos2d::ui::CheckBox::EventType::SELECTED:
	{
		checkBox->setSelected(true);
		cocos2d::log("checkBox:%d selected", index);
	} 
	break;
	case cocos2d::ui::CheckBox::EventType::UNSELECTED:
	{
		checkBox->setSelected(false);
		cocos2d::log("checkBox:%d unselected", index);
	}
	break;
	default:
		break;
	}
}

void GameUILayer::onTeamMemOk(cocos2d::Ref * sender, cocos2d::ui::Widget::TouchEventType touchType)
{
	m_teamMemSelectPanel->setVisible(false);
	if (m_currentTeamNo == TeamNo::Invalid)
	{
		return;
	}

	std::vector<int> teamMem;
	for (auto checkBoxValue : m_teamMemCheckBox)
	{
		if (checkBoxValue.gameObjectId == 0)
		{
			continue;
		}
		if (checkBoxValue.checkBox->isSelected())
		{
			teamMem.push_back(checkBoxValue.gameObjectId);
		}
		//没被选中的恢复无队列状态
		else
		{
			auto gameObject = GameObjectManager::getInstance()->getGameObjectById(checkBoxValue.gameObjectId);
			Soldier* soldier = dynamic_cast<Soldier*>(gameObject);
			if (soldier != nullptr)
			{
				soldier->setTeamNo(TeamNo::Invalid);
			}
			
		}

		//重置
		checkBoxValue.gameObjectId = 0;
	}

	GameBattle::getInstance()->setPlayerTeam(m_currentTeamNo, teamMem);

	selectTeam(m_currentTeamNo);
}

bool GameUILayer::selectTeam(TeamNo teamNo)
{
	m_pathStartPos = cocos2d::Vec2::ZERO;
	int teamId = GameBattle::getInstance()->getPlayerTeamId(teamNo);
	if (teamId == -1)
	{
		return false;
	}
	auto teamMembers = TeamManager::getInstance()->getTeamMembers(teamId);
	
	for (auto gameObject : teamMembers)
	{
		m_pathStartPos += gameObject->getPosition();
	}

	m_pathStartPos = m_pathStartPos / teamMembers.size();

	m_pathStartPos = m_pathStartPos * m_scale;

	m_currentTeamNo = teamNo;

	GameBattle::getInstance()->selectPlayerTeam(teamNo);

	return true;
}

void GameUILayer::showTeamMemList()
{
	auto teamId = GameBattle::getInstance()->getPlayerTeamId(m_currentTeamNo);
	auto teamMem = TeamManager::getInstance()->getTeamMembers(teamId);
	//已经在队伍中的在前面，不在队伍中的按照类型排在后面
	std::vector<Soldier*> sortedSoldiers;
	for (auto soldier : teamMem)
	{
		sortedSoldiers.push_back(dynamic_cast<Soldier*>(soldier));
	}
	auto allSoldiers = GameBattle::getInstance()->getPlayerSoldiers();
	for (auto soldiers : allSoldiers)
	{
		for (auto soldier : soldiers.second)
		{
			bool inTeam = false;
			for (auto s : teamMem)
			{
				if (s->getId() == soldier->getId())
				{
					inTeam = true;
					break;
				}
			}
			auto teamNo = soldier->getTeamNo();
			//已经在其他编组里面了
			if ((teamNo != TeamNo::Invalid) && (teamNo != m_currentTeamNo))
			{
				continue;
			}
			if (!inTeam)
			{
				sortedSoldiers.push_back(soldier);
			}
		}
	}
	int index = 0;
	int teamMemSize = teamMem.size();
	int checkBoxSize = m_teamMemCheckBox.size();
	for (auto soldier : sortedSoldiers)
	{
		if (index >= checkBoxSize)
		{
			break;
		}
		auto checkBox = m_teamMemCheckBox[index].checkBox;
		if (index < teamMemSize)
		{
			checkBox->setSelected(true);
		}
		else
		{
			checkBox->setSelected(false);
		}
		std::string soldierImg;
		switch (soldier->getSoldierType())
		{
		case SoldierType::Archer:
		{
			soldierImg = "Archer.png";
		}
		break;
		case SoldierType::Cavalry:
		{
			soldierImg = "Enchanter.png";
		}
		break;
		case SoldierType::Infantry:
		{
			soldierImg = "Barbarian.png";
		}
		break;
		default:
			break;
		}
		if (!soldierImg.empty())
		{
			checkBox->loadTextureBackGround(soldierImg);
			checkBox->loadTextureFrontCross("CheckBox_Selected.png");
			m_teamMemCheckBox[index].gameObjectId = soldier->getId();
		}
		++index;
		checkBox->setVisible(true);
	}
	for (int i = index; i < checkBoxSize; ++i)
	{
		auto checkBox = m_teamMemCheckBox[i].checkBox;
		checkBox->setVisible(false);
	
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


