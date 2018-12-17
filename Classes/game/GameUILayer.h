#pragma once
#include "cocos2d.h"
#include "ui/UIImageView.h"
#include "ui/UIText.h"
#include "ui/UIButton.h"
#include "ui/UIScrollView.h"
#include "ui/UICheckBox.h"
#include "GameDefine.h"


typedef struct _soldier_node_st
{
	bool selected = false;
	SoldierType type = SoldierType::Invalid;
	std::vector<int> gameObjectIds;
}SoldierNode;

typedef struct _checkBox_value_st
{
	cocos2d::ui::CheckBox* checkBox = nullptr;
	SoldierNode soldierNode;
}CheckBoxValue;

class Soldier;

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

	static void showWin();
	static void showFail();

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
	void onExit(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType touchType);

	void onTeam(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType touchType);

	void onPathSwitch(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType touchType);
	void onPathOk(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType touchType);
	void onPathCancel(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType touchType);
	void onTeamMemSelect(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType touchType);
	void onCheckBoxSelect(cocos2d::Ref* sender, cocos2d::ui::CheckBox::EventType eventType);
	void onTeamMemOk(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType touchType);

	bool selectTeam(TeamNo teamNo);

	void showTeamMemList();
	void drawPath();
	void updatePathStartPos();

	void getSoldierNode(std::vector<Soldier*>& sortedSoldiers, bool selected, std::vector<SoldierNode>& soldierNodes);
private:
	cocos2d::Node* m_gameUI = nullptr;
	cocos2d::ui::ImageView* m_miniMapImgView = nullptr;
	cocos2d::DrawNode* m_miniMapDrawNode = nullptr;
	time_t m_gameStartTime;
	std::map<cocos2d::Ref*, std::function<bool()> > m_createCallback;
	cocos2d::Vec2 m_touchPos;

	std::map<cocos2d::Ref*, std::function<bool()> > m_teamCallback;
	//路径设置地图
	cocos2d::Node* m_pathSettingPanel = nullptr;
	cocos2d::ui::ImageView* m_mediumMapImgView = nullptr;
	cocos2d::DrawNode* m_mediumMapDrawNode = nullptr;

	TeamNo m_currentTeamNo = TeamNo::Invalid;
	std::list<cocos2d::Vec2> m_pathList;
	cocos2d::Vec2 m_pathStartPos;
	std::vector<cocos2d::Ref*> m_teamButtons;
	float m_scale = 1.0;

	//队伍编辑
	cocos2d::Node* m_teamMemSelectPanel = nullptr;
	cocos2d::ui::ScrollView* m_scrollView = nullptr;
	std::vector<CheckBoxValue> m_teamMemCheckBox;
	std::map<cocos2d::Ref*, int> m_checkBoxIndex;
	std::map<cocos2d::Ref*, bool> m_teamButtonSelect;
	bool m_isSelectOp = true;

	static cocos2d::ui::Text* ms_showWinText;
	static cocos2d::ui::Text* ms_showFailText;
};
