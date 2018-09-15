#pragma once

#include "cocos2d.h"

class MsgDisplay : public cocos2d::Node
{
public:
	MsgDisplay();
	~MsgDisplay();
	virtual bool init();
	CREATE_FUNC(MsgDisplay);
	void msgDisplay(const std::string& msg);
	void fadeOutCallback();
private:
	cocos2d::Label* m_pMsgLabel;
};