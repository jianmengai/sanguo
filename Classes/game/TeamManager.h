#pragma once

#include <vector>
#include <map>
#include "GameObject.h"

class TeamManager
{
public:
	static TeamManager* getInstance();
	int getUniqTeamId();
	void addTeam(int teamId, GameObject* gameObject);
	void removeFromTeam(GameObject* gameObject);
	void clearTeam(int teamId);
	void clearTeam();

	std::vector<GameObject*>& getTeamMembers(int teamId);

	void notifyTeamMemberEnemy(int teamId, GameObject* enemy);

	void update(float dt);
private:
	void removeDiedObject();
private:
	std::map<int, std::vector<GameObject*> > m_teams;
};