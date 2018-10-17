#pragma once
#include "cocos2d.h" 
class FixUI
{
public:    
	FixUI();
	~FixUI();
	static void init(cocos2d::Size& designSize);
	static void setFixScale(cocos2d::Node* node);        
	static void setScaleMin(cocos2d::Node* node);
	static void setScaleMax(cocos2d::Node* node);
	static void setRootNodeWithFixed(cocos2d::Node* node);
	static void fixScene(cocos2d::Node* node);
	static void fixUI(cocos2d::Node* node);

private:
	static cocos2d::Size s_screenSize;
	static cocos2d::Size s_designSize;
	static float s_xScale;
	static float s_yScale;
	static float s_minScale;
	static float s_maxScale;
};
