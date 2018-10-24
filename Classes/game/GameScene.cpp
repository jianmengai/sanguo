#include "GameScene.h"
#include "cocostudio/ActionTimeline/CSLoader.h"
#include "GameBattle.h"
GameScene::GameScene()
{
}

GameScene::~GameScene()
{

}

cocos2d::Scene* GameScene::scene()
{
	cocos2d::Scene* scene = cocos2d::Scene::create();
	GameScene *gameSceme = GameScene::create();
	scene->addChild(gameSceme);
	return scene;
}

bool GameScene::init()
{
	if (!cocos2d::Layer::init())
	{
		return false;
	}

	cocos2d::log("loading map...");
	std::string tileMap = "map/test4.tmx";
	if (!MapManager::getInstance()->init(this, tileMap))
	{
		return false;
	}

	auto mapContentSize = MapManager::getInstance()->getContentSize();
	m_warFog = WarFogLayer::create(mapContentSize.width, mapContentSize.height);
	if (nullptr == m_warFog)
	{
		return false;
	}
	this->addChild(m_warFog);

	m_gameUI = GameUILayer::create();
	if (nullptr == m_gameUI)
	{
		return false;
	}
	this->addChild(m_gameUI);

	auto dispatcher = cocos2d::Director::getInstance()->getEventDispatcher();
	m_touchOneByOneListener = cocos2d::EventListenerTouchOneByOne::create();
	m_touchOneByOneListener->setSwallowTouches(true);
	m_touchOneByOneListener->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchBegan, this);
	m_touchOneByOneListener->onTouchMoved = CC_CALLBACK_2(GameScene::onTouchMoved, this);
	m_touchOneByOneListener->onTouchEnded = CC_CALLBACK_2(GameScene::onTouchEnded, this);
	dispatcher->addEventListenerWithSceneGraphPriority(m_touchOneByOneListener, this);

	scheduleUpdate();

	return true;
}


void GameScene::update(float deltaTime)
{
	MapManager::getInstance()->update(deltaTime);
	GameBattle::getInstance()->update(deltaTime);
}

void GameScene::pauseGame()
{

}

void GameScene::onWin()
{

}

void GameScene::onLost()
{

}

bool GameScene::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event)
{
	m_preTouchPos = touch->getLocation();
	//cocos2d::log("------x:%0.1f, y:%0.1f------", m_preTouchPos.x, m_preTouchPos.y);
	return true;
}

void GameScene::onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event)
{
	cocos2d::Vec2 curPosition = touch->getLocation();
	cocos2d::Vec2 deltaPos = curPosition - m_preTouchPos;
	m_preTouchPos = curPosition;
	//cocos2d::log("GameScene::onTouchMoved, x:%0.1f, y:%0.1f", deltaPos.x, deltaPos.y);
	MapManager::getInstance()->setPosition(deltaPos, true);
	

}

void GameScene::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event)
{
	cocos2d::Vec2 curPosition = touch->getLocation();
	cocos2d::log("GameScene::onTouchEnded, ");
	GameBattle::getInstance()->touchProcess(curPosition);
}