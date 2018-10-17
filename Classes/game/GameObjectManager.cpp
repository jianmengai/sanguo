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
