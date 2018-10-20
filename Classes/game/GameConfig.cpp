#include "cocos2d.h"
#include "GameConfig.h"
#include "GameUtils.h"

GameConfig* GameConfig::getInstance()
{
	static GameConfig gameConfigInstance;
	return &gameConfigInstance;
}

bool GameConfig::init()
{
	std::string configFileName = "config.xml";
	std::string fullPath = cocos2d::FileUtils::getInstance()->fullPathForFilename(configFileName);
	tinyxml2::XMLDocument* doc = new (std::nothrow)tinyxml2::XMLDocument;
	if (doc == nullptr)
	{
		cocos2d::log("new tinyxml2::XMLDocument failed");
		return false;
	}
	tinyxml2::XMLError err = doc->LoadFile(fullPath.c_str());
	if (err != 0)
	{
		delete doc;
		doc = nullptr;
		cocos2d::log("XMLDocument load failed, file:%s", fullPath.c_str());
		return false;
	}
	tinyxml2::XMLElement* root = doc->RootElement();
	if (!parseMapConf(root))
	{
		delete doc;
		doc = nullptr;
		return false;
	}
	if (!parseBuildingConf(root))
	{
		delete doc;
		doc = nullptr;
		return false;
	}
	if (!parseSpecialEffectConf(root))
	{
		delete doc;
		doc = nullptr;
		return false;
	}
	if (!parseSoldierConf(root))
	{
		delete doc;
		doc = nullptr;
		return false;
	}
	if (!parseBuildingConf(root))
	{
		delete doc;
		doc = nullptr;
		return false;
	}
	delete doc;
	doc = nullptr;

	if (!loadRes())
	{
		return false;
	}
	return true;
}

bool GameConfig::loadRes()
{
	std::vector<std::string> plistRes;
	plistRes.push_back("BuildingCommon.plist");
	for (auto& it : m_soldierConf)
	{
		SoldierConf* soldierConf = it.second;
		plistRes.push_back(soldierConf->attackToEastAnimationPList);
		plistRes.push_back(soldierConf->attackToNorthEastAnimationPList);
		plistRes.push_back(soldierConf->attackToNorthWestAnimationPList);
		plistRes.push_back(soldierConf->attackToSouthEastAnimationPList);
		plistRes.push_back(soldierConf->attackToSouthWestAnimationPList);
		plistRes.push_back(soldierConf->attackToWestAnimationPList);

		plistRes.push_back(soldierConf->standAndFaceToEastAnimationPList);
		plistRes.push_back(soldierConf->standAndFaceToNorthEastAnimationPList);
		plistRes.push_back(soldierConf->standAndFaceToNorthWestAnimationPList);
		plistRes.push_back(soldierConf->standAndFaceToSouthEastAnimationPList);
		plistRes.push_back(soldierConf->standAndFaceToSouthWestAnimationPList);
		plistRes.push_back(soldierConf->standAndFaceToWestAnimationPList);

		plistRes.push_back(soldierConf->moveToEastAnimationPList);
		plistRes.push_back(soldierConf->moveToNorthEastAnimationPList);
		plistRes.push_back(soldierConf->moveToNorthWestAnimationPList);
		plistRes.push_back(soldierConf->moveToSouthEastAnimationPList);
		plistRes.push_back(soldierConf->moveToSouthWestAnimationPList);
		plistRes.push_back(soldierConf->moveToWestAnimationPList);
	}

	for (auto& it : plistRes)
	{
		cocos2d::SpriteFrameCache::getInstance()->addSpriteFramesWithFile(it);
		if (!GameUtils::createAnimationWithPList(it))
		{
			cocos2d::log("createAnimationWithPList failed, file:%s", it.c_str());
			return false;
		}
	}

	return true;
}

bool GameConfig::parseMapConf(const tinyxml2::XMLElement* node)
{
	if (node == nullptr)
	{
		return false;
	}
	const tinyxml2::XMLElement* mapNode = node->FirstChildElement("Map");

	return true;
}

bool GameConfig::parseBulletConf(const tinyxml2::XMLElement* node)
{
	if (node == nullptr)
	{
		return false;
	}
	const tinyxml2::XMLElement* bulletNode = node->FirstChildElement("Bullet");
	
	return true;
}

