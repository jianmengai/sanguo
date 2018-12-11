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

	//������Ļ��ʾ�ĵ�ͼλ�ã�isDeltaΪtrue��ʾposition����ԭ��λ���ϱ仯��
	void setPosition(cocos2d::Vec2& position, bool isDelta = false);
	cocos2d::Vec2 getPosition();

	//��Ļλ��ת�ɵ�ͼ�ϵ�λ��
	cocos2d::Vec2 toMapPos(const cocos2d::Vec2& pos);

	//����ת����Ƭ�������
	cocos2d::Vec2 toTileRowCol(const cocos2d::Vec2& pos);

	//posΪ��ͼ����λ��
	TileNode* getTileNode(const cocos2d::Vec2& pos);
	TileNode* getTileNode(int row, int col);

	cocos2d::Vec2 tileRowColToPos(const int row, const int col);

	//�����ϰ���
	void setOccupy(const cocos2d::Vec2& pos, const cocos2d::Size& contentSize, OccupyType type);

	void setOccupy(const int row, const int col, OccupyType type);
	//true:����   false:������
	bool checkOccupy(const cocos2d::Vec2& pos, const cocos2d::Size& contentSize, OccupyType type);

	//true:����   false:������
	bool checkOccupy(const int row, const int col, OccupyType type);

	/*��ͼ���ű���*/
	void setMapScale(float scale);
	float getMapScale();

	//gameObject����
	void addChildToGameObjectLayer(cocos2d::Sprite* unit, int zOrder = 1);

	//��ȡ��Ƭ�Ĵ�С
	const cocos2d::Size& getTileSize();
	//��ȡ��ͼ��Ƭ������
	cocos2d::Size& getMapSize();
	//��ȡ��ͼ��С
	cocos2d::Size& getContentSize();

	//NPC��������
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
	cocos2d::Size m_mapSize;       //��Ƭ��������
	cocos2d::Size m_tileSize;
	cocos2d::Size m_clientWinSize;
	cocos2d::experimental::TMXLayer* m_gameObjectLayer;

	cocos2d::DrawNode* m_drawNode = nullptr;

	cocos2d::Vec2 m_tileMapOriginCoor;   //tilemap����ԭ��(0,0)��openGL����ϵ�е�����λ��


	BasePosition m_basePositions;
	cocos2d::Vec2 m_playerInitPosition;
};