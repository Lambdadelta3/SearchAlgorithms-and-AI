#pragma once
#include <string>
#include "Node.h"
#include "SearchAlgorithm.h"

class Fighter
{
	Node* node = nullptr;
	SearchAlgorithm* search = nullptr;

	int timeMS = 0;

public:



	Fighter(int t, int m)
	{
		this->timeMS = t;
	}

	Fighter(Node* _node, SearchAlgorithm* _sa, int _timeMS)
	{
		this->node = _node;
		this->search = _sa;
		this->timeMS = _timeMS;
	}


	virtual void initiate()
	{

	}

	virtual void prepareToFight(int order)
	{

		while (this->node->getParent() != nullptr)
			this->node = this->node->getParent();



		this->node->reset();
		this->search->reset();



	}



	std::string nextMove(std::string enemyMove)
	{

		if (node->getChildrenNum() <= 0)
		{
			//If second move
			if (enemyMove.compare("") != 0)
			{
				node->expand();							// get the childs of the first move
				node = node->getChildMove(enemyMove);	// choose the move picked by opponent
			}
		}
		//If after 2nd move
		else
			node = node->getChildMove(enemyMove);


		node = search->execute(node, timeMS);


		if (node->leafCondition())
			return "GAME OVER";


		return node->getLastMove();

	}


	void print() 
    {
        node->print();
    }


	int checkWinner(int order)
	{
		if (order == 1)
			return node->checkWin();
		else if (order == 2)
			return -node->checkWin();

	}


	void info() 
	{
	
	}


	void terminate()
	{

	}

	

};

