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

	while (true)
	{
		auto nextPathNode = findNextPathNodeBeside(g_currentNode);
		if (!nextPathNode)
		{
			if (!g_openList.empty())
			{
				g_currentNode = startNode;
				continue;
			}
			else
			{
				break;
			}
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
	}

	return pathList;
}

bool AutoFindPath::canVisit(TileNode* node)
{
	bool result = true;

	if (node->occupy == 1 ||
		node->isVisit)
	{
		result = false;
	}

	return result;
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


				//在插入时就实现排序
				if (g_openList.empty())
				{
					g_openList.push_back(prepareToVisitNode);
				}
				else
				{
					bool shouldPushBack = false;
					int elementIndex = 0;
					for (auto& tileNode : g_openList)
					{
						if (tileNode->sumWeight >= prepareToVisitNode->sumWeight)
						{
							break;
						}

						if (elementIndex < (int)g_openList.size() - 1)
						{
							elementIndex++;
						}
						else
						{
							shouldPushBack = true;
						}
					}

					std::list<TileNode*>::iterator minGreaterIter = std::next(g_openList.begin(), elementIndex);
					if (shouldPushBack)
					{
						g_openList.push_back(prepareToVisitNode);
					}
					else
					{
						g_openList.insert(minGreaterIter, prepareToVisitNode);
					}
				}
			}
		}
	}

	if (!g_openList.empty())
	{
		// 按sumWeight值从小到大排序
		//g_openList.sort(isLeftSumWeightLessThanRight);
		nextPathNode = g_openList.front();
		for (auto alternativeNode : g_openList)
		{
			if (nextPathNode->sumWeight > alternativeNode->sumWeight)
			{
				nextPathNode = alternativeNode;
			}
			/*if (alternativeNode->gotoStartNodeWeight > node->gotoStartNodeWeight)
			{
				nextPathNode = alternativeNode;

				break;
			}*/
		}
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
