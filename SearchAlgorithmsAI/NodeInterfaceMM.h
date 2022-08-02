#pragma once


#include "Node.h"

class NodeInterfaceMM :
	public Node
{

	//char depthExplored = 0;

	//NodeInterfaceMM* parent = nullptr;
	//std::vector<NodeInterfaceMM*> children;

	//char childrenNum = -1;

	//float evaluation = -100;
	//char depth = -1;
	//


	//bool expanded = false;

	//char mode = -1;
	char depthExplored = 0;

public:
	

	//get rid off later
	int getDepthExplored()
	{
		return (int)this->depthExplored;
	}

	void setDepthExplored(int value)
	{
		this->depthExplored = value;
	}

	virtual int captureCount() { return 0; };
	virtual std::vector<NodeInterfaceMM*> getChildrenCaptures() {
		std::vector<NodeInterfaceMM*> n;
		return n;
	};


	virtual float captureSearch(int depth, float alpha, float beta) = 0;


	//virtual float evaluationFunction() = 0;
	//virtual int expand() =0;
	//virtual bool leafCondition() =0;
	//virtual void step() {};
	//virtual std::string getLastMove() = 0;
	//virtual int checkWin() =0;
	//virtual void print() =0;


	//NodeInterfaceMM()
	//{

	//}

	//void initiate(NodeInterfaceMM* p)
	//{
	//	this->parent = p;
	//	this->depth = p->depth + 1;
	//	this->mode = p->mode;
	//}

	//void initiate(int mode_)
	//{
	//	this->parent = nullptr;
	//	this->depth = 0;
	//	this->mode = mode_;
	//}

	//virtual ~NodeInterfaceMM()
	//{

	//	for (int i = 0; i < this->children.size(); i++)
	//	{
	//		delete this->children.at(i);
	//	}

	//}

	//



	//NodeInterfaceMM* getParent()
	//{
	//	return this->parent;
	//}



	//float getEvaluation()
	//{
	//	return this->evaluation;
	//}

	//void setEvaluation(float e)
	//{
	//	this->evaluation = e;
	//}



	//int getMode()
	//{
	//	return (int)this->mode;
	//}


	//int getChildrenNum()
	//{
	//	return (int)this->childrenNum;
	//}

	//void setChildrenNum(int i)
	//{
	//	this->childrenNum = (unsigned char)i;
	//}

	//void addChild(NodeInterfaceMM* d, int index)
	//{
	//	//cerr << "start here" << endl;
	//	//if (this->children.size() < index + 1)
	//		//this->children.resize(index + 1, nullptr);

	//	this->children[index] = d;
	//}

	//virtual NodeInterfaceMM* getChildren(int index)
	//{
	//	//if (this->children.size() < index + 1)
	//		//children.resize(index + 1, nullptr);


	//	return this->children[index];
	//}

	//virtual NodeInterfaceMM* getChildMove(std::string move) { return nullptr; }


	//void sizeChilds(int size)
	//{
	//	this->children.resize(size, nullptr);
	//}


	//int getExpanded()
	//{
	//	return this->expanded;
	//}

	//void setExpanded()
	//{
	//	expanded = true;
	//}
};

