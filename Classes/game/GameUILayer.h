#pragma once
#include "cocos2d.h"
#include "ui/UIImageView.h"
#include "ui/UIText.h"
#include "ui/UIButton.h"
#include "ui/UIScrollView.h"
//#include "MsgDisplay.h"
#include "GameDefine.h"

class GameUILayer : public cocos2d::Node
{
public:
	GameUILayer();
	~GameUILayer();
	CREATE_FUNC(GameUILayer);
	//CC_SYNTHESIZE_READONLY(MsgDisplay*, _msgDisplay, MsgDisplay);

	void onMinimapTouched(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType touchType);
	void onMediumMapTouched(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType touchType);
	void update(float dt);

	bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
	void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);
	void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);

private:
	bool init() override;
	bool initMiniMap();

	bool initPathSettingMap();

	//初始化建造、生成、升级等按钮
	bool initCreateButton();

	bool initTeamButton();

	bool initTeamMemSelect();

	void updateMiniMap();
	void updateGameTime();
	void updateButtonStatus();
	void updateTechPoint();

	bool createBuilding(BuildingType type);
	bool createSoldier(SoldierType type);
	void onCreateObject(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType touchType);

	void onTeam(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType touchType);

	void onPathSwitch(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType touchType);
	void onPathOk(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType touchType);
	void onPathCancel(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType touchType);
	void onTeamMemSelect(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType touchType);


	bool selectTeam(TeamNo teamNo);

	void showTeamMemList();
private:
	cocos2d::Node* m_gameUI = nullptr;
	cocos2d::ui::ImageView* m_miniMapImgView = nullptr;
	cocos2d::DrawNode* m_miniMapDrawNode = nullptr;
	time_t m_gameStartTime;
	std::map<cocos2d::Ref*, std::function<bool()> > m_createCallback;
	cocos2d::Vec2 m_touchPos;

	std::map<cocos2d::Ref*, std::function<bool()> > m_teamCallback;
	//路径设置地图
	cocos2d::ui::ImageView* m_mediumMapImgView = nullptr;
	cocos2d::DrawNode* m_mediumMapDrawNode = nullptr;

	int m_currentTeam = 0;
	std::list<cocos2d::Vec2> m_pathList;
	cocos2d::Vec2 m_pathStartPos;


	//队伍编辑
	cocos2d::ui::ScrollView* m_scrollView = nullptr;

	cocos2d::Node* m_pathSettingPanel = nullptr;
	cocos2d::Node* m_teamMemSelectPanel = nullptr;
};
