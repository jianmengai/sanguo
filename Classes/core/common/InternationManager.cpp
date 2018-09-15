#include "InternationManager.h"
#include "ReadUtil.h"
#include "cocos2d.h"

InternationManager::InternationManager()
{
}
InternationManager::~InternationManager()
{
}
void InternationManager::readData(const char* filename)
{
	ssize_t fileLen = 0;
	unsigned char *fileData = NULL;
	fileData = cocos2d::FileUtils::getInstance()->getFileData(filename, "rb", &fileLen);
	unsigned char* temp = fileData;
	std::string key;
	std::string value;
	ssize_t pos = 0;
	bool flag = false;
	bool nextLine = false;
	while (pos < fileLen)
	{
		char c;
		pos += readChar(fileData, c);
		//注释，跳过这行
		if (c == '#' && key == "")
		{
			nextLine = true;
		}
		if (nextLine)
		{
			if (c == '\n')
			{
				key.clear();
				value.clear();
				flag = false;
				nextLine = false;
			}
			continue;
		}
		if (c == '=')
		{
			flag = true;
		}
		else if (c == '\n')
		{
			if (key != "" && value != "")
			{
				m_characterDic.insert(std::map<std::string, std::string>::value_type(key, value));
			}
			key.clear();
			value.clear();
			flag = false;
			nextLine = false;
		}
		else if (c == '\r')
		{
		}
		else
		{
			if (flag)
				value += c;
			else
				key += c;
		}
	}
	//最后一行
	if (key != "" && value != "")
	{
		m_characterDic.insert(std::map<std::string, std::string>::value_type(key, value));
	}
	CC_SAFE_DELETE_ARRAY(temp);
}
std::map<std::string, std::string>* InternationManager::getAllCharacterText()
{
	return &m_characterDic;
}
std::string InternationManager::getCharacterText(std::string key)
{
	std::map<std::string, std::string>::iterator it = m_characterDic.find(key);
	if (it != m_characterDic.end())
	{
		return it->second;
	}
	return "";
}