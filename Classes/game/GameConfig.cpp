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
	cocos2d::log("GameConfig::init");
	std::string configFileName = "config.xml";
	std::string fullPath = cocos2d::FileUtils::getInstance()->fullPathForFilename(configFileName);
	tinyxml2::XMLDocument* doc = new (std::nothrow)tinyxml2::XMLDocument;
	if (doc == nullptr)
	{
		cocos2d::log("new tinyxml2::XMLDocument failed");
		return false;
	}
	ssize_t contentSize = 0;   
	unsigned char* content = cocos2d::FileUtils::getInstance()->getFileData(configFileName, "r", &contentSize);
	//cocos2d::Data data = cocos2d::FileUtils::getInstance()->getDataFromFile(fullPath);
	if (content == nullptr)
	{
		cocos2d::log("load config file failed, file:%s", fullPath.c_str());
		return false;
	}

	tinyxml2::XMLError err = doc->Parse((const char*)content, contentSize); //(fullPath.c_str());
	if (err != 0)
	{
		cocos2d::log("load config file failed, file:%s, err:%d", fullPath.c_str(), static_cast<int>(err));
		delete doc;
		doc = nullptr;
		return false;
	}
	tinyxml2::XMLElement* root = doc->RootElement();
	if (!parseMapConf(root))
	{
		delete doc;
		doc = nullptr;
		return false;
	}
	if (!parseBulletConf(root))
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
	if (!parseSoundEffectConf(root))
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
	for (auto i = 0; i < 2; ++i)
	{
		std::map<SoldierType, SoldierConf*>* pointer = nullptr;
		if (i == 0)
		{
			pointer = &m_playerSoldierConf;
		}
		else
		{
			pointer = &m_npcSoldierConf;
		}
		for (auto& it :*pointer)
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
	if (bulletNode == nullptr)
	{
		return false;
	}
	const tinyxml2::XMLElement* brotherNode = bulletNode->FirstChildElement();
	while (brotherNode != nullptr)
	{
		std::string type = GameUtils::escapeString(brotherNode->Attribute("type"));
		if (type.empty())
		{
			return false;
		}
		BulletType bulletType = BulletType::Invalid;
		if (type == "Arrow")
		{
			bulletType = BulletType::Arrow;
		}
		else if (type == "Magic")
		{
			bulletType = BulletType::Magic;
		}
		BulletConf* bulletConf = new (std::nothrow)BulletConf;
		bulletConf->fileName = GameUtils::escapeString(brotherNode->Attribute("bulletFileName"));
		bulletConf->specialEffect = GameUtils::escapeString(brotherNode->Attribute("specialEffectName"));
		bulletConf->speed = atof(brotherNode->Attribute("speed"));
		m_bulletConf[bulletType] = bulletConf;
		brotherNode = brotherNode->NextSiblingElement();
	}
	
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
	for (auto i = 0; i < 2; ++i)
	{
		const tinyxml2::XMLElement* parentNode = nullptr;
		if (i == 0)
		{
			parentNode = node->FirstChildElement("Player");
		}
		else
		{
			parentNode = node->FirstChildElement("Npc");
		}
		const tinyxml2::XMLElement* soldierNode = parentNode->FirstChildElement("Soldier");
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
			soldierConf->attackToNorthWestAnimationPList = GameUtils::escapeString(brotherNode->Attribute("atkToNWPlist"));
			soldierConf->attackToSouthEastAnimationPList = GameUtils::escapeString(brotherNode->Attribute("atkToSEPlist"));
			soldierConf->attackToSouthWestAnimationPList = GameUtils::escapeString(brotherNode->Attribute("atkToSWPlist"));
			soldierConf->attackToWestAnimationPList = GameUtils::escapeString(brotherNode->Attribute("atkToWPlist"));

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
			soldierConf->technologyPoint = atoi(brotherNode->Attribute("technologyPoint"));
			if (i == 0)
			{
				m_playerSoldierConf[soldierType] = soldierConf;
			}
			else
			{
				m_npcSoldierConf[soldierType] = soldierConf;
			}
			brotherNode = brotherNode->NextSiblingElement();
		}
	}
	
	return true;
}

