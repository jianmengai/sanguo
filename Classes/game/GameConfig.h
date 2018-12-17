#pragma once
#include <string>
#include <map>
#include "tinyxml2/tinyxml2.h"
#include "GameDefine.h"


typedef struct bulletConf_st
{
	std::string fileName;
	std::string specialEffect;
	float speed;
}BulletConf;

typedef struct specialEffectConf_st
{
	std::string animationPListName;
	float perUnitIntervalBySecond = 0.0f;
}SpecialEffectConf;

typedef struct mapConf_st
{

}MapConf;

typedef struct soldierConf_st
{
	std::string moveToNorthEastAnimationPList;
	std::string moveToEastAnimationPList;
	std::string moveToSouthEastAnimationPList;
	std::string moveToSouthWestAnimationPList;
	std::string moveToWestAnimationPList;
	std::string moveToNorthWestAnimationPList;

	std::string dieAnimationPList;

	std::string standAndFaceToNorthEastAnimationPList;
	std::string standAndFaceToEastAnimationPList;
	std::string standAndFaceToSouthEastAnimationPList;
	std::string standAndFaceToSouthWestAnimationPList;
	std::string standAndFaceToWestAnimationPList;
	std::string standAndFaceToNorthWestAnimationPList;

	std::string attackToNorthEastAnimationPList;
	std::string attackToEastAnimationPList;
	std::string attackToSouthEastAnimationPList;
	std::string attackToSouthWestAnimationPList;
	std::string attackToWestAnimationPList;
	std::string attackToNorthWestAnimationPList;

	int maxHp = 0;
	int attackPower = 0;
	int maxAttackRadius = 0;
	int maxAlertRadius = 0;
	int perSecondAttackCount = 0;
	float perSecondMoveSpeedByPixel = 0.0f;
	BulletType bulletType = BulletType::Invalid;
	DamageType damageType = DamageType::Invalid;
	float aoeDamageRadius = 0.0f; // 非AOE伤害的Npc无视这个参数

	float shadowYPosition = 0.0f;
	float hpBarYPosition = 0.0f;

	float moveAnimateDelayPerUnit = 0.0f;
	float standAnimateDelayPerUnit = 0.0f;
	float dieAnimateDelayPerUnit = 0.0f;
	float reinforceRadius = 0.0f;

	std::string blueSelectedTipsTextureName;
	std::string redSelectedTipsTextureName;

	int technologyPoint = 0;

	bool isAir = false;
	bool canAirAttack = false;

	int subCount = 1;   //建造栏一个图像代表士兵的个数
}SoldierConf;

typedef struct buildingConf_st
{
	std::string prepareToBuildStatusTextureName;
	std::string beingBuiltStatusTextureName;
	std::string workingStatusTextureName;
	std::string destroyStatusTextureName;
	std::string shadowTextureName;
	std::string destroySpecialEffectTemplateName;
	float shadowYPositionInBeingBuiltStatus = 0.0f;
	float shadowYPositionInWorkingStatus = 0.0f;
	float shadowYPositionInDestroyStatus = 0.0f;
	float buildingTimeBySecond = 0.0f;
	float extraEnemyAttackRadius = 0.0f;
	int maxHP = 0;
	float centerBottomGridYPosition = 0.0f;
	int bottomGridColumnCount = 0;
	int bottomGridRowCount = 0;
	std::string defenceNpcName;
	float defenceNpcYPosition = 0.0f;
	float attackRange = 0.0f;
	float attackPower = 0.0f;

	std::string blueSelectedTipsTextureName;
	std::string redSelectedTipsTextureName;

	int technologyPoint = 0;

	bool canDestroy = true;

	int maxCount = 0;  //最大建造数，0:不限制 
}BuildingConf;

typedef struct cooldownConf_st
{
	float npcFindTargetCdTime = 0;
	float alertCdTime = 0;
	float updateVisible = 0;
}CooldownConf;

typedef struct buildingSoundEffectData_st
{
	std::string destroyedName;
	std::string constructName;
}BuildingSoundEffectData;

typedef struct soldierSoundEffectData_st
{
	std::string attackName;
	std::string deathName;
	std::string selectName;
	std::string moveName;
}SoldierSoundEffectData;

class GameConfig
{
public:
	static GameConfig* getInstance();
	bool init();

	MapConf* getMapConf(int mapId = 0);
	BulletConf* getBulletConf(const BulletType type);
	SpecialEffectConf* getSpecialEffectConf(const std::string& specialEffectName);
	SoldierConf* getSoldierConf(ForceType forceType, const SoldierType type);
	BuildingConf* getBuildingConf(ForceType forceType, const BuildingType type);
	CooldownConf* getCooldownConf();
	BuildingSoundEffectData* getBuildingSoundEffectConf();
	SoldierSoundEffectData* getSoldierSoundEffectConf(const SoldierType type);

private:
	bool parseMapConf(const tinyxml2::XMLElement* node);
	bool parseBulletConf(const tinyxml2::XMLElement* node);
	bool parseSpecialEffectConf(const tinyxml2::XMLElement* node);
	bool parseSoldierConf(const tinyxml2::XMLElement* node);
	bool parseBuildingConf(const tinyxml2::XMLElement* node);
	bool parseSoundEffectConf(const tinyxml2::XMLElement* node);
	bool parseCooldownConf(const tinyxml2::XMLElement* node);

	bool loadRes();

private:
	std::map<int, MapConf*> m_mapConf;
	std::map<BulletType, BulletConf*> m_bulletConf;
	std::map<std::string, SpecialEffectConf*> m_specialEffectConf;
	std::map<SoldierType, SoldierConf*> m_playerSoldierConf;
	std::map<SoldierType, SoldierConf*> m_npcSoldierConf;
	std::map<BuildingType, BuildingConf*> m_playerBuildingConf;
	std::map<BuildingType, BuildingConf*> m_npcBuildingConf;
	CooldownConf m_coolDownConf;
	BuildingSoundEffectData m_buildingSound;
	std::map<SoldierType,SoldierSoundEffectData*> m_soldierSounds;
};