bool GameConfig::parseSpecialEffectConf(const tinyxml2::XMLElement* node)
{
	if (node == nullptr)
	{
		return false;
	}
	const tinyxml2::XMLElement* specialEffectNode = node->FirstChildElement("SpecialEffect");

	return true;
}

bool GameConfig::parseSoldierConf(const tinyxml2::XMLElement* node)
{
	if (node == nullptr)
	{
		return false;
	}
	const tinyxml2::XMLElement* soldierNode = node->FirstChildElement("Soldier");
	if (soldierNode == nullptr)
	{
		return false;
	}
	const tinyxml2::XMLElement* brotherNode = soldierNode->FirstChildElement();
	while (brotherNode != nullptr)
	{
		const char* p = brotherNode->Attribute("type");
		if (p == nullptr)
		{
			break;
		}
		SoldierType soldierType = static_cast<SoldierType>(atoi(p));
		
		SoldierConf* soldierConf = new (std::nothrow)SoldierConf;
		if (soldierConf == nullptr)
		{
			break;
		}

		soldierConf->attackToEastAnimationPList = GameUtils::escapeString(brotherNode->Attribute("atkToEPlist"));
		soldierConf->attackToNorthEastAnimationPList = GameUtils::escapeString(brotherNode->Attribute("atkToNEPlist"));
		soldierConf->attackToNorthWestAnimationPList = GameUtils::escapeString(brotherNode->Attribute("atkToSEPlist"));
		soldierConf->attackToSouthEastAnimationPList = GameUtils::escapeString(brotherNode->Attribute("atkToWPlist"));
		soldierConf->attackToSouthWestAnimationPList = GameUtils::escapeString(brotherNode->Attribute("atkToNWPlist"));
		soldierConf->attackToWestAnimationPList = GameUtils::escapeString(brotherNode->Attribute("atkToSWPlist"));

		soldierConf->standAndFaceToEastAnimationPList = GameUtils::escapeString(brotherNode->Attribute("standFaceToEPlist"));
		soldierConf->standAndFaceToNorthEastAnimationPList = GameUtils::escapeString(brotherNode->Attribute("standFaceToNEPlist"));
		soldierConf->standAndFaceToNorthWestAnimationPList = GameUtils::escapeString(brotherNode->Attribute("standFaceToNWPlist"));
		soldierConf->standAndFaceToSouthEastAnimationPList = GameUtils::escapeString(brotherNode->Attribute("standFaceToSEPlist"));
		soldierConf->standAndFaceToSouthWestAnimationPList = GameUtils::escapeString(brotherNode->Attribute("standFaceToSWPlist"));
		soldierConf->standAndFaceToWestAnimationPList = GameUtils::escapeString(brotherNode->Attribute("standFaceToWPlist"));

		soldierConf->moveToNorthEastAnimationPList = GameUtils::escapeString(brotherNode->Attribute("moveToNEPlist"));
		soldierConf->moveToEastAnimationPList = GameUtils::escapeString(brotherNode->Attribute("moveToEPlist"));
		soldierConf->moveToSouthEastAnimationPList = GameUtils::escapeString(brotherNode->Attribute("moveToSEPlist"));
		soldierConf->moveToSouthWestAnimationPList = GameUtils::escapeString(brotherNode->Attribute("moveToSWPlist"));
		soldierConf->moveToWestAnimationPList = GameUtils::escapeString(brotherNode->Attribute("moveToWPlist"));
		soldierConf->moveToNorthWestAnimationPList = GameUtils::escapeString(brotherNode->Attribute("moveToNWPlist"));

		soldierConf->dieAnimationPList = GameUtils::escapeString(brotherNode->Attribute("diePlist"));

		soldierConf->maxHp = atoi(brotherNode->Attribute("maxHp"));
		soldierConf->attackPower = atoi(brotherNode->Attribute("atkPower"));
		soldierConf->maxAttackRadius = atoi(brotherNode->Attribute("maxAtkRadius"));
		soldierConf->maxAlertRadius = atoi(brotherNode->Attribute("maxAlertRadius"));
		soldierConf->perSecondAttackCount = atoi(brotherNode->Attribute("atkSpeed"));
		soldierConf->perSecondMoveSpeedByPixel = atof(brotherNode->Attribute("moveSpeed"));
		soldierConf->bulletType = static_cast<BulletType>(atoi(brotherNode->Attribute("bulletType")));
		soldierConf->damageType = static_cast<DamageType>(atoi(brotherNode->Attribute("damageType")));
		soldierConf->aoeDamageRadius = atof(brotherNode->Attribute("aoeDamageRadius"));
		soldierConf->standAnimateDelayPerUnit = atof(brotherNode->Attribute("standAnimatePerUnit"));
		soldierConf->moveAnimateDelayPerUnit = atof(brotherNode->Attribute("moveAnimatePerUnit"));
		soldierConf->dieAnimateDelayPerUnit = atof(brotherNode->Attribute("dieAnimatePerUnit"));
		m_soldierConf[soldierType] = soldierConf;
		brotherNode = brotherNode->NextSiblingElement();
	}
	return true;
}

