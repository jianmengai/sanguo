#include "SimpleAudioEngine.h"
#include "GameSceneManager.h"


GameSceneManager::GameSceneManager()
{
	m_pLoginLayer = nullptr;
	m_pGameScene = nullptr;
	m_bIsGameSceneRetain = false;
	m_pCurrentScene = nullptr;
}
GameSceneManager::~GameSceneManager()
{
	releaseGameScene();
}
void GameSceneManager::retainGameScene()
{
	if (m_pGameScene)
	{
		if (!m_bIsGameSceneRetain)
		{
			m_bIsGameSceneRetain = true;
			m_pGameScene->retain();
		}
	}
}
void GameSceneManager::releaseGameScene()
{
	if (m_pGameScene)
	{
		if (m_bIsGameSceneRetain)
		{
			m_bIsGameSceneRetain = false;
			m_pGameScene->release();
		}
	}
}
int GameSceneManager::getCurrentSceneType() const
{
	return m_currentSceneType;
}
void GameSceneManager::setCurrentSceneType(int var)
{
	cocos2d::log("setCurrentSceneType, var:%d", var);
	if (m_currentSceneType != var)
	{
		m_currentSceneType = var;
		switch (var)
		{
		case scene_type_run_loading:
		{
			cocos2d::Scene *pScene = LoadingLayer::scene();
			m_pCurrentScene = pScene;
			cocos2d::Director::getInstance()->runWithScene(pScene);
			break;
		}
		case scene_type_loading_2_login:
		case scene_type_game_2_login:
		{
			enterLoginScene();
			break;
		}
		case scene_type_game_2_loading:
		case scene_type_login_2_loading:
		{
			/*
			if (_gameUILayer)
			{
				setCurrentSceneType(scene_type_loading_2_game);
			}
			else
			{
				cocos2d::Scene *lodingScene = LoadingLayer::scene();
				m_pCurrentScene = lodingScene;
				cocos2d::Director::getInstance()->replaceScene(lodingScene);
				if (!m_pGameScene)
				{
					cocos2d::Scene* m_pGameScene = GameLayer::scene();
					setGameScene(m_pGameScene);
				}
				retainGameScene();
			}
			*/
			break;
		}
		case scene_type_loading_2_game:
		{
			m_pCurrentScene = m_pGameScene;
			cocos2d::Director::getInstance()->replaceScene(m_pGameScene);
			releaseGameScene();
			break;
		}
		}
	}
}
void GameSceneManager::exitEXE()
{
	releaseGameScene();
	cocos2d::Director::getInstance()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}

void GameSceneManager::enterLoginScene()
{
	m_curLoadLoginBGNum = 0;
	m_loginBGList.clear();

	int totalW = 3200;
	int sliceW = 256;
	int num = totalW / sliceW;
	if (totalW%sliceW != 0)
		num += 1;
	char str[256] = {'\0'};
	for (int i = 0; i < num; i++)
	{
		memset(str, 0, sizeof(str));
		sprintf(str, "ui/login/login_bg_0_%d.jpg", i);
		m_loginBGList.push_back(str);
		cocos2d::Director::getInstance()->getTextureCache()->addImageAsync(str, CC_CALLBACK_1(GameSceneManager::loadLoginImgCallback, this));
		//TextureManager::getInstance().addStaticImageAsync(str, this, callfuncOD_selector(GameSceneManager::loadLoginImgCallback));
	}
}

void GameSceneManager::loadLoginImgCallback(cocos2d::Texture2D* texture)
{
	m_curLoadLoginBGNum++;
	if (m_curLoadLoginBGNum == m_loginBGList.size())
	{
		switch (m_currentSceneType)
		{
		case scene_type_loading_2_login:
		{
			cocos2d::Scene *pScene = LoginLayer::scene();
			m_pCurrentScene = pScene;
			cocos2d::Director::getInstance()->replaceScene(pScene);
			break;
		}
		case scene_type_game_2_login:
		{
			cocos2d::Scene *pScene = LoginLayer::scene();
			m_pCurrentScene = pScene;
			cocos2d::Director::getInstance()->replaceScene(pScene);
			retainGameScene();
			//GameSceneManager::getInstance().getGameUILayer()->resetPanelState();
			break;
		}
		}
	}
}

void GameSceneManager::exitLoginScene()
{
	std::vector<std::string>::iterator itr;
	for (itr = m_loginBGList.begin(); itr != m_loginBGList.end(); itr++)
	{
		//TextureManager::getInstance().removeStaticImage((*itr).c_str());
	}
}

void GameSceneManager::getLoginImg(std::vector<std::string>& list)
{
	std::vector<std::string> blank;
	list.swap(blank);
	list = m_loginBGList;
}
