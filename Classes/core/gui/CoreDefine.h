#pragma once


constexpr auto FONT_NAME_CONSOLAS = "Consolas";
constexpr auto FONT_NAME_ARIAL = "Arial";
constexpr auto FONT_NAME_THONBURI = "Thonburi";
constexpr auto FONT_NAME_HELVETICA = "Helvetica";
constexpr auto FONT_NAME_MICROSOFT_YAHEI = "微软雅黑";

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

constexpr auto ALIGN_ITEMS_HORIZONTALLY_TOP   = 0; //水平布局，上
constexpr auto ALIGN_ITEMS_VERTICALLY_LEFT    = 1; //垂直布局，左

constexpr auto ALIGN_ITEM_HORIZONTALLY_LEFT   = 0; //水平居左
constexpr auto ALIGN_ITEM_HORIZONTALLY_CENTER = 1; //水平居中
constexpr auto ALIGN_ITEM_HORIZONTALLY_RIGHT  = 2; //水平居右

constexpr auto TEXT_ALIGNMENT_LEFT = 1;    //文字在图片外部左边
constexpr auto TEXT_ALIGNMENT_CENTER = 2;	//文字在图片内部中间
constexpr auto TEXT_ALIGNMENT_RIGHT = 3;	//文字在图片外部右边
