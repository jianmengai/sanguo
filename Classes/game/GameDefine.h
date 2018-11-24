#pragma once

#include "cocos2d.h"

constexpr auto ENABLE_BUILD_GRID_FILE_NAME = "EnableBuildGBrid.png";

constexpr auto MAX_GAME_OBJECT_COUNT = 1000;

constexpr auto BEING_BUILT_PROGRESS_BAR = "PlayerHPBar.png";
constexpr auto PLAYER_HP_BAR_TEXTURE_NAME = "PlayerHPBar.png";
constexpr auto AI_HP_BAR_TEXTURE_NAME = "AIHPBar.png";
constexpr auto HP_BAR_BACKGROUND_TEXTURE_NAME = "HPBarBackground.png";

constexpr auto BUILDING_DELAY_REMOVE_TIME = 2.0f;

constexpr auto INVALID_GAMEOBJECT_ID = 0;

enum class GameObjectType
{
	Invalid = 0,
	Building,
	Soldier,

	Total
};

enum class BuildingType
{
	Invalid = 0,
	Barrack,        //兵营
	DefenceTower,   //防御工事
	MainTown        //主城
};


enum class FaceDirection
{
	Invalid = 0,

	FaceToEast,        //东
	FaceToNorthEast,   //东北
	FaceToNorthWest,   //西北
	FaceToSouthEast,   //东南
	FaceToSouthWest,   //西南
	FaceToWest,        //西

	Total
};

enum class SoldierType
{
	Invalid = 0,

	Infantry,     //步兵
	Archer,       //弓箭手
	Cavalry,      //骑兵

	Total
};

enum class GameObjectStatus
{
	Invalid = 0,

	Move,
	Stand,
	Attack,
	Die,

	Total
};

enum class DamageType
{
	Invalid = 0,

	AreaOfEffect,
	Normal,
};

enum class BulletType
{
	Invalid = 0,

	Arrow,

	Total
};

enum class BuildingStatus
{
	Invalid = 0,

	PrepareToBuild,
	BeingBuilt,
	Working,
	Destory,

	Total
};

enum class ForceType
{
	Invalid,

	Player,
	AI,
};



enum class NpcSoundEffectType
{
	Attack,
	Death,
	Select,
	Move
};

enum class BuildingSoundEffectType
{
	Construct,
	Destroyed
};

enum class UIEffectType
{
	ReinforcePointIncrease,
	TrainFinished,
	ButtonClick
};

enum class TeamNo
{
	Invalid = 0,
	One,
	Two,
	Three,
	Four,
	Five,
	Six,
	Seven,
	Eight,
	Nine
};



struct BasePosition
{
	cocos2d::Vec2 basePosition;
	cocos2d::Vec2 barrackPosition;
	std::vector<cocos2d::Vec2> archorTowerPositions;
};