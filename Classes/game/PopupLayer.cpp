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

		//���������ı���ͼƬ������������λ��Ϊ������Ļ���е�
		auto background = cocos2d::Sprite::create("background.png");
		m_bgSprite = background;
		background->setPosition(cocos2d::Vec2(winSize.width / 2, winSize.height / 2));
		this->addChild(background);

		//��ñ���ͼƬ�Ĵ�С
		auto contentSize = background->getContentSize();
		m_size = contentSize;

		//��������˵����������
		CCMenuItemImage * item1 = CCMenuItemImage::create("btn-play-normal.png",
			"btn-play-selected.png", "",
			this, menu_selector(PopupLayer::yesButton));

		CCMenuItemImage * item2 = CCMenuItemImage::create("btn-highscores-normal.png",
			"btn-highscores-selected.png", "",
			this, menu_selector(PopupLayer::noButton));

		CCMenu * menu = CCMenu::create(item1, item2, NULL);
		menu->alignItemsHorizontallyWithPadding(5);
		menu->setPosition(ccp(contentSize.width / 2, contentSize.height / 3));
		//kCCMenuHandlerPriority��ֵΪ-128��������ǲ˵���ť�Ĵ������ȼ�
		//����menu���ȼ�����������Ϊ��ͨmenu�Ķ�����һ��ԭ���±�
		menu->setTouchPriority(kCCMenuHandlerPriority * 2 - 1);

		background->addChild(menu);

		//������Ŀ���ı�����
		this->setTitle();
		this->setContent();

		this->setTouchEnabled(true);
		bRet = true;
	} while (0);

	return bRet;
}

void PopupLayer::registerWithTouchDispatcher()
{
	//kCCMenuHandlerPriority=-128�������ֵ����Ϊ-128�Ķ��������Ա��±ߵĲ�����ȼ���
	//����ccTouchBegan�ķ���ֵΪtrue��˵�������Ĳ㽫���ܲ������������Ϣ�ˣ�ֻ��������ϱߵ�
	//�˵������ȼ�������Ҫ���������ϱߵĲ˵��ǿ��Խ��յ�������Ϣ��
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this,
		kCCMenuHandlerPriority * 2, true);
}

bool PopupLayer::ccTouchBegan(CCTouch * touch, CCEvent * pevent)
{
	return true;
}

//����˵���ť��ʱ����õ��¼�������
void PopupLayer::yesButton(CCObject * object)
{
	this->removeFromParentAndCleanup(true);
}

void PopupLayer::noButton(CCObject * object)
{
	this->removeFromParentAndCleanup(true);
}

//������������Ŀ
void PopupLayer::setTitle()
{
	//CCLabelTTF * title = CCLabelTTF::create("Tips","",24);
	CCLabelBMFont * title = CCLabelBMFont::create("Tips", "bitmapFontChinese.fnt");
	title->setPosition(ccp(m_size.width / 2, m_size.height - title->getContentSize().height / 2));
	m_bgSprite->addChild(title);
}

//���ò������
void PopupLayer::setContent()
{
	CCLabelTTF * content = CCLabelTTF::create("hello! everyone,welcome to www.jb51.net",
		"", 24);
	content->setPosition(ccp(m_size.width / 2, m_size.height / 2));
	//����ttf���ı���
	content->setDimensions(CCSize(this->m_size.width - 60, this->m_size.height - 100));
	//����ttf��ˮƽ���뷽ʽ
	content->setHorizontalAlignment(kCCTextAlignmentLeft);

	m_bgSprite->addChild(content);
}