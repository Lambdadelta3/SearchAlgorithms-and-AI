#pragma once
#include "Node.h"

class SearchAlgorithm
{
public:
	virtual Node* execute(Node* searchNode, int timeMS) = 0;
	virtual void reset() = 0;
};
