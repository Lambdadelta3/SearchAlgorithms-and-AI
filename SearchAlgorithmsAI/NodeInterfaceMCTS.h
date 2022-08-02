#pragma once

#include <vector>
#include <iostream>
#include "Node.h"




class NodeInterfaceMCTS :
	public Node
{
private:

	//NodeInterfaceMCTS* parent = nullptr;
	//std::vector<NodeInterfaceMCTS*> children;

	//bool leaf = false;

	//int depth = -1;
	//float evaluation = -100;

	//char childrenNum = -1;
	//bool expanded = false;

	//char mode = -1;
	//char playerType = 0;



	float sumEval = 0;
	int numSum = 0;

	char uctSavedChild = 0;
	char uctCounter = 0;

	float bias = 0;
	float biasSum = 0;

public:


	
	void initiate(NodeInterfaceMCTS* p)
	{
		this->Node::initiate(p);
		this->setEvaluation(0);
		this->setNumSum(0);
		this->setSumEval(0);
		/*
		this->setParent(p);
		this->depth = p->depth + 1;
		this->playerType = -p->playerType;
		this->mode = p->mode;
		this->expanded = false;*/

	}

	void initiate(int mode_)
	{
		this->Node::initiate(mode_);
		this->setEvaluation(0);
		this->setNumSum(0);
		this->setSumEval(0);

		//this->setParent(nullptr);
		//this->playerType = 1;
		//this->depth = 0;
		//

		//this->expanded = false;
		//this->mode = mode_;

	}

	
	void reset()
	{
		this->Node::reset();

		sumEval = 0;
		numSum = 0;
		uctSavedChild = 0;
		uctCounter = 0;

		bias = 0;
		biasSum = 0;
	}


	//get or set the UCT child chosen as best from previous times
	int getUCTSavedChild()
	{
		return this->uctSavedChild;
	}

	void setUCTSavedChild(int i)
	{
		this->uctSavedChild = i;
	}


	//get/set sums of all evaluations
	float getSumEval()
	{
		return this->sumEval;
	}

	void setSumEval(float e)
	{
		this->sumEval = e;
	}

	//get/set number of sums
	int getNumSum()
	{
		return this->numSum;
	}

	void setNumSum(int d)
	{
		this->numSum = d;
	}


	//get/set uctCounter
	int getUCTCounter()
	{
		return this->uctCounter;
	}

	void setUCTCounter(int c)
	{
		this->uctCounter = c;
	}

	//get/set bias
	float getBias()
	{
		return this->bias;
	}

	void setBias(float b)
	{
		this->bias = b;
	}

	//get/set biasSum
	float getBiasSum()
	{
		return this->biasSum;
	}

	void setBiasSum(float bs)
	{
		this->biasSum = bs;
	}

	virtual void playout() = 0;

	//virtual NodeInterfaceMCTS* expand() { return nullptr; };
	//virtual void evaluate() {};
	

	//virtual void step() {};

	//virtual void print() {};
	//virtual std::string getLastMove() { return ""; }
	//virtual int checkWin() { return 0; };


	//

	//int getPlayerType()
	//{
	//	return (int)this->playerType;
	//}

	//void setPlayerType(int p)
	//{
	//	this->playerType = p;
	//}

	//int getMoveType()
	//{
	//	return -(int)this->playerType;
	//}

	//void setMoveType(int m)
	//{
	//	this->playerType = -m;
	//}

	//void sizeChilds(int size)
	//{
	//	this->children.resize(size, nullptr);
	//}


	////this might be problematic when deleting if we implement some optimizaions, careful!!!
	//virtual ~NodeInterfaceMCTS()
	//{
	//	

	//	for (int i = 0; i < this->children.size(); i++)
	//	{
	//		delete this->children.at(i);
	//	}

	//}


	//bool leafCondition()
	//{
	//	return this->leaf;
	//}

	//void setleaf()
	//{
	//	this->leaf = true;
	//}

	//void setleaf(bool v)
	//{
	//	this->leaf = v;
	//}

	////Get/set parent node
	//NodeInterfaceMCTS* getParent()
	//{
	//	return this->parent;
	//}

	//void setParent(NodeInterfaceMCTS* p)
	//{
	//	this->parent = p;
	//}


	//int getMode()
	//{
	//	return (int)this->mode;
	//}


	////Get/set child nodes
	//void addChild(NodeInterfaceMCTS* d, int index)
	//{
	//	//cerr << "start here" << endl;
	//	//if (this->children.size() < index + 1)
	//		//this->children.resize(index + 1, nullptr);

	//	this->children.at(index) = d;
	//}

	//virtual NodeInterfaceMCTS* getChildren(int index)
	//{
	//	//if (this->children.size() < index+1)
	//	//	children.resize(index + 1, nullptr);


	//	return this->children[index];
	//}

	//virtual NodeInterfaceMCTS* getChildMove(std::string move) { return nullptr; }


	//int getChildrenNum()
	//{
	//	return (int)this->childrenNum;
	//}

	//void setChildrenNum(int i)
	//{
	//	this->childrenNum = (unsigned char)i;
	//}




	////Get/set evaluation of node
	//float getEvaluation()
	//{
	//	return this->evaluation;
	//}

	//void setEvaluation(float e)
	//{
	//	this->evaluation = e;
	//}





	////Get/set depth of node
	//int getDepth()
	//{
	//	return this->depth;
	//}

	//void setDepth(int d)
	//{
	//	this->depth = d;
	//}

	////



	//void setExpanded()
	//{
	//	expanded = true;
	//}

	//int getExpanded()
	//{
	//	return this->expanded;
	//}



	//void reserve(int n)
	//{
	//	this->children.reserve(n);
	//}



	




};//NodeInterfaceMCTS

