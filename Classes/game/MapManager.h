#pragma once
#include "cocos2d.h"

enum class TileMapLayerType
{
	Invalid = 0,

	BackgroundLayer,
	GameObjcetLayer,

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

	int occupy = 0; // 是否占用

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
	cocos2d::Vec2& toMapPos(const cocos2d::Vec2& pos);

	//坐标转成瓦片块的行列
	cocos2d::Vec2 toTileRowCol(const cocos2d::Vec2& pos);

	cocos2d::Vec2 tileRowColToPos(const int row, const int col);

	//设置障碍物
	void setOccupy(const cocos2d::Vec2& pos, const cocos2d::Size& contentSize);

	//true:可用   false:不可用
	bool checkOccupy(const cocos2d::Vec2& pos, const cocos2d::Size& contentSize);

	//true:可用   false:不可用
	bool checkOccupy(const int row, const int col);

	/*地图缩放比例*/
	void setMapScale(float scale);
	float getMapScale();

	//gameObject层添
	void addChildToGameObjectLayer(cocos2d::Sprite* unit, int zOrder = 1);

	cocos2d::Size getTileSize();
	cocos2d::Size getMapSize();

private:
	void initTileNodeTable();

	void drawTileTable();
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
};