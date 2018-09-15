#pragma once

#include "cocos2d.h"
#include "LoadingScene.h"
#include "LoginScene.h"
#include "GameDefine.h"
#include "GameUILayer.h"

typedef enum
{
	scene_type_start = 1,

	scene_type_run_loading,//���м���
	scene_type_run_login,  
	scene_type_run_download,//�������س���
	scene_type_download_2_loading,//���س���2���س���
	scene_type_login_2_loading,//��½����2���س���
	scene_type_loading_2_login,//���س���2��½����
	scene_type_loading_2_game,//���س���2��Ϸ����
	scene_type_game_2_loading,//��Ϸ����2���س���
	scene_type_game_2_login,//��Ϸ����2��½����

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

	//retain��Ϸ����
	void retainGameScene();
	//release��Ϸ����
	void releaseGameScene();
	//��ֹ��Ϸ���˳�exe
	void exitEXE();

	CC_SYNTHESIZE(LoadingLayer*, m_pLoadingLayer, LoadingLayer);//��Ϸ���ز�
	CC_SYNTHESIZE(LoginLayer*, m_pLoginLayer, LoginLayer);//��½��
	CC_SYNTHESIZE(cocos2d::Scene*, m_pGameScene, GameScene);//��Ϸscene
	CC_PROPERTY(int, m_currentSceneType, CurrentSceneType); //��ǰ���г�������
	CC_SYNTHESIZE_READONLY(cocos2d::Scene*, m_pCurrentScene, CurrentScene); //��ǰ���г���
	CC_SYNTHESIZE(bool, m_bIsFirstLoading, IsFirstLoading); //�Ƿ����״μ���
	CC_SYNTHESIZE(GameUILayer*, m_pGameUILayer, GameUILayer);//��Ϸui��
public:
	//�����½����
	void enterLoginScene();
	//�뿪��½����
	void exitLoginScene();
	//���ص�½����bgͼ�Ļص�
	void loadLoginImgCallback(cocos2d::Texture2D* texture);
	//��ȡ��½����bgͼ��Ϣ
	void getLoginImg(std::vector<std::string>& list);
private:
	GameSceneManager();
	GameSceneManager(const GameSceneManager&);
	GameSceneManager& operator=(const GameSceneManager&);
	~GameSceneManager();
private:
	//��Ϸ�����Ƿ�retain
	bool m_bIsGameSceneRetain;
	//��½�ı���ͼ
	std::vector<std::string> m_loginBGList;
	//��ǰload����ͼƬ��Ŀ
	unsigned int m_curLoadLoginBGNum;
};