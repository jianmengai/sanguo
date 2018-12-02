#pragma once

#include <map>
#include <vector>
#include "GameObject.h"


typedef std::map<int, GameObject*> GameObjectMap;

class GameObjectManager
{
public:
	static GameObjectManager* getInstance();

	GameObject* getGameObjectById(int objectId);
	void addGameObject(GameObject* gameObject);
	GameObjectMap& getGameObjectMap();
	void addReadyToRemoveGameObject(int gameObjectId);
	void removeAllReadyToRemoveGameObjects();
private:
	void removeGameObjectById(int gameObjectId);
private:
	GameObjectMap m_gameObjects;
	std::vector<int> m_readyToRemoveGameObjectIdList;
};