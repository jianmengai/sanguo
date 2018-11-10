#include "GameUtils.h"

std::map<std::string, cocos2d::Animation*> GameUtils::s_animationCache;
cocos2d::Size GameUtils::s_designResolutionSize;
int GameUtils::s_currentId = 0;

cocos2d::Animation* GameUtils::createAnimationWithPList(const std::string& plistFileName)
{
	cocos2d::Animation* animation = nullptr;

	CCASSERT(cocos2d::FileUtils::getInstance()->isFileExist(plistFileName), cocos2d::StringUtils::format("%s is invalid", plistFileName.c_str()).c_str());

	auto animationIter = s_animationCache.find(plistFileName);
	if (animationIter == s_animationCache.end())
	{
		cocos2d::SpriteFrameCache::getInstance()->addSpriteFramesWithFile(plistFileName);

		cocos2d::ValueMap plistDataMap = cocos2d::FileUtils::getInstance()->getValueMapFromFile(plistFileName);

		animation = parseAnimationValueMap(plistDataMap);
		s_animationCache[plistFileName] = animation;
		animation->retain();
	}
	else
	{
		animation = animationIter->second->clone();
	}

	return animation;
}

cocos2d::Animation* GameUtils::parseAnimationValueMap(cocos2d::ValueMap& valueMap)
{
	auto animation = cocos2d::Animation::create();

	auto framesIter = valueMap.find("frames");
	CCASSERT(framesIter != valueMap.end(), "plist file has not frames key");

	auto& framesMap = valueMap["frames"].asValueMap();
	std::vector<std::string> frameKeysList;
	for (auto& iter : framesMap)
	{
		frameKeysList.push_back(iter.first);
	}

	std::sort(frameKeysList.begin(), frameKeysList.end());

	for (auto& key : frameKeysList)
	{
		auto spriteFrame = cocos2d::SpriteFrameCache::getInstance()->getSpriteFrameByName(key);
		if (spriteFrame)
		{
			animation->addSpriteFrame(spriteFrame);
		}
	}

	return animation;
}


float GameUtils::computeRotatedDegree(const cocos2d::Vec2& beginPosition, const cocos2d::Vec2& endPosition)
{
	float rotation = 0.0f;

	auto moveVector = endPosition - beginPosition;
	if (moveVector.x > -FLT_EPSILON && moveVector.x < FLT_EPSILON)
	{
		if (moveVector.y >= 0.0f)
		{
			rotation = 0.0f;
		}
		else
		{
			rotation = 180.0f;
		}

		return rotation;
	}
	else if (moveVector.y > -FLT_EPSILON && moveVector.y < FLT_EPSILON)
	{
		if (moveVector.x >= 0.0f)
		{
			rotation = 90.0f;
		}
		else
		{
			rotation = 270.0f;
		}

		return rotation;
	}

	float tanTheta = moveVector.x / moveVector.y;
	if (tanTheta > 0.0f)
	{
		if (moveVector.y > 0.0f)
		{
			rotation = atan(tanTheta) * 180.0f / M_PI;
		}
		else
		{
			rotation = atan(tanTheta) * 180.0f / M_PI + 180.0f;
		}
	}
	else
	{
		if (moveVector.x > 0.0f)
		{
			rotation = atan(tanTheta) * 180.0f / M_PI + 180.0f;
		}
		else
		{
			rotation = atan(tanTheta) * 180.0f / M_PI + 360.0f;
		}
	}

	return rotation;
}

float GameUtils::computeDistanceBetween(const cocos2d::Vec2& beginPosition, const cocos2d::Vec2& endPosition)
{
	float distance = sqrt((endPosition.x - beginPosition.x) * (endPosition.x - beginPosition.x) +
		(endPosition.y - beginPosition.y) * (endPosition.y - beginPosition.y));

	return distance;
}

int GameUtils::getLastestUniqId()
{
	++s_currentId;
	return s_currentId;
}


std::string GameUtils::escapeString(const std::string& str)
{
	bool escape = false;
	std::size_t start = 0;
	for (auto it = str.begin(); it != str.end(); ++it)
	{
		if (*it == ' ' || *it == '\t')
		{
			escape = true;
			++start;
			continue;
		}
		else
		{
			break;
		}
	}
	std::size_t end = str.size();
	for (auto it = str.rbegin(); it != str.rend(); ++it)
	{
		if (*it == ' ' || *it == '\t')
		{
			escape = true;
			--end;
			continue;
		}
		else
		{
			break;
		}
	}

	std::size_t len = (end >= start) ? (end - start) : 0;
	if (escape)
	{
		cocos2d::log("raw str:[%s], escape str:[%s]", str.c_str(), str.substr(start, len).c_str());
	}

	return str.substr(start, len);
}