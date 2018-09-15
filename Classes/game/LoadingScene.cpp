#include "LoadingScene.h"
#include "GameSceneManager.h"
#include "DataManager.h"
#include "GameDefine.h"
#include "UIDefine.h"

LoadingLayer::LoadingLayer()
{

}

LoadingLayer::~LoadingLayer()
{
	GameSceneManager::getInstance().setLoadingLayer(nullptr);
}

cocos2d::Scene* LoadingLayer::scene()
{
	cocos2d::Scene * scene = cocos2d::Scene::create();

	LoadingLayer *layer = LoadingLayer::create();
	GameSceneManager::getInstance().setLoadingLayer(layer);
	scene->addChild(layer);

	return scene;
}

bool LoadingLayer::init()
{
	cocos2d::Layer::init();
	auto winSize = cocos2d::Director::getInstance()->getWinSize();

	m_pPercent = cocos2d::Label::createWithBMFont("fonts/number.fnt", "0/100");
	m_pPercent->setPosition(cocos2d::Point(winSize.width / 2, 50));
	addChild(m_pPercent, 1);

	m_pBG = nullptr;

	scheduleUpdate();
	//setKeypadEnabled(true);//ÏìÓ¦¼üÅÌ

	this->resetProgress();

	return true;
}

void LoadingLayer::onEnter()
{
	cocos2d::Layer::onEnter();
	auto winSize = cocos2d::Director::getInstance()->getWinSize();
	m_pBG = cocos2d::Sprite::create(LOADING_IMAGE);
	m_pBG->setPosition(cocos2d::Point(winSize.width / 2, winSize.height / 2));
	addChild(m_pBG, 0);
	cocos2d::Director::getInstance()->getTextureCache()->removeTextureForKey(LOADING_IMAGE);
	this->startLoad();
}

void LoadingLayer::update(float dt)
{
	if (DataManager::getInstance().loadResTick(dt))
	{
		stopPreLoad();
		GameSceneManager::getInstance().setIsFirstLoading(false);
	}
}

void LoadingLayer::startLoad()
{
	DataManager::getInstance().loadRes();
}

void LoadingLayer::stopPreLoad()
{
	GameSceneManager::getInstance().setCurrentSceneType(scene_type_loading_2_login);
}


void LoadingLayer::setProgress()
{

	int per = m_iCurSize * 100 / m_iTotalSize;
	char perStr[256] = {'\0'};
	memset(perStr, 0, sizeof(perStr));
	sprintf(perStr, "%d/100", per);
	m_pPercent->setString(perStr);
}

void LoadingLayer::addCurreSize(int curreSize)
{
	this->m_iCurSize += curreSize;
	setProgress();
}

void LoadingLayer::addTotalSize(int totalSize)
{
	this->m_iTotalSize += totalSize;
	setProgress();
}

void LoadingLayer::resetProgress()
{
	m_iCurSize = 0;
	m_iTotalSize = 0;
	m_pPercent->setString("0/100");
}

