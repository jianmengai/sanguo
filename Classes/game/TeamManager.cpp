#include "TeamManager.h"

TeamManager* TeamManager::getInstance()
{
	static TeamManager teamManagerInst;
	return &teamManagerInst;
}

int TeamManager::getUniqTeamId()
{
	static int teamId = 100;
	return ++teamId;
}

void TeamManager::addTeam(int teamId, GameObject * gameObject)
{
	if ((teamId <=0) || (gameObject == nullptr))
	{
		return;
	}
	std::vector<GameObject*>& gameObjects = m_teams[teamId];
	gameObjects.push_back(gameObject);
}

void TeamManager::removeFromTeam(int teamId, GameObject * gameObject)
{
	if ((teamId <= 0) || (gameObject == nullptr))
	{
		return;
	}
	auto it = m_teams.find(teamId);
	if (it == m_teams.end())
	{
		return;
	}
	std::vector<GameObject*>& gameObjects = it->second;
	for (auto it = gameObjects.begin(); it != gameObjects.end(); ++it)
	{
		if ((*it) == gameObject)
		{
			gameObjects.erase(it);
			break;
		}
	}
}

void TeamManager::clearTeam(int teamId)
{
	if (teamId <= 0)
	{
		return;
	}
	auto it = m_teams.find(teamId);
	if (it != m_teams.end())
	{
		m_teams.erase(it);
	}
}

void TeamManager::clearTeam()
{
	m_teams.clear();
}

std::vector<GameObject*>& TeamManager::getTeamMembers(int teamId)
{
	return m_teams[teamId];
}

void TeamManager::notifyTeamMemberEnemy(int teamId, GameObject * enemy)
{
	if (teamId <= 0)
	{
		return;
	}
	auto it = m_teams.find(teamId);
	if (it == m_teams.end())
	{
		return;
	}
	for (auto object : it->second)
	{
		if (object != nullptr)
		{
			//设置新的攻击对象
		}
	}
}
