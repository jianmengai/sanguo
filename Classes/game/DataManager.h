#pragma once

#include <queue>

class DataManager
{
public:
	static DataManager& getInstance()
	{
		static DataManager instance;
		return instance;
	}
	DataManager();
	~DataManager();

	void loadRes();
	bool loadResTick(float dt);

private:
	std::queue<int> m_waitInitDataQueue;//等待加载的数据
};