bool GameConfig::parseBuildingConf(const tinyxml2::XMLElement* node)
{
	if (node == nullptr)
	{
		return false;
	}
	for (auto i = 0; i < 2; ++i)
	{
		const tinyxml2::XMLElement* parentNode = nullptr;
		if (i == 0)
		{
			parentNode = node->FirstChildElement("Player");
		}
		else
		{
			parentNode = node->FirstChildElement("Npc");
		}
		const tinyxml2::XMLElement* buildingNode = parentNode->FirstChildElement("Building");
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
			buildingConf->buildingTimeBySecond = atof(brotherNode->Attribute("buildingTime"));
			buildingConf->technologyPoint = atoi(brotherNode->Attribute("technologyPoint"));
			if (i == 0)
			{
				m_playerBuildingConf[buildingType] = buildingConf;
			}
			else
			{
				m_npcBuildingConf[buildingType] = buildingConf;
			}
			brotherNode = brotherNode->NextSiblingElement();
		}
	}

	return true;
}

bool GameConfig::parseSoundEffectConf(const tinyxml2::XMLElement * node)
{
	if (node == nullptr)
	{
		return false;
	}
	const tinyxml2::XMLElement* bulletNode = node->FirstChildElement("SoundEffect");
	if (bulletNode == nullptr)
	{
		return false;
	}
	const tinyxml2::XMLElement* brotherNode = bulletNode->FirstChildElement();
	while (brotherNode != nullptr)
	{
		std::string type = GameUtils::escapeString(brotherNode->Attribute("type"));
		if (type == "building")
		{
			m_buildingSound.constructName = GameUtils::escapeString(brotherNode->Attribute("constructFileName"));
			m_buildingSound.destroyedName = GameUtils::escapeString(brotherNode->Attribute("destroyFileName"));
		}
		else if (type == "soldier")
		{
			SoldierType soldierType = SoldierType::Invalid;
			SoldierSoundEffectData* soldierEffectData = new (std::nothrow)SoldierSoundEffectData;
			if (soldierEffectData == nullptr)
			{
				continue;
			}
			soldierType = static_cast<SoldierType>(atoi(brotherNode->Attribute("soldierType")));
			soldierEffectData->attackName = GameUtils::escapeString(brotherNode->Attribute("atkFileName"));
			soldierEffectData->moveName = GameUtils::escapeString(brotherNode->Attribute("moveFileName"));
			soldierEffectData->selectName = GameUtils::escapeString(brotherNode->Attribute("selectFileName"));
			soldierEffectData->deathName = GameUtils::escapeString(brotherNode->Attribute("dieFileName"));
			m_soldierSounds[soldierType] = soldierEffectData;
		}
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

SoldierConf* GameConfig::getSoldierConf(ForceType forceType, const SoldierType type)
{
	if (forceType == ForceType::Player)
	{
		auto it = m_playerSoldierConf.find(type);
		if (it != m_playerSoldierConf.end())
		{
			return it->second;
		}
	}
	else if (forceType == ForceType::AI)
	{
		auto it = m_npcSoldierConf.find(type);
		if (it != m_npcSoldierConf.end())
		{
			return it->second;
		}
	}
	return nullptr;
}

BuildingConf* GameConfig::getBuildingConf(ForceType forceType, const BuildingType type)
{
	if (forceType == ForceType::Player)
	{
		auto it = m_playerBuildingConf.find(type);
		if (it != m_playerBuildingConf.end())
		{
			return it->second;
		}
	}
	else if (forceType == ForceType::AI)
	{
		auto it = m_npcBuildingConf.find(type);
		if (it != m_npcBuildingConf.end())
		{
			return it->second;
		}
	}
	
	return nullptr;
}

CooldownConf * GameConfig::getCooldownConf()
{
	return &m_coolDownConf;
}

BuildingSoundEffectData * GameConfig::getBuildingSoundEffectConf()
{
	return &m_buildingSound;
}

SoldierSoundEffectData * GameConfig::getSoldierSoundEffectConf(SoldierType type)
{
	auto it = m_soldierSounds.find(type);
	if (it == m_soldierSounds.end())
	{
		return nullptr;
	}
	return m_soldierSounds[type];
}
