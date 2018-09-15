#include "ReadUtil.h"
#include "PlistManager.h"

UIPlistFrame::UIPlistFrame()
{
	m_frame = cocos2d::Rect::ZERO;
	m_offset = cocos2d::Point(0, 0);
	m_rateted = false;
	m_sourceSize = cocos2d::Size::ZERO;
}

void UIPlistFrame::encode(unsigned char*& data, unsigned long& pos)
{
	short x = 0;
	short y = 0;
	short w = 0;
	short h = 0;
	pos += readShort(data, x);
	pos += readShort(data, y);
	pos += readShort(data, w);
	pos += readShort(data, h);
	m_frame = cocos2d::Rect(x, y, w, h);

	short px = 0;
	short py = 0;
	pos += readShort(data, px);
	pos += readShort(data, py);
	m_offset = cocos2d::Point(px, py);

	pos += readBool(data, m_rateted);

	pos += readShort(data, px);
	pos += readShort(data, py);
	m_sourceSize = cocos2d::CCSizeMake(px, py);
}



PlistManager::PlistManager()
{

}
PlistManager::~PlistManager()
{
	std::map<std::string, std::vector<std::string> > blank;
	m_mapUIPlistDic.swap(blank);
}

void PlistManager::addUISpriteFrames(const char* plistPath)
{
	auto it = m_mapUIPlistDic.find(plistPath);
	if (it == m_mapUIPlistDic.end())//不存在就加载
	{
		ssize_t fileLen = 0;
		unsigned char *fileData = nullptr;
		fileData = cocos2d::FileUtils::getInstance()->getFileData(plistPath, "rb", &fileLen);
		if (!fileData)
		{
			return;
		}
		unsigned char* temp = fileData;
		unsigned long pos = 0;
		std::string picNameStr = "";
		pos += readString(fileData, picNameStr);
		picNameStr = cocos2d::FileUtils::getInstance()->fullPathFromRelativeFile(picNameStr.c_str(), plistPath);
		cocos2d::Texture2D* pobTexture = cocos2d::TextureCache::getInstance()->addImage(picNameStr.c_str());
		if (pobTexture)
		{
			std::vector<std::string> nameList;
			UIPlistFrame uiPlistframe;
			cocos2d::SpriteFrame* spriteFrame;
			std::string spriteFrameName;
			short len;
			pos += readShort(fileData, len);
			for (short i = 0; i < len; i++)
			{
				pos += readString(fileData, spriteFrameName);
				uiPlistframe.encode(fileData, pos);
				spriteFrame = new cocos2d::SpriteFrame();
				spriteFrame->initWithTexture(pobTexture, \
					uiPlistframe.m_frame, \
					uiPlistframe.m_rateted, \
					uiPlistframe.m_offset, \
					uiPlistframe.m_sourceSize);
				cocos2d::SpriteFrameCache::getInstance()->addSpriteFrame(spriteFrame, spriteFrameName.c_str());
				spriteFrame->release();
				nameList.push_back(spriteFrameName);
			}

			m_mapUIPlistDic[plistPath] = nameList;
		}
		CC_SAFE_DELETE_ARRAY(temp);
	}
}

void PlistManager::removeUISpirteFrames(const char* plistPath)
{
	auto it = m_mapUIPlistDic.find(plistPath);
	if (it != m_mapUIPlistDic.end())
	{
		for (auto& listIt : it->second)
		{
			cocos2d::SpriteFrameCache::getInstance()->removeSpriteFrameByName(listIt);
		}
		m_mapUIPlistDic.erase(it);
	}
}
