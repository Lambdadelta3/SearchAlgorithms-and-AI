#pragma once

#include <string>
#include <vector>

class Node
{

	Node* parent = nullptr;
	std::vector<Node*> children;

	bool leaf = false;

	char depth = -1;
	float evaluation = -100;
	char playerType = 1;

	char childrenNum = -1;
	bool expanded = false;

	char mode = -1;

	

public:

	virtual float evaluationFunction() = 0;
	virtual int expand() = 0;
	virtual void step() = 0;
	virtual std::string getLastMove() = 0;
	virtual int checkWin() = 0;
	virtual void print() = 0;


	

	virtual void step(int a, int b) {};
	virtual void stepBack() {};



	//Initiation/deletion of the node

	void initiate(Node* p)
	{
		this->parent = p;
		this->depth = p->depth + 1;
		this->mode = p->mode;
		this->playerType = -p->playerType;
	}

	void initiate(int mode_)
	{
		this->parent = nullptr;
		this->depth = 0;
		this->mode = mode_;
		this->playerType = 1;
	}

	virtual ~Node()
	{

		for (int i = 0; i < this->children.size(); i++)
		{
			delete this->children.at(i);
		}

	}


	bool leafCondition()
	{
		return this->leaf;
	}

	void setleaf()
	{
		this->leaf = true;
	}

	void setleaf(bool v)
	{
		this->leaf = v;
	}

	//Deletes all children and sets the state back to standard?
	virtual void reset()
	{

		for (int i = 0; i < this->children.size(); i++)
		{
			delete this->children.at(i);
		}

		this->parent = nullptr;
		this->depth = 0;
		this->children.clear();

		this->evaluation = -100;
		this->playerType = 1;

		this->childrenNum = -1;
		this->expanded = false;
		this->leaf = false;
	}



	//Info

	float getEvaluation()
	{
		return this->evaluation;
	}

	void setEvaluation(float e)
	{
		this->evaluation = e;
	}

	int getDepth()
	{
		return this->depth;
	}

	int setDepth(int d)
	{
		this->depth = d;
	}

	int getMode()
	{
		return (int)this->mode;
	}
	//Returns the type of the player that is this node
	int getPlayerType()
	{
		return (int)this->playerType;
	}

	void setPlayerType(int p)
	{
		this->playerType = p;
	}
	//Returns the move type of the last move made in this board
	int getMoveType()
	{
		return -(int)this->playerType;
	}

	void setMoveType(int m)
	{
		this->playerType = -m;
	}

	//Has the node been expanded?
	int getExpanded()
	{
		return this->expanded;
	}

	void setExpanded()
	{
		expanded = true;
	}


	//Get parent node
	Node* getParent()
	{
		return this->parent;
	}



	//Children Management
	int getChildrenNum()
	{
		return (int)this->childrenNum;
	}

	void setChildrenNum(int i)
	{
		this->childrenNum = (unsigned char)i;
	}

	void addChild(Node* d, int index)
	{
		this->children[index] = d;
	}

	virtual Node* getChildren(int index)
	{
		return this->children[index];
	}

	virtual Node* getChildMove(std::string move) { return nullptr; }


	void sizeChilds(int size)
	{
		this->children.resize(size, nullptr);
	}

	

	void orderEvaluation(int numchilds)
	{
		if(this->playerType == 1)
			std::sort(this->children.begin(), this->children.begin() + numchilds, [](Node* a, Node* b) {return a->getEvaluation() > b->getEvaluation(); });
		else
			std::sort(this->children.begin(), this->children.begin() + numchilds, [](Node* a, Node* b) {return a->getEvaluation() < b->getEvaluation(); });
	}

	

	virtual bool operator==(const Node* n) const
	{
		std::cout << "ERROOOOORRRRRRRRRRRR";
		return true;

	}


	const virtual int hashValue() const
	{
		std::cout << "ERROOOOORRRRRRRRRRRR";
		return 0;
	}

};

