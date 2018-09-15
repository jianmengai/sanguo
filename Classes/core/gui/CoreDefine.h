#pragma once


constexpr auto FONT_NAME_CONSOLAS = "Consolas";
constexpr auto FONT_NAME_ARIAL = "Arial";
constexpr auto FONT_NAME_THONBURI = "Thonburi";
constexpr auto FONT_NAME_HELVETICA = "Helvetica";
constexpr auto FONT_NAME_MICROSOFT_YAHEI = "΢���ź�";

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
constexpr auto FONT_NAME_DEFAULT = "fonts/msyh.ttf";
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
constexpr auto FONT_NAME_DEFAULT = "_H_Helvetica";
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
constexpr auto FONT_NAME_DEFAULT = "Helvetica";
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
constexpr auto FONT_NAME_DEFAULT = "DroidSansFallback";
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
constexpr auto FONT_NAME_DEFAULT = "FreeSerif";
#else
constexpr auto FONT_NAME_DEFAULT = "";
#endif

constexpr auto FONT_SIZE_20 = 20;
constexpr auto FONT_SIZE_24 = 24;

constexpr auto ALIGN_ITEMS_HORIZONTALLY_TOP   = 0; //ˮƽ���֣���
constexpr auto ALIGN_ITEMS_VERTICALLY_LEFT    = 1; //��ֱ���֣���

constexpr auto ALIGN_ITEM_HORIZONTALLY_LEFT   = 0; //ˮƽ����
constexpr auto ALIGN_ITEM_HORIZONTALLY_CENTER = 1; //ˮƽ����
constexpr auto ALIGN_ITEM_HORIZONTALLY_RIGHT  = 2; //ˮƽ����

constexpr auto TEXT_ALIGNMENT_LEFT = 1;    //������ͼƬ�ⲿ���
constexpr auto TEXT_ALIGNMENT_CENTER = 2;	//������ͼƬ�ڲ��м�
constexpr auto TEXT_ALIGNMENT_RIGHT = 3;	//������ͼƬ�ⲿ�ұ�
