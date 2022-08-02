#pragma once

#include <vector>

#include <string>



class NodeInterfaceAStar
{
private:
	NodeInterfaceAStar* parent = nullptr;


	float h = 0;
	float f = 0;
	int g = 0;

	int hash = 0;

public:

	virtual ~NodeInterfaceAStar() {};

	void initiate(NodeInterfaceAStar* p)
	{
		this->setParent(p);
		this->setDepth(this->getParent()->getDepth() + 1);
	}

	void initiate()
	{
		this->setDepth(0);
		this->parent = nullptr;
	}


	virtual std::vector<NodeInterfaceAStar*> expand()
	{
		std::vector<NodeInterfaceAStar*> som;
		return som;
	};
	virtual void evaluate()
	{

	};
	virtual bool equals(NodeInterfaceAStar* b)
	{
		return true;
	};
	virtual bool equalsState(NodeInterfaceAStar* b)
	{
		return true;
	};
	virtual void writeNode() {};


	//Get/set parent node
	NodeInterfaceAStar* getParent()
	{
		return this->parent;
	}

	void setParent(NodeInterfaceAStar* p)
	{
		this->parent = p;
	}




	//Get/set evaluation of node
	float getEvaluation()
	{
		return this->f;
	}
	void setEvaluation(float e)
	{
		this->f = e;
	}


	//Get/set depth of node
	int getDepth()
	{
		return this->g;
	}
	void setDepth(int d)
	{
		this->g = d;
	}

	//Get/set quality of node
	float getQuality()
	{
		return this->h;
	}
	void setQuality(float q)
	{
		this->h = q;
	}


	//Get/set hash
	int getHash()
	{
		return this->hash;
	}
	void setHash(int q)
	{
		this->hash = q;
	}

	virtual void calculateHash() {};
};

