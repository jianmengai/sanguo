#include "GameObjectManager.h"

GameObjectManager* GameObjectManager::getInstance()
{
	static GameObjectManager gameObjectManagerInstance;
	return &gameObjectManagerInstance;
}

GameObject* GameObjectManager::getGameObjectById(int objectId)
{
	GameObject* gameObject = nullptr;
	auto it = m_gameObjects.find(objectId);
	if (it != m_gameObjects.end())
	{
		gameObject = it->second;
	}

	return gameObject;
}

void GameObjectManager::addGameObject(GameObject* gameObject)
{
	if (gameObject == nullptr)
	{
		return ;
	}
	int id = gameObject->getId();
	auto it = m_gameObjects.find(id);
	if (it != m_gameObjects.end())
	{
		cocos2d::log("object id:%d exist", id);
		return ;
	}
	m_gameObjects[id] = gameObject;
}

GameObjectMap& GameObjectManager::getGameObjectMap()
{
	return m_gameObjects;
}

void GameObjectManager::addReadyToRemoveGameObject(int gameObjectId)
{
	m_readyToRemoveGameObjectIdList.push_back(gameObjectId);
}

void GameObjectManager::removeAllReadyToRemoveGameObjects()
{
	for (auto gameObjectId : m_readyToRemoveGameObjectIdList)
	{
		removeGameObjectById(gameObjectId);
	}
	m_readyToRemoveGameObjectIdList.clear();
}

void GameObjectManager::removeGameObjectById(int gameObjectId)
{
	auto gameObjectIter = m_gameObjects.find(gameObjectId);
	if (gameObjectIter != m_gameObjects.end())
	{
		gameObjectIter->second->removeFromParent();
		m_gameObjects.erase(gameObjectId);
	}
}