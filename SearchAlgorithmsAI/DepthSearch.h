#pragma once
#include "NodeInterfaceMCTS.h"
#include <random>
#include <Windows.h>
#include <iostream>
#include <chrono>

#define debug true

class DepthSearch
{
	NodeInterfaceMCTS* currentRoot;
	int cnt;
	int backcounter;

public:
	DepthSearch(NodeInterfaceMCTS* _root)
	{
		this->currentRoot = _root;
		this->cnt = 0;
		this->backcounter = 0;
	}


	NodeInterfaceMCTS* execute(int remainingTimeMillis)
	{
		//Start timer
		auto startTime = std::chrono::steady_clock::now();
		auto endTime = startTime + std::chrono::milliseconds(remainingTimeMillis);
		auto time = startTime;

		NodeInterfaceMCTS* currentNode = currentRoot;

		int randIndex;
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_real_distribution<> dis(0.0, 1.0);

		int counter = 0;



		//std::cerr << "Time: " << time << " endtime: " << endTime <<" RemainingTime: " << remainingTimeMillis << "\n";

		//std::cerr << "expanded: " << currentNode->getExpanded() << "\n";

		while (time < endTime)
		{
			//pick random childs until one hasnt been expanded
			while (currentNode->getExpanded() && time < endTime)
			{
				if (currentNode->getChildrenNum() == 0)
					currentNode = currentRoot;

				randIndex = (int)(dis(gen) * currentNode->getChildrenNum());

				if (randIndex >= 16 || randIndex <= -1)
					std::cerr << "ACCESING BAD" << std::endl;

				//std::cerr << "Accessing child: " << randIndex <<"  childrennum:" << currentNode->getChildrenNum() << "\n";
				currentNode = (NodeInterfaceMCTS*)currentNode->getChildren(randIndex);

				time = std::chrono::steady_clock::now();
			}

			if (time >= endTime)
				break;

			//expand the node found
			currentNode->expand();

			//if leaf node then backpropagate result
			if (currentNode->getChildrenNum() == 0)
				this->backpropagation(currentNode);


			//time shit
			time = std::chrono::steady_clock::now();

			counter++;
			cnt++;
			//std::cerr << "loop end\n";
		}




		NodeInterfaceMCTS* bestMove = currentRoot;
		int bestIndex = -1;

		//Selecting best move from children of current root
		for (int i = 0; i < currentRoot->getChildrenNum(); i++)
		{
			if (debug)
				;//std::cerr << "Child " << i << ": " << currentRoot->getChildren(i)->getQuality() << std::endl;

			if (bestMove->getEvaluation() <= currentRoot->getChildren(i)->getEvaluation() && currentRoot->getChildren(i)->getExpanded())
			{
				bestMove = (NodeInterfaceMCTS*)currentRoot->getChildren(i);
				bestIndex = i;
			}

		}

		if (debug)
		{
			auto timeLost = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - time).count();
			std::cerr << "\nCounter MCTS: " << counter << "  Time lost:" << timeLost << " Total Nodes:" << this->cnt << " Total Paths: " << this->backcounter << std::endl;
		}

		this->backcounter = 0;



		this->currentRoot = bestMove;

		return bestMove;

	}

	void backpropagation(NodeInterfaceMCTS* node)
	{
		this->backcounter++;

		node->evaluationFunction();
		while (node->getDepth() > currentRoot->getDepth())
		{
			if (node->getEvaluation() > node->getParent()->getEvaluation())
				node->getParent()->setEvaluation(node->getEvaluation());
			else
			{

				break;
			}

			node = (NodeInterfaceMCTS*)node->getParent();
		}

		node = currentRoot;
	}


};