#include "FixUI.h"
#include "GameUtils.h"

cocos2d::Size FixUI::s_screenSize;
cocos2d::Size FixUI::s_designSize;
float FixUI::s_xScale = 1.0f;
float FixUI::s_yScale = 1.0f;
float FixUI::s_minScale = 1.0f;
float FixUI::s_maxScale = 1.0f;



FixUI::FixUI() 
{

}

FixUI::~FixUI() 
{

}

void FixUI::init(cocos2d::Size& designSize)
{
	s_designSize = designSize;//GameUtils::getDesignResolutionSize();
	s_screenSize = cocos2d::Director::getInstance()->getVisibleSize();
	s_xScale = s_designSize.width / s_screenSize.width;
	s_yScale = s_designSize.height / s_screenSize.height;
	s_minScale = MIN(s_screenSize.height / s_designSize.height, s_screenSize.width / s_designSize.width);
	s_maxScale = MAX(s_screenSize.height / s_designSize.height, s_screenSize.width / s_designSize.width);

	cocos2d::log("xScale = %f * xyScale = %f \n screenSize.width = %f * screenSize.height = %f \n", s_xScale, s_yScale, s_screenSize.width, s_screenSize.height);

}

//1、先适配layout层到屏幕大小
void FixUI::setFixScale(cocos2d::Node *node) 
{
	auto nodeX = node->getScaleX();
	auto nodeY = node->getScaleY();
	cocos2d::log("setFixScale nodeX = %f * nodeX = %f \n", nodeX, nodeY);
	nodeX = nodeX * s_xScale;
	nodeY = nodeY * s_yScale;

	cocos2d::log("setFixScale nodeX = %f * nodeX = %f \n", nodeX, nodeY);
	node->setScaleX(nodeX);
	node->setScaleY(nodeY);
}

//屏幕宽、高分别和设计分辨率宽、高计算缩放因子，取较(大)者作为宽、高的缩放因子。
//适用于控件的缩放
void FixUI::setScaleMax(cocos2d::Node *node) 
{
	auto nodeX = node->getScaleX();
	auto nodeY = node->getScaleY();
	cocos2d::log("setScaleMax nodeX = %f * nodeX = %f \n", nodeX, nodeY);

	nodeX = nodeX * s_maxScale;
	nodeY = nodeY * s_maxScale;
	cocos2d::log("setScaleMax nodeX = %f * nodeX = %f \n", nodeX, nodeY);

	node->setScaleX(nodeX);
	node->setScaleY(nodeY);
}

//屏幕宽、高分别和设计分辨率宽、高计算缩放因子，取较(小)者作为宽、高的缩放因子。
//适用于背景的缩放
void FixUI::setScaleMin(cocos2d::Node *node) 
{
	auto nodeX = node->getScaleX();
	auto nodeY = node->getScaleY();
	cocos2d::log("setScaleMin nodeX = %f * nodeX = %f \n", nodeX, nodeY);

	nodeX = nodeX * s_minScale;
	nodeY = nodeY * s_minScale;
	cocos2d::log("setScaleMin nodeX = %f * nodeX = %f \n", nodeX, nodeY);

	node->setScaleX(nodeX);
	node->setScaleY(nodeY);
}


void FixUI::setRootNodeWithFixed(cocos2d::Node* node) 
{
	auto moveX = (s_designSize.width - s_screenSize.width);
	auto moveY = (s_designSize.height - s_screenSize.height);
	cocos2d::log("=====setPosition x:%0.1f, y:%0.1f=====", moveX, moveY);
	node->setPosition(cocos2d::Vec2(moveX, moveY));
}

void FixUI::fixScene(cocos2d::Node* node) 
{
	auto pos = node->getPosition();
	cocos2d::log("=====pre position x:%0.1f, y:%0.1f=====", pos.x, pos.y);
	fixUI(node);
	FixUI::setRootNodeWithFixed(node);
}


void FixUI::fixUI(cocos2d::Node* node) 
{
	cocos2d::Vector<cocos2d::Node*> childrenList = node->getChildren();

	//for (auto child : childrenList) {
		//cocos2d::log("fixUI nodeX = %d \n", child->getTag());
		FixUI::setScaleMin(node);
	//}

}

