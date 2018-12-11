#pragma once
#include "cocos2d.h"
#include "GameDefine.h"

enum class TileMapLayerType
{
	Invalid = 0,

	BackgroundLayer,
	GameObjcetLayer,
	ResourceLayer,

	Total
};

struct TileNode
{
	int gid = 0;
	cocos2d::Vec2 position;

	int gotoEndNodeWeight = 0;
	int gotoStartNodeWeight = 0;
	int sumWeight = 0;

	int rowIndex = 0;
	int columnIndex = 0;

	bool isVisit = false;

	OccupyType occupy = OccupyType::Valid;

	TileNode* parent = nullptr;
};



const int OBSTACLE_ID = 2;
const int PASSABLE_ID = 0;

class MapManager
{
public:
	static MapManager* getInstance();
	MapManager();
	~MapManager();
	bool init(cocos2d::Layer* parentLayer, const std::string& mapFileName);

	void update(float dt);

	//设置屏幕显示的地图位置，isDelta为true表示position是在原有位置上变化量
	void setPosition(cocos2d::Vec2& position, bool isDelta = false);
	cocos2d::Vec2 getPosition();

	//屏幕位置转成地图上的位置
	cocos2d::Vec2 toMapPos(const cocos2d::Vec2& pos);

	//坐标转成瓦片块的行列
	cocos2d::Vec2 toTileRowCol(const cocos2d::Vec2& pos);

	//pos为地图坐标位置
	TileNode* getTileNode(const cocos2d::Vec2& pos);
	TileNode* getTileNode(int row, int col);

	cocos2d::Vec2 tileRowColToPos(const int row, const int col);

	//设置障碍物
	void setOccupy(const cocos2d::Vec2& pos, const cocos2d::Size& contentSize, OccupyType type);

	void setOccupy(const int row, const int col, OccupyType type);
	//true:可用   false:不可用
	bool checkOccupy(const cocos2d::Vec2& pos, const cocos2d::Size& contentSize, OccupyType type);

	//true:可用   false:不可用
	bool checkOccupy(const int row, const int col, OccupyType type);

	/*地图缩放比例*/
	void setMapScale(float scale);
	float getMapScale();

	//gameObject层添
	void addChildToGameObjectLayer(cocos2d::Sprite* unit, int zOrder = 1);

	//获取瓦片的大小
	const cocos2d::Size& getTileSize();
	//获取地图瓦片的数量
	cocos2d::Size& getMapSize();
	//获取地图大小
	cocos2d::Size& getContentSize();

	//NPC基地坐标
	BasePosition& getBasePosition();

	cocos2d::Vec2& getPlayerInitPosition();
private:
	bool initBasePosition();

	void initTileNodeTable();

	void drawTileTable();

	void updateTileNodeTable();

	cocos2d::Vec2& getObjectPosition(cocos2d::ValueMap& valueMap, std::string& name);

	void updateOccupy();
private:
	float m_mapScale;
	cocos2d::experimental::TMXTiledMap* m_tiledMap;
	std::vector<std::vector<TileNode*> > m_tileNodeTable;
	cocos2d::Size m_mapContentSize;
	cocos2d::Size m_mapSize;       //瓦片的行列数
	cocos2d::Size m_tileSize;
	cocos2d::Size m_clientWinSize;
	cocos2d::experimental::TMXLayer* m_gameObjectLayer;

	cocos2d::DrawNode* m_drawNode = nullptr;

	cocos2d::Vec2 m_tileMapOriginCoor;   //tilemap格子原点(0,0)在openGL坐标系中的坐标位置


	BasePosition m_basePositions;
	cocos2d::Vec2 m_playerInitPosition;
};