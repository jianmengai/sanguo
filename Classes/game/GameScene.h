#pragma once
#include "cocos2d.h"
#include "MapManager.h"

#include "GameUILayer.h"

class GameScene : public cocos2d::Layer
{
public:
	GameScene();
	~GameScene();

	static cocos2d::Scene* scene();

	CREATE_FUNC(GameScene);

	bool init();

	void update(float deltaTime) override;

	bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
	void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);
	void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);

private:
	void pauseGame();

	void onWin();
	void onLost();
private:
	cocos2d::Vec2 m_preTouchPos;
	cocos2d::EventListenerTouchOneByOne* m_touchOneByOneListener = nullptr;
	GameUILayer* m_gameUI = nullptr;
};
