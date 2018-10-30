#include "GameScene.h"
#include "cocostudio/ActionTimeline/CSLoader.h"
#include "GameBattle.h"
#include "GameObjectManager.h"
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
	auto sprite = cocos2d::Sprite::create("logo.png");
	sprite->setAnchorPoint(cocos2d::Vec2(0.0f, 0.0f));
	//sprite->setScaleX(0.5f);
	this->addChild(sprite, 0);
	cocos2d::log("loading map...");
	std::string tileMap = "map/test451.tmx";
	if (!MapManager::getInstance()->init(this, tileMap))
	{
		return false;
	}

	auto mapSize = MapManager::getInstance()->getMapSize();
	auto tileSize = MapManager::getInstance()->getTileSize();
	m_warFog = WarFogLayer::create(mapSize.width, mapSize.height);
	if (nullptr == m_warFog)
	{
		return false;
	}

	m_warFog->setTileSize(tileSize);
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
	updateWarFog();
}


void GameScene::updateWarFog()
{
	auto& gameObjectMap = GameObjectManager::getInstance()->getGameObjectMap();
	for (auto& objectIt : gameObjectMap)
	{
		auto object = objectIt.second;
		if (object->getForceType() == ForceType::Player)
		{
			auto pos = MapManager::getInstance()->toTileRowCol(object->getPosition());
			m_warFog->inView(pos.x, pos.y);
		}
	}
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
	m_warFog->setPosition(deltaPos);

}

void GameScene::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event)
{
	cocos2d::Vec2 curPosition = touch->getLocation();
	cocos2d::log("GameScene::onTouchEnded, ");
	GameBattle::getInstance()->touchProcess(curPosition);
}