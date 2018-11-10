#include <algorithm> 
#include "MapManager.h"
#include "AutoFindPath.h"

static std::vector<std::vector<TileNode*> > g_tileNodeTable;

static TileNode* g_currentNode = nullptr;
static TileNode* g_endNode = nullptr;
static std::list<TileNode*> g_openList;
static std::list<TileNode*> g_closeList;

static int g_maxRowCount = 10;
static int g_maxColumnCount = 10;



void AutoFindPath::reset()
{
	for (auto tileNodeInOpenList : g_openList)
	{
		tileNodeInOpenList->gotoEndNodeWeight = 0;
		tileNodeInOpenList->gotoStartNodeWeight = 0;
		tileNodeInOpenList->sumWeight = 0;

		tileNodeInOpenList->isVisit = false;
		tileNodeInOpenList->parent = nullptr;
	}

	for (auto tileNodeInCloseList : g_closeList)
	{
		tileNodeInCloseList->gotoEndNodeWeight = 0;
		tileNodeInCloseList->gotoStartNodeWeight = 0;
		tileNodeInCloseList->sumWeight = 0;

		tileNodeInCloseList->isVisit = false;
		tileNodeInCloseList->parent = nullptr;
	}

	g_openList.clear();
	g_closeList.clear();
}

void AutoFindPath::initTileNodeTable(const std::vector<std::vector<TileNode*> >& tileNodeTable)
{
	g_tileNodeTable = tileNodeTable;
	g_maxColumnCount = (int)tileNodeTable.size();
	g_maxRowCount = (int)tileNodeTable[0].size();

	g_currentNode = nullptr;
	g_endNode = nullptr;
	g_openList.clear();
	g_closeList.clear();
}

std::list<TileNode*> AutoFindPath::computeTileNodePathListBetween(TileNode* startNode, TileNode* endNode)
{
	std::list<TileNode*> pathList;

	reset();

	g_currentNode = startNode;
	g_endNode = endNode;

	startNode->isVisit = true;
	g_closeList.push_back(startNode);
	do
	{
		auto nextPathNode = findNextPathNodeBeside(g_currentNode);
		if (nextPathNode == nullptr)
		{
			break;
		}
		g_openList.remove(nextPathNode);
		g_closeList.push_back(nextPathNode);
		g_currentNode = nextPathNode;

		if (nextPathNode == g_endNode)
		{
			pathList = getPathList(startNode, g_endNode);
			pathList.pop_front();
			break;
		}
	} while (!g_openList.empty());

	return pathList;
}

bool AutoFindPath::canVisit(TileNode* node)
{
	/*if (node->occupy == 1)
	{
		return false;
	}*/
	for (auto closeNode : g_closeList)
	{
		if (closeNode == node)
		{
			return false;
		}
	}
	for (auto openNode : g_openList)
	{
		if (openNode == node)
		{
			return false;
		}
	}
	return true;
}

bool isLeftSumWeightLessThanRight(const TileNode* left, const TileNode* right);
TileNode* AutoFindPath::findNextPathNodeBeside(TileNode* node)
{
	TileNode* nextPathNode = nullptr;

	int minRowIndex = std::max(node->rowIndex - 1, 0);
	int maxRowIndex = std::min(node->rowIndex + 1, g_maxRowCount - 1);

	int minColumnIndex = std::max(node->columnIndex - 1, 0);
	int maxColumnIndex = std::min(node->columnIndex + 1, g_maxColumnCount - 1);

	//搜寻周边8个tileNode
	for (int rowIndex = minRowIndex; rowIndex <= maxRowIndex; ++rowIndex)
	{
		for (int columnIndex = minColumnIndex; columnIndex <= maxColumnIndex; ++columnIndex)
		{
			auto prepareToVisitNode = g_tileNodeTable[rowIndex][columnIndex];
			if (canVisit(prepareToVisitNode))
			{
				prepareToVisitNode->isVisit = true;
				prepareToVisitNode->parent = node;
				
				if (prepareToVisitNode->rowIndex != node->rowIndex &&
					prepareToVisitNode->columnIndex != node->columnIndex)
				{
					prepareToVisitNode->gotoStartNodeWeight = node->gotoStartNodeWeight + MOVE_SLOP_WEIGHT;
				}
				else
				{
					prepareToVisitNode->gotoStartNodeWeight = node->gotoStartNodeWeight + MOVE_STRAIGHT_WEIGHT;
				}

				int gotoEndNodeRowDistance = std::abs(g_endNode->rowIndex - prepareToVisitNode->rowIndex);
				int gotoEndNodeColumnDistance = std::abs(g_endNode->columnIndex - prepareToVisitNode->columnIndex);
				prepareToVisitNode->gotoEndNodeWeight = (gotoEndNodeRowDistance + gotoEndNodeColumnDistance) * MOVE_STRAIGHT_WEIGHT;

				prepareToVisitNode->sumWeight = prepareToVisitNode->gotoStartNodeWeight + prepareToVisitNode->gotoEndNodeWeight;

				//g_openList.push_back(prepareToVisitNode);
				//在插入时就实现排序
				
				if (g_openList.empty())
				{
					g_openList.push_back(prepareToVisitNode);
				}
				else
				{
					bool shouldPushBack = true;
					int elementIndex = 0;
					std::list<TileNode*>::iterator preIt = g_openList.end();
					for (std::list<TileNode*>::iterator it = g_openList.begin();
						it != g_openList.end(); ++it)
					{
						TileNode* tileNode = *it;
						if (tileNode->sumWeight >= prepareToVisitNode->sumWeight)
						{
							shouldPushBack = false;
							break;
						}
						preIt = it;
					}
					if (preIt == g_openList.end())
					{
						g_openList.push_front(prepareToVisitNode);
					}
					else if (shouldPushBack)
					{
						g_openList.push_back(prepareToVisitNode);
					}
					else
					{
						++ preIt;
						g_openList.insert(preIt, prepareToVisitNode);
					}
				}
			}
		}
	}

	if (!g_openList.empty())
	{
		/*int sumWeight = INT_MAX;
		for (auto openNode : g_openList)
		{
			if (openNode->sumWeight < sumWeight)
			{
				nextPathNode = openNode;
				sumWeight = openNode->sumWeight;
			}
		}*/
		nextPathNode = g_openList.front();
	}

	return nextPathNode;
}

bool isLeftSumWeightLessThanRight(const TileNode* left, const TileNode* right)
{
	return left->sumWeight < right->sumWeight;
}

std::list<TileNode*> AutoFindPath::getPathList(TileNode* startNode, TileNode* endNode)
{
	std::list<TileNode*> pathList;
	auto previousNode = endNode;
	while (previousNode)
	{
		pathList.push_front(previousNode);
		cocos2d::log("push front:[%d, %d]", previousNode->rowIndex, previousNode->columnIndex);
		previousNode = previousNode->parent;
	}

	return pathList;
}
