#pragma once

#include "NodeInterfaceMCTS.h"
#include "MCTS.h"
#include "TTTnode.h"
#include "UTTTnode.h"

#include <random>
#include <Windows.h>
#include <iostream>
#include <iomanip>
#include <chrono>

#include "CvZnode.h"
#include "SearchAlgorithm.h"

#define debug false
#define DELETE false

class MCTS :
	public SearchAlgorithm
{
	NodeInterfaceMCTS* currentRoot;


	int cnt;
	


	float constantUCT = 0.6;

public:
	int backcounter;
	int nodecounter = 0;

	int backTurnCounter=0;
	int nodeTurnCounter=0;

	int biasConstant = 15;

	MCTS()
	{
		this->cnt = 0;
		this->backcounter = 0;
	}


	MCTS(NodeInterfaceMCTS* _root)
	{
		this->currentRoot = _root;
		this->cnt = 0;
		this->backcounter = 0;

	}

	MCTS(NodeInterfaceMCTS* _root, float uct, int biasC_)
	{
		this->currentRoot = _root;
		this->cnt = 0;
		this->backcounter = 0;


		constantUCT = uct;
		biasConstant = biasC_;
	}


	void updateNode(NodeInterfaceMCTS* new_root)
	{
		this->currentRoot = new_root;
		this->cnt = 0;
		this->backcounter = 0;

	}


	Node* execute(Node* searchNode, int timeMS)
	{

		this->currentRoot = dynamic_cast<NodeInterfaceMCTS*>(searchNode);

		if (this->currentRoot == nullptr)
		{
			std::cout << "Error in dynamic cast";
			exit(-1);
		}
		

		NodeInterfaceMCTS* result = nullptr;
		result = execute(timeMS, 1);

		return result;
		
	}

	void reset()
	{
		this->cnt = 0;
		this->backcounter = 0;

		nodecounter = 0;

		backTurnCounter = 0;
		nodeTurnCounter = 0;

		this->currentRoot = nullptr;
	}



	NodeInterfaceMCTS* execute(int remainingTimeMillis, const int mode)
	{
		//Start timer
		auto startTime = std::chrono::steady_clock::now();
		auto endTime = startTime + std::chrono::milliseconds(remainingTimeMillis);
		auto time = startTime;

		if(currentRoot->getNumSum() == 0)
			currentRoot->setNumSum(1);

		NodeInterfaceMCTS* currentNode = currentRoot;

		int index=0;


		int counter = 0;
		int playcounter = 0;
		int timePlayouts = 0;
		int timeExpanding = 0;
		int timeChilds = 0;

		this->backTurnCounter = 0;
		this->nodeTurnCounter = 0;

		if (currentRoot->leafCondition())
		{
			std::cerr << "\n ***Leaf root***" << std::endl;
			exit;
		}
			


		while (time < endTime)
		{


			time = std::chrono::steady_clock::now();
			//pick random childs until one hasnt been expanded
			int exitCnt = 0;
			while (currentNode->getExpanded() && time < endTime)
			{
				if (currentNode->getChildrenNum() == 0)
				{
					this->backpropagationVS(currentNode);
					currentNode = currentRoot;
				}
				

				/*if (mode == 1)
				{
					index = rand() % currentNode->getChildrenNum();
				}
				else if (mode == 2)
				{
					index = UCTChild(currentNode);
				}
				else if (mode == 3)
				{
					index = optimizedUCT(currentNode);
				}
				else if (mode == 4)
				{
					index = optimizedUCT2(currentNode);
				}*/
				
				index = optimizedUCT2(currentNode);


				if (index >= 82 || index <= -1)
					std::cerr << "ACCESING BAD" << std::endl;


				currentNode = (NodeInterfaceMCTS*)currentNode->getChildren(index);

				exitCnt++;

				if (exitCnt > 200)
				{
					timeChilds += std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - time).count();
					time = std::chrono::steady_clock::now();
				}
			}

			timeChilds += std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - time).count();



			if (currentNode->getExpanded())
				break;

			//If the node hasnt been explored before then do a playout and backpropagate the result
			if (currentNode->getNumSum() < 1)
			{
				time = std::chrono::steady_clock::now();

				this->playout(currentNode);
				playcounter++;
				this->nodeTurnCounter++;
				this->backpropagationVS(currentNode);

				currentNode = currentRoot;

				timePlayouts += std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - time).count();


			}
			else
			{
				time = std::chrono::steady_clock::now();
				//if it has been explored before then expand the node
				currentNode->expand();
				counter++;
				cnt++;

				timeExpanding += std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - time).count();
			}


			//time count
			time = std::chrono::steady_clock::now();

		}




		NodeInterfaceMCTS* bestMove = (NodeInterfaceMCTS*)currentRoot->getChildren(0);
		float bestEval = -10;
		int bestIndex = -1;
		float biasCoheficient = 0;

		//Selecting best move from children of current root
		for (int i = 0; i < currentRoot->getChildrenNum(); i++)
		{
			if (debug)
				std::cerr << "Child " << std::setw(2) << i + 1 << ": " << std::setw(13) << currentRoot->getChildren(i)->getEvaluation() << "   Evals: " << ((NodeInterfaceMCTS*)currentRoot->getChildren(i))->getNumSum() << std::endl; // "    Board: " << (int)((UTTTnode*)currentRoot->getChildren(i))->boards.currentBoard <<  " Move: " << (int)((UTTTnode*)currentRoot->getChildren(i))->currentMove.first << " " << (int)((UTTTnode*)currentRoot->getChildren(i))->currentMove.second << std::endl;

			NodeInterfaceMCTS* child = (NodeInterfaceMCTS*)currentRoot->getChildren(i);



			biasCoheficient = (biasConstant / ((float)(child->getNumSum() + biasConstant + 0.01 * biasConstant * child->getNumSum()))) * 0.75 + 0.25;

			


			float eval = (1-biasCoheficient) * child->getEvaluation() + child->getBias() * biasCoheficient;


			if (eval >= bestEval  && child->getExpanded())
			{
				bestMove = child;
				bestEval = eval;
				bestIndex = i;
			}

		}


		if (DELETE)
		{
			for (int i = 0; i < currentRoot->getChildrenNum(); i++)
			{
				if (i != bestIndex)
				{
					delete currentRoot->getChildren(i);
				}

			}
		}




		if (debug)
		{
			std::cerr << "Child chosen: " << bestIndex+1 << "  Win rate: " << currentRoot->getChildren(bestIndex)->getEvaluation() << "  Current Depth: "  << currentRoot->getChildren(bestIndex)->getDepth() << std::endl;

			auto timeLost = std::chrono::duration_cast<std::chrono::milliseconds>(time - endTime).count();
			std::cerr << "\nNodes: " << counter << " Paths: " << playcounter << "  Time lost:" << timeLost << " Total Nodes:" << this->cnt << " Total Paths: " << this->backcounter << std::endl;
			std::cerr << "Time playouts: " << timePlayouts << "    Time Expanding: " << timeExpanding << "   Time childs: " << timeChilds << std::endl;

		}





		this->currentRoot = bestMove;

		return bestMove;

	}

	void backpropagationScore(NodeInterfaceMCTS* node)
	{
		this->backcounter++;
		this->backTurnCounter++;

		float value = node->getEvaluation();

		while (node!= nullptr && node->getDepth() >= currentRoot->getDepth())
		{
			if (node->getEvaluation() < value)
				node->setEvaluation(value);

			node->setNumSum(node->getNumSum() + 1);

			node = (NodeInterfaceMCTS*)node->getParent();
		}

	}


	void backpropagationVS(NodeInterfaceMCTS* node)
	{
		this->backcounter++;
		this->backTurnCounter++;

		float value = node->getEvaluation();
		float bias = node->getBias();
		int type = 1;


		while ( node!= nullptr && node->getDepth() >= currentRoot->getDepth())
		{
			node->setNumSum(node->getNumSum() + 1);

			node->setSumEval(node->getSumEval() + (value * type));
			node->setEvaluation(node->getSumEval() / node->getNumSum());

			node->setBiasSum(node->getBiasSum() + (bias * type));
			node->setBias(node->getBiasSum() / node->getNumSum());

			

			node = (NodeInterfaceMCTS*)node->getParent();
			type = -type;
		}

	}


	void playout(NodeInterfaceMCTS* node)
	{
		/*
		node->saveState();

		while (!node->leafCondition())
		{
			node->step();

			node->evaluate();
			
			if (node->getEvaluation() < bestEval)
			{
				node->setEvaluation(node->getEvaluation() - 2);
				break;
			}
			
		}

		if (node->getEvaluation() > this->bestEval)
			this->bestEval = node->getEvaluation();

		node->restoreState();
		*/

		node->playout();



	}



	int UCTChild(NodeInterfaceMCTS* node)
	{
		int indx = 0;
		NodeInterfaceMCTS* child;

		float maxUCT = -10;
		float auxUCT = 0;
		float parentConst = log((float)node->getNumSum());

		float biasCoheficient=0;

		/*
		if ( node->getDepth() == 0 && this->currentRoot->mode == 14 && node->getChildrenNum() > 10 && node->getParent() == nullptr)
		{
			node->getChildren(40)->setEvaluation(100000);
			return 40;
		}
		*/


		this->constantUCT = sqrt((float)node->getNumSum())/100.0 + 0.3;

		//this->constantUCT = 0.6;

		/*
		if (node->getNumSum() >= 2000 )
		{
			int i = 0;
			std::cout << "i";
		}
		*/

		float c = 0;

		for (int i = 0; i < node->getChildrenNum(); i++)
		{
			child = (NodeInterfaceMCTS*)node->getChildren(i);
			int childPlays = child->getNumSum() + 1;
			float childEval = child->getEvaluation();




			float a = childEval;
			float b = (this->constantUCT * sqrt(parentConst / ((float)childPlays)));

		
			c = child->getBias();
			biasCoheficient = (biasConstant / ((float)(childPlays + biasConstant + 0.01 * biasConstant * childPlays))) * 0.66 + 0.33;

	

			

			auxUCT = (1 - biasCoheficient) * a + b + c * biasCoheficient;




			//std::cerr << "Child: " << i << "  UCT: " << auxUCT << "  eval: " << a << "  explore: " << b << std::endl;

			if (auxUCT > maxUCT)
			{
				maxUCT = auxUCT;
				indx = i;
			}
				
		}

		return indx;

	}

	int optimizedUCT2(NodeInterfaceMCTS* node)
	{
		int childs = node->getChildrenNum();
		int playouts = node->getNumSum();

		int childToPlay = -1;

		if		(playouts - 1 < childs)
		{
			childToPlay = node->getUCTSavedChild();
			node->setUCTSavedChild(node->getUCTSavedChild()+1);

			//information
			this->nodecounter++;
			
		}
		else 
		{
			if (node->getUCTCounter() == 0)
			{
				childToPlay = UCTChild(node);

				node->setUCTSavedChild(childToPlay);

				int repetitions = (((NodeInterfaceMCTS*)node->getChildren(childToPlay))->getNumSum() / childs) + 1;

				if (repetitions < 125)
					node->setUCTCounter(repetitions);
				else
					node->setUCTCounter(125);
			}
			else
			{
				node->setUCTCounter(node->getUCTCounter() - 1);
				childToPlay = node->getUCTSavedChild();
			}
		}



		return childToPlay;


	}


	int optimizedUCT(NodeInterfaceMCTS* node)
	{
		int childs = node->getChildrenNum();
		int playouts = node->getNumSum();

		int childToPlay = -1;

		if (playouts - 1 < childs)
		{
			childToPlay = node->getUCTSavedChild();
			node->setUCTSavedChild(node->getUCTSavedChild() + 1);
		}
		else if (playouts < childs * 2)
		{
			if (node->getUCTCounter() == 0)
			{
				childToPlay = UCTChild(node);

				node->setUCTSavedChild(childToPlay);
				node->setUCTCounter(1);
			}
			else
			{
				node->setUCTCounter(node->getUCTCounter() - 1);
				childToPlay = node->getUCTSavedChild();
			}
		}
		else if (playouts < childs * 20)
		{
			if (node->getUCTCounter() == 0)
			{
				childToPlay = UCTChild(node);

				node->setUCTSavedChild(childToPlay);
				node->setUCTCounter(2);
			}
			else
			{
				node->setUCTCounter(node->getUCTCounter() - 1);
				childToPlay = node->getUCTSavedChild();
			}
		}
		else if (playouts < childs * 60)
		{
			if (node->getUCTCounter() == 0)
			{
				childToPlay = UCTChild(node);

				node->setUCTSavedChild(childToPlay);
				node->setUCTCounter(3);
			}
			else
			{
				node->setUCTCounter(node->getUCTCounter() - 1);
				childToPlay = node->getUCTSavedChild();
			}
		}
		else if (playouts < childs * 100)
		{
			if (node->getUCTCounter() == 0)
			{
				childToPlay = UCTChild(node);

				node->setUCTSavedChild(childToPlay);
				node->setUCTCounter(4);
			}
			else
			{
				node->setUCTCounter(node->getUCTCounter() - 1);
				childToPlay = node->getUCTSavedChild();
			}
		}
		else if (playouts < childs * 300)
		{
			if (node->getUCTCounter() == 0)
			{
				childToPlay = UCTChild(node);

				node->setUCTSavedChild(childToPlay);
				node->setUCTCounter(7);
			}
			else
			{
				node->setUCTCounter(node->getUCTCounter() - 1);
				childToPlay = node->getUCTSavedChild();
			}
		}
		else if (playouts < childs * 500)
		{
			if (node->getUCTCounter() == 0)
			{
				childToPlay = UCTChild(node);

				node->setUCTSavedChild(childToPlay);
				node->setUCTCounter(10);
			}
			else
			{
				node->setUCTCounter(node->getUCTCounter() - 1);
				childToPlay = node->getUCTSavedChild();
			}
		}
		else if (playouts < childs * 1000)
		{
			if (node->getUCTCounter() == 0)
			{
				childToPlay = UCTChild(node);

				node->setUCTSavedChild(childToPlay);
				node->setUCTCounter(20);
			}
			else
			{
				node->setUCTCounter(node->getUCTCounter() - 1);
				childToPlay = node->getUCTSavedChild();
			}
		}
		else
		{
			if (node->getUCTCounter() == 0)
			{
				childToPlay = UCTChild(node);

				node->setUCTSavedChild(childToPlay);
				node->setUCTCounter(40);
			}
			else
			{
				node->setUCTCounter(node->getUCTCounter() - 1);
				childToPlay = node->getUCTSavedChild();
			}
		}


		if (childToPlay == childs)
			getchar();


		return childToPlay;


	}




	int optimizedUCT3(NodeInterfaceMCTS* node)
	{
		int childs = node->getChildrenNum();
		int playouts = node->getNumSum();

		int coheficient = playouts-1 / childs;

		int childToPlay = -1;

		if (coheficient == 0)
		{
			childToPlay = node->getUCTSavedChild();
			node->setUCTSavedChild(node->getUCTSavedChild() + 1);
		}
		else
		{
			if (node->getUCTCounter() == 0)
			{
				childToPlay = UCTChild(node);

				int repetitions = log(coheficient);

				node->setUCTSavedChild(childToPlay);
				if (repetitions < 250)
					node->setUCTCounter(repetitions);
				else
					node->setUCTCounter(250);
			}
			else
			{
				node->setUCTCounter(node->getUCTCounter() - 1);
				childToPlay = node->getUCTSavedChild();
			}
		}

		return childToPlay;


	}









	void enemyMove(NodeInterfaceMCTS* node)
	{
		if (DELETE)
		{
			for (int i = 0; i < currentRoot->getChildrenNum(); i++)
			{
				if (currentRoot->getChildren(i) != node)
				{
					delete currentRoot->getChildren(i);
				}

			}
		}


		this->currentRoot = node; 

		

	}



};//MCTS