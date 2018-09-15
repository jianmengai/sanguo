#pragma once
#include "cocos2d.h"

class LoadingLayer : public cocos2d::Layer
{
public:
	LoadingLayer();
	~LoadingLayer();

	virtual bool init();
	virtual void onEnter();
	virtual void update(float dt);
	static cocos2d::Scene* scene();

	CREATE_FUNC(LoadingLayer);

	void addTotalSize(int totalSize);
	void addCurreSize(int curreSize);

	void startLoad();
	void stopPreLoad();

private:
	void setProgress();
	void resetProgress();

private:
	cocos2d::Label* m_pPercent;
	cocos2d::Sprite* m_pBG;
	int m_iCurSize;//当前加载数目
	int m_iTotalSize;//加载上限
};