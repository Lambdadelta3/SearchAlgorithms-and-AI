#pragma once
#include "SearchAlgorithm.h"
class RandomSearch :
    public SearchAlgorithm
{

public:


	Node* execute(Node* searchNode, int timeMS)
	{
		

		int childs = searchNode->expand();

		return searchNode->getChildren(rand() % childs);


	}
	void reset() {};

};

