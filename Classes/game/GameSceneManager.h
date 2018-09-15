#pragma once

#include "cocos2d.h"
#include "LoadingScene.h"
#include "LoginScene.h"
#include "GameDefine.h"
#include "GameUILayer.h"

typedef enum
{
	scene_type_start = 1,

	scene_type_run_loading,//运行加载
	scene_type_run_login,  
	scene_type_run_download,//运行下载场景
	scene_type_download_2_loading,//下载场景2加载场景
	scene_type_login_2_loading,//登陆场景2加载场景
	scene_type_loading_2_login,//加载场景2登陆场景
	scene_type_loading_2_game,//加载场景2游戏场景
	scene_type_game_2_loading,//游戏场景2加载场景
	scene_type_game_2_login,//游戏场景2登陆场景

	scene_type_end
} SceneType;

class GameSceneManager : public cocos2d::CCObject
{
public:
	static GameSceneManager& getInstance()
	{
		static GameSceneManager instance;
		return instance;
	}

	//retain游戏场景
	void retainGameScene();
	//release游戏场景
	void releaseGameScene();
	//终止游戏，退出exe
	void exitEXE();

	CC_SYNTHESIZE(LoadingLayer*, m_pLoadingLayer, LoadingLayer);//游戏加载层
	CC_SYNTHESIZE(LoginLayer*, m_pLoginLayer, LoginLayer);//登陆层
	CC_SYNTHESIZE(cocos2d::Scene*, m_pGameScene, GameScene);//游戏scene
	CC_PROPERTY(int, m_currentSceneType, CurrentSceneType); //当前运行场景类型
	CC_SYNTHESIZE_READONLY(cocos2d::Scene*, m_pCurrentScene, CurrentScene); //当前运行场景
	CC_SYNTHESIZE(bool, m_bIsFirstLoading, IsFirstLoading); //是否是首次加载
	CC_SYNTHESIZE(GameUILayer*, m_pGameUILayer, GameUILayer);//游戏ui层
public:
	//进入登陆场景
	void enterLoginScene();
	//离开登陆场景
	void exitLoginScene();
	//加载登陆场景bg图的回调
	void loadLoginImgCallback(cocos2d::Texture2D* texture);
	//获取登陆场景bg图信息
	void getLoginImg(std::vector<std::string>& list);
private:
	GameSceneManager();
	GameSceneManager(const GameSceneManager&);
	GameSceneManager& operator=(const GameSceneManager&);
	~GameSceneManager();
private:
	//游戏场景是否retain
	bool m_bIsGameSceneRetain;
	//登陆的背景图
	std::vector<std::string> m_loginBGList;
	//当前load背景图片数目
	unsigned int m_curLoadLoginBGNum;
};