bool GameConfig::parseBuildingConf(const tinyxml2::XMLElement* node)
{
	if (node == nullptr)
	{
		return false;
	}
	const tinyxml2::XMLElement* buildingNode = node->FirstChildElement("Building");
	if (buildingNode == nullptr)
	{
		return false;
	}
	const tinyxml2::XMLElement* brotherNode = buildingNode->FirstChildElement();
	while (brotherNode != nullptr)
	{
		const char* p = brotherNode->Attribute("type");
		if (p == nullptr)
		{
			break;
		}
		BuildingType buildingType = static_cast<BuildingType>(atoi(p));
		BuildingConf* buildingConf = new (std::nothrow)BuildingConf;
		if (buildingConf == nullptr)
		{
			break;
		}
		buildingConf->prepareToBuildStatusTextureName = GameUtils::escapeString(brotherNode->Attribute("prepareTexture"));
		buildingConf->beingBuiltStatusTextureName = GameUtils::escapeString(brotherNode->Attribute("beingTexture"));
		buildingConf->workingStatusTextureName = GameUtils::escapeString(brotherNode->Attribute("workingTexture"));
		buildingConf->destroyStatusTextureName = GameUtils::escapeString(brotherNode->Attribute("destroyTexture"));
		buildingConf->shadowTextureName = GameUtils::escapeString(brotherNode->Attribute("shadowTexture"));
		buildingConf->destroySpecialEffectTemplateName = GameUtils::escapeString(brotherNode->Attribute("destroySpecialEffect"));
		buildingConf->maxHP = atoi(brotherNode->Attribute("maxHp"));
		buildingConf->attackPower = atof(brotherNode->Attribute("atkPower"));
		buildingConf->attackRange = atof(brotherNode->Attribute("maxAtkRadius"));
		buildingConf->maxCount = atoi(brotherNode->Attribute("maxCount"));
		m_buildingConf[buildingType] = buildingConf;
		brotherNode = brotherNode->NextSiblingElement();
	}
	return true;
}

MapConf* GameConfig::getMapConf(int mapId)
{
	auto it = m_mapConf.find(mapId);
	if (it != m_mapConf.end())
	{
		return it->second;
	}

	return nullptr;
}

BulletConf* GameConfig::getBulletConf(const BulletType type)
{
	auto it = m_bulletConf.find(type);
	if (it != m_bulletConf.end())
	{
		return it->second;
	}

	return nullptr;
}

SpecialEffectConf* GameConfig::getSpecialEffectConf(const std::string& specialEffectName)
{
	auto it = m_specialEffectConf.find(specialEffectName);
	if (it != m_specialEffectConf.end())
	{
		return it->second;
	}

	return nullptr;
}

SoldierConf* GameConfig::getSoldierConf(const SoldierType type)
{
	auto it = m_soldierConf.find(type);
	if (it != m_soldierConf.end())
	{
		return it->second;
	}

	return nullptr;
}

BuildingConf* GameConfig::getBuildingConf(const BuildingType type)
{
	auto it = m_buildingConf.find(type);
	if (it != m_buildingConf.end())
	{
		return it->second;
	}

	return nullptr;
}