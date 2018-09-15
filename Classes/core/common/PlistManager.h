#pragma once

#include "cocos2d.h"

class UIPlistFrame
{
public:
	cocos2d::Rect m_frame;//切图矩形
	cocos2d::Point m_offset;//偏移
	bool m_rateted;//是否旋转
	cocos2d::Size m_sourceSize;//原始尺寸
public:
	UIPlistFrame();
	void encode(unsigned char*& data, unsigned long& pos);
};


class PlistManager
{
public:
	static PlistManager& getInstance()
	{
		static PlistManager instance;
		return instance;
	}

	void addUISpriteFrames(const char* plistPath);//添加UI的spriteFrame，plistPath：plist路径
	void removeUISpirteFrames(const char* plistPath);//移除UI的spriteFrame

private:
	PlistManager();
	PlistManager(const PlistManager&);
	//PlistManager& operator=(const PlistManager&);
	~PlistManager();

private:
	std::map<std::string, std::vector<std::string> > m_mapUIPlistDic;//已经加载了的ui plist
};
