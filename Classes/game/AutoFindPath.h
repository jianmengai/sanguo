#pragma once
#include <vector>
#include <list>

const int MOVE_SLOP_WEIGHT = 14;
const int MOVE_STRAIGHT_WEIGHT = 10;

struct TileNode;

class AutoFindPath
{
public:
	static void initTileNodeTable(const std::vector<std::vector<TileNode*> >& tileNodeTable);
	static std::list<TileNode*> computeTileNodePathListBetween(TileNode* startNode, TileNode* endNode);

private:
	static void reset();

	static bool canVisit(TileNode* node);
	static TileNode* findNextPathNodeBeside(TileNode* node);
	static std::list<TileNode*> getPathList(TileNode* startNode, TileNode* endNode);

	AutoFindPath() {}
	~AutoFindPath() {}
};
