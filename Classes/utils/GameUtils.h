#pragma once

#include "cocos2d.h"

class GameUtils
{
public:
	static cocos2d::Animation* createAnimationWithPList(const std::string& plistFileName);
	static float computeRotatedDegree(const cocos2d::Vec2& beginPosition, const cocos2d::Vec2& endPosition);
	static float computeDistanceBetween(const cocos2d::Vec2& beginPosition, const cocos2d::Vec2& endPosition);

	static int getLastestUniqId();

	static cocos2d::Size& getDesignResolutionSize() { return s_designResolutionSize; }
	static void setDesignResolutionSize(cocos2d::Size& designResolutionSize) { s_designResolutionSize = designResolutionSize; }

	static std::string escapeString(const std::string& str);

private:
	static cocos2d::Animation* parseAnimationValueMap(cocos2d::ValueMap& valueMap);

private:
	static std::map<std::string, cocos2d::Animation*> s_animationCache;
	static int s_currentId;
	static cocos2d::Size s_designResolutionSize;
};