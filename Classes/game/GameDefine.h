#pragma once

constexpr auto LAST_ACCOUNT_NAME = "last-accout-name";
constexpr auto LAST_PASSWORD = "last-pass-word";

constexpr auto ENABLE_BUILD_GRID_FILE_NAME = "EnableBuildGBrid.png";

constexpr auto MAX_GAME_OBJECT_COUNT = 1000;


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
	Barrack,        //��Ӫ
	DefenceTower,   //��������
	MainTown        //����
};


enum class FaceDirection
{
	Invalid = 0,

	FaceToEast,        //��
	FaceToNorthEast,   //����
	FaceToNorthWest,   //����
	FaceToSouthEast,   //����
	FaceToSouthWest,   //����
	FaceToWest,        //��

	Total
};

enum class SoldierType
{
	Invalid = 0,

	Infantry,     //����
	Archer,       //������
	Cavalry,      //���

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


enum class UIEffectType
{
	ReinforcePointIncrease,
	TrainFinished,
	ButtonClick
};
