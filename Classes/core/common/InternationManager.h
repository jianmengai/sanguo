#pragma once

#include <map>

#define GETSTR(str) InternationManager::getInstance().getCharacterText(str)

class InternationManager
{
public:
	static InternationManager& getInstance()
	{
		static InternationManager instance;
		return instance;
	}

	void readData(const char* filename);//��ȡ��̬����
	std::string getCharacterText(std::string key);//��ȡһ���ִ�
	std::map<std::string, std::string>* getAllCharacterText();

private:
	InternationManager();
	InternationManager(const InternationManager&);
	InternationManager& operator=(const InternationManager&) {}
	~InternationManager();

private:
	std::map<std::string, std::string> m_characterDic;
};