#pragma once
#include "NodeInterfaceMM.h"
#include <iostream>
#include <iomanip>
#include <chrono>
#include "UTTTnodeMM.h"
#include "SearchAlgorithm.h"

#define debug false
#define mapdebug false

class Minimax :
	public SearchAlgorithm
	
{
public:
	bool ordering = false;
	int depthOrder = 2;

	int timedepth = 10;

	float cutoffsTotal = 0;
	float cutoffs = 0;

	int lastDepth = 0;
	
	int totalNodes = 0;
	int turnNodes = 0;


	std::chrono::steady_clock::time_point endTime;





	

	Node* execute(Node* searchNode, int timeMS)
	{

		NodeInterfaceMM* node = dynamic_cast<NodeInterfaceMM*>(searchNode);

		if (node == nullptr)
		{
			std::cout << "Error in dynamic cast";
			exit(-1);
		}


		node = IDMinimax(node, true, timeMS);

		return node;
	}


	void reset()
	{

		this->cutoffsTotal = 0;
		this->cutoffs = 0;

		this->lastDepth = 0;

		this->totalNodes = 0;
		this->turnNodes = 0;

	}

	NodeInterfaceMM* IDMinimax(NodeInterfaceMM* node, bool maxPlayer, int time)
	{

		auto startTime = std::chrono::steady_clock::now();
		this->endTime = startTime + std::chrono::milliseconds(time);		

		NodeInterfaceMM* bestChild = nullptr;

		turnNodes = 0;
		float alpha = -9999;
		float beta = 9999;
		float bestEvalFound = 0;

		int depthExplored = node->getDepthExplored();
		//depthExplored--;
		if (depthExplored <= 0)
			depthExplored = 1;



		int playerTYpe;
		if (node->getPlayerType() == 1)
			maxPlayer = true;
		else
			maxPlayer = false;

		for (int i = 1; i < 7; i++)
		{

			//minimax(node, i , alpha, beta, maxPlayer);
			negamax(node, i, alpha, beta);

			//node->orderEvaluation();
			//auto time = std::chrono::steady_clock::now();
			//if (time > this->endTime)
				//break;

		}




		if (debug)
		{
			std::cerr << "*****************Current MM depth: " << (int)node->getDepthExplored() << std::endl;

			std::cerr << "Cutoff Average: " << cutoffsTotal/cutoffs << " moves" << std::endl;
		}

		for (int i = 0; i < node->getChildrenNum(); i++)
		{
			NodeInterfaceMM* child = (NodeInterfaceMM*)node->getChildren(i);

			if (debug)
				std::cerr << "Child " << std::setw(2) << i + 1 << ": " << std::setw(13) << child->getEvaluation() <<  "  Depth: " << (int)child->getDepthExplored() << std::endl;

			
		}

		if (debug)
			std::cerr << "Move chosen : " << std::setw(2) << 0 + 1 << ": " << std::setw(13) << ((NodeInterfaceMM*)node->getChildren(0))->getEvaluation() << " Board:" << (int)((UTTTnodeMM*)node->getChildren(0))->currentBoard << " X:" << (int)((UTTTnodeMM*)node->getChildren(0))->currentMove/3 << " Y:" << (int)((UTTTnodeMM*)node->getChildren(0))->currentMove%3 << std::endl;

		if (mapdebug)
			((NodeInterfaceMM*)node->getChildren(0))->print();

		
		this->totalNodes += this->turnNodes;

		return (NodeInterfaceMM*)node->getChildren(0);

	}





	float negamax(NodeInterfaceMM* node, int depth, float alpha, float beta)
	{
		if (depth == 0 || node->leafCondition())
		{
			if (node->getEvaluation() == -100)
			{
				if(node->getMode() == 0)
					node->setEvaluation(node->evaluationFunction());
				
				if(node->getMode() == 1)
					node->setEvaluation( node->captureSearch(100 , alpha, beta) * node->getPlayerType());

				if(node->getMode() == 2)
					node->setEvaluation(captureSearch(node, 100, alpha, beta) * node->getPlayerType());

				this->turnNodes++;
			}
			return node->getEvaluation() * node->getPlayerType();
		}

		int numChildren = -1;
		if (node->getChildrenNum() <= 0)
			numChildren = node->expand();
		else
			numChildren = node->getChildrenNum();


		float value = -9999;

		for (int i = 0; i < numChildren; i++)
		{
			cutoffsTotal++;
			NodeInterfaceMM* child = nullptr;

			child = (NodeInterfaceMM*)node->getChildren(i);

			value = (std::max)(value, -negamax(child, depth - 1, -beta, -alpha));


			if (depth >= timedepth)
			{
				auto time = std::chrono::steady_clock::now();
				if (time > this->endTime)
				{
					//std::cout << "time ran out";
					node->orderEvaluation(i);
					return value;
				}
			}

			alpha = (std::max)(alpha, value);
			if (alpha >= beta)
			{
				numChildren = i;
				break;
			}

		}

		cutoffs++;
		node->setEvaluation(value * node->getPlayerType());
		node->setDepthExplored(depth);
		node->orderEvaluation(numChildren);
		return value;

	}

	float captureSearch(NodeInterfaceMM* node, int depth, float alpha, float beta)
	{
		if (node->leafCondition() || node->captureCount() == 0 || depth == 0)
		{
			return node->evaluationFunction() * node->getPlayerType();
		}

		if (node->getChildrenNum() <= 0)
			node->expand();

		std::vector<NodeInterfaceMM*> children = node->getChildrenCaptures();

		//maybe some ordering here??

		float value = -9999;
		for (NodeInterfaceMM* child:children)
		{

			value = (std::max)(value, -captureSearch(child, depth - 1, -beta, -alpha));

			alpha = (std::max)(alpha, value);
			if (alpha >= beta)
			{
				break;
			}

		}


		return value;


	}


	float minimax(NodeInterfaceMM* node, int depth, float alpha, float beta, bool maxPlayer)
	{
		if (depth == 0 || node->leafCondition())
		{
			if (node->getEvaluation() == -100)
			{
				node->setEvaluation(node->evaluationFunction());
				this->turnNodes++;
			}
			return node->getEvaluation();
		}




		int numChildren = -1;
		if (node->getChildrenNum() <= 0)
			numChildren = node->expand();
		else
			numChildren = node->getChildrenNum();






		float value;

		if (maxPlayer)
		{
			value = -9999;
			
			for (int i = 0; i < numChildren; i++)
			{

				NodeInterfaceMM* child = nullptr;

				if (ordering && depth >= depthOrder)// && sizeAux > i)
				{
					float best = -999;
					int bestInd= -1;

					child = (NodeInterfaceMM*)node->getChildren(i);
					
					if(child == nullptr)
						child = (NodeInterfaceMM*)node->getChildren(i);
					else
					{
						for (int j = i; j < numChildren; j++)
						{
							child = (NodeInterfaceMM*)node->getChildren(j);
							if (child == nullptr)
								break;

							if (child->getEvaluation() > best)
							{
								best = child->getEvaluation();
								bestInd = j;
							}
						}

						child = (NodeInterfaceMM*)node->getChildren(bestInd);
						node->addChild(node->getChildren(i), bestInd);
						node->addChild(child, i);

					}
				}
				else
					child = (NodeInterfaceMM*)node->getChildren(i);


				value = (std::max)(value, minimax(child, depth - 1, alpha, beta, !maxPlayer));

				cutoffsTotal++;

				if (depth >= timedepth)
				{
					auto time = std::chrono::steady_clock::now();
					if (time > this->endTime)
					{
						//std::cout << "time ran out";
						node->orderEvaluation(i);
						return value;
					}
				}

				alpha = (std::max)(alpha, value);
				if (alpha >= beta)
				{
					numChildren = i;
					break;
				}
					
			}

		}
		else//min player
		{
			value = 9999;

			for (int i = 0; i < numChildren; i++)
			{
				NodeInterfaceMM* child = nullptr;

				if (ordering && depth >= depthOrder)
				{
					float best = 999;
					int bestInd = -1;

					child = (NodeInterfaceMM*)node->getChildren(i);

					if (child == nullptr)
						child = (NodeInterfaceMM*)node->getChildren(i);
					else
					{
						for (int j = i; j < numChildren; j++)
						{
							child = (NodeInterfaceMM*)node->getChildren(j);
							if (child == nullptr)
								break;

							if (child->getEvaluation() < best)
							{
								best = child->getEvaluation();
								bestInd = j;
							}
						}

						child = (NodeInterfaceMM*)node->getChildren(bestInd);
						node->addChild(node->getChildren(i), bestInd);
						node->addChild(child, i);

					}


				}
				else
					child = (NodeInterfaceMM*)node->getChildren(i);



				value = (std::min)(value, minimax(child, depth - 1, alpha, beta, !maxPlayer));

				cutoffsTotal++;

				if (depth >= timedepth)
				{
					auto time = std::chrono::steady_clock::now();
					if (time > this->endTime)
					{
						//std::cout << "time ran out";
						node->orderEvaluation(i);
						return value;
					}
				}


				beta = (std::min)(value, beta);
				if (beta <= alpha)
				{
					numChildren = i;
					break;
				}
			}

		}

		cutoffs++;
		node->setEvaluation(value);
		node->setDepthExplored(depth);
		node->orderEvaluation(numChildren);
		return value; 

			

	}











};

