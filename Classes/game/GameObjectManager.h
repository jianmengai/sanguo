#pragma once

#include <unordered_map>
#include "GameObject.h"


typedef std::unordered_map<int, GameObject*> GameObjectMap;

class GameObjectManager
{
public:
	static GameObjectManager* getInstance();

	GameObject* getGameObjectById(int objectId);
	void addGameObject(GameObject* gameObject);
	GameObjectMap& getGameObjectMap();
private:
	GameObjectMap m_gameObjects;
};