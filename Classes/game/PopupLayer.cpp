#include "PopupLayer.h"

cocos2d::Scene * PopupLayer::scene()
{
	cocos2d::Scene* scene = nullptr;
	do
	{
		scene = cocos2d::Scene::create();
		PopupLayer* layer = PopupLayer::create();
		scene->addChild(layer);
	} while (0);

	return scene;
}

bool PopupLayer::init()
{
	bool bRet = false;
	do
	{
		if(!cocos2d::Layer::init());
		{
			return false;
		}
		auto winSize = cocos2d::Director::getInstance()->getWinSize();

		//设置这个层的背景图片，并且设置其位置为整个屏幕的中点
		auto background = cocos2d::Sprite::create("background.png");
		m_bgSprite = background;
		background->setPosition(cocos2d::Vec2(winSize.width / 2, winSize.height / 2));
		this->addChild(background);

		//获得背景图片的大小
		auto contentSize = background->getContentSize();
		m_size = contentSize;

		//添加俩个菜单在这个层中
		CCMenuItemImage * item1 = CCMenuItemImage::create("btn-play-normal.png",
			"btn-play-selected.png", "",
			this, menu_selector(PopupLayer::yesButton));

		CCMenuItemImage * item2 = CCMenuItemImage::create("btn-highscores-normal.png",
			"btn-highscores-selected.png", "",
			this, menu_selector(PopupLayer::noButton));

		CCMenu * menu = CCMenu::create(item1, item2, NULL);
		menu->alignItemsHorizontallyWithPadding(5);
		menu->setPosition(ccp(contentSize.width / 2, contentSize.height / 3));
		//kCCMenuHandlerPriority的值为-128，代表的是菜单按钮的触摸优先级
		//设置menu优先级，这里设置为普通menu的二倍减一，原因看下边
		menu->setTouchPriority(kCCMenuHandlerPriority * 2 - 1);

		background->addChild(menu);

		//设置题目和文本内容
		this->setTitle();
		this->setContent();

		this->setTouchEnabled(true);
		bRet = true;
	} while (0);

	return bRet;
}

void PopupLayer::registerWithTouchDispatcher()
{
	//kCCMenuHandlerPriority=-128，将这个值设置为-128的二倍，可以比下边的层的优先级高
	//而且ccTouchBegan的返回值为true，说明其他的层将接受不到这个触摸消息了，只有这个层上边的
	//菜单的优先级比他还要打，所以它上边的菜单是可以接收到触摸消息的
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this,
		kCCMenuHandlerPriority * 2, true);
}

bool PopupLayer::ccTouchBegan(CCTouch * touch, CCEvent * pevent)
{
	return true;
}

//点击菜单按钮的时候调用的事件处理函数
void PopupLayer::yesButton(CCObject * object)
{
	this->removeFromParentAndCleanup(true);
}

void PopupLayer::noButton(CCObject * object)
{
	this->removeFromParentAndCleanup(true);
}

//设置这个层的题目
void PopupLayer::setTitle()
{
	//CCLabelTTF * title = CCLabelTTF::create("Tips","",24);
	CCLabelBMFont * title = CCLabelBMFont::create("Tips", "bitmapFontChinese.fnt");
	title->setPosition(ccp(m_size.width / 2, m_size.height - title->getContentSize().height / 2));
	m_bgSprite->addChild(title);
}

//设置层的内容
void PopupLayer::setContent()
{
	CCLabelTTF * content = CCLabelTTF::create("hello! everyone,welcome to www.jb51.net",
		"", 24);
	content->setPosition(ccp(m_size.width / 2, m_size.height / 2));
	//设置ttf的文本域
	content->setDimensions(CCSize(this->m_size.width - 60, this->m_size.height - 100));
	//设置ttf的水平对齐方式
	content->setHorizontalAlignment(kCCTextAlignmentLeft);

	m_bgSprite->addChild(content);
}