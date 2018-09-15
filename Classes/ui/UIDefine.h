#pragma once
#include "cocos2d.h"
#include "CoreDefine.h"

constexpr auto BG_TRANSPARENT_42_42 = "bg_transparent_42_42.png"; //全透明背景
#define BG_TRANSPARENT_42_42_CCRect  cocos2d::Rect(5,5,32,32)

const cocos2d::Color3B CH_GREEN = { 0xb2,0xd2,0x35 };//绿色

constexpr auto LOADING_IMAGE = "logo.png";
constexpr auto LOGIN_LOGO = "ui/login/login_logo.png";
constexpr auto LOGIN_BG = "logo.png";

constexpr auto LOGIN_PLIST = "ui/login.plist";
constexpr auto UI_COMMON_SUDOKU_1_PLIST = "ui/ui_common_sudoku_1.plist";  //共通九宫格图片1

constexpr auto LOGIN_INPUT = "login_edit.png";
constexpr auto LOGIN_ACCOUNT_DES = "login_account.png";
constexpr auto LOGIN_PWD_DES = "login_pwd.png";
constexpr auto LOGIN_NORMAL = "login_login_normal.png";
constexpr auto LOGIN_SELECTED = "login_login_selected.png";
constexpr auto LOGIN_ROTATE_BIG = "login_rotate_b.png";