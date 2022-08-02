#pragma once
#include "NodeInterfaceMCTS.h"
#include "NodeInterfaceAStar.h"
#include "CvZstate.h"
#include <iostream>
#include <cmath>
#include <random>

#define MC 16
#define PI 3.14159265
#define ANGLE  (360/MC)*(PI/180)


//
//class CvZnode :
//	public NodeInterfaceMCTS
//{
//public:
//
//	CvZstate* state;
//	CvZstate* backup;
//
//	int optimalEval;
//
//	CvZnode()
//	{
//		this->initiate(0);
//		this->state = nullptr;
//		this->reserve(MC);
//	}
//
//	CvZnode(Humans* h, Zombies* z, int ashX, int ashY, int sc)
//	{
//		this->initiate(0);
//		this->reserve(MC);
//		this->state = new CvZstate(h, z, ashX, ashY, sc);
//	}
//
//	CvZnode(CvZnode* p)
//	{
//		this->initiate(p);
//		this->reserve(MC);
//		this->state = p->state->clone();
//	}
//
//	~CvZnode()
//	{
//		delete this->state;
//		delete this->backup;
//	}
//
//
//
//
//
//	void zombiesMove()
//	{
//		//zombies move
//		short movex = 0, movey = 0, closestx = 0, closesty = 0;
//		float distance = 100000.0, fractionMov = 0.0;
//		for (int i = 0; i < this->state->zombies->total; i++)
//		{
//			if (this->state->zombies->alive[i])
//			{
//
//				//if zombie's target is dead then target = none
//				if (this->state->zombies->targetID[i] >= 0 && !this->state->humans->alive[this->state->zombies->targetID[i]])
//					this->state->zombies->targetID[i] = -2;
//
//				//if target == not human or distance to ash is less than distance to target then target is Ash
//				if (this->state->zombies->targetID[i] < 0 || this->state->zombies->distToTarget[i] > this->state->zombies->distanceTo(this->state->ashX, this->state->ashY, i))
//				{
//					this->state->zombies->distToTarget[i] = this->state->zombies->distanceTo(this->state->ashX, this->state->ashY, i);
//					this->state->zombies->targetID[i] = -1;
//				}
//
//				//if target == not human then check all humans?????
//
//				if (this->state->zombies->targetID[i] < 0)
//				{
//					for (int j = 0; j < this->state->humans->total; j++)
//					{
//						if (this->state->humans->alive[j])
//						{
//							distance = this->state->zombies->distanceTo(this->state->humans->x[j], this->state->humans->y[j], i);
//
//							if (distance < this->state->zombies->distToTarget[i])
//							{
//								this->state->zombies->distToTarget[i] = distance;
//								this->state->zombies->targetID[i] = j;
//							}
//						}
//					}
//				}
//
//
//				if (this->state->zombies->targetID[i] < 0)
//				{
//					closestx = this->state->ashX;
//					closesty = this->state->ashY;
//				}
//
//				if (this->state->zombies->targetID[i] >= 0)
//				{
//					closestx = this->state->humans->x[this->state->zombies->targetID[i]];
//					closesty = this->state->humans->y[this->state->zombies->targetID[i]];
//				}
//
//
//				fractionMov = 400 / this->state->zombies->distToTarget[i];
//				if (fractionMov > 1)
//					fractionMov = 1;
//
//				movex = floor(fractionMov * (closestx - this->state->zombies->x[i]));
//				movey = floor(fractionMov * (closesty - this->state->zombies->y[i]));
//
//				this->state->zombies->x[i] += movex;
//				this->state->zombies->y[i] += movey;
//
//				//actualizing distace to target
//				this->state->zombies->distToTarget[i] = this->state->zombies->distanceTo(this->state->humans->x[this->state->zombies->targetID[i]], this->state->humans->y[this->state->zombies->targetID[i]], i);
//
//			}
//		}
//	}
//
//
//
//	bool ashRandomMove()
//	{
//
//		int randIndex;
//
//		int moveX;
//		int moveY;
//
//
//		while (true)
//		{
//			randIndex = rand() % (int)MC;
//
//			int moveY = floor(sin(ANGLE * randIndex) * 1000);
//			int moveX = floor(cos(ANGLE * randIndex) * 1000);
//
//			if (this->state->ashX + moveX <= 16000 && this->state->ashX + moveX >= 0 && this->state->ashY + moveY <= 9000 && this->state->ashY + moveY >= 0)
//			{
//				this->state->ashX += moveX;
//				this->state->ashY += moveY;
//				return true;
//			}
//		}
//
//	}
//
//
//
//
//	void killing()
//	{
//
//
//		int fib = 1;
//		int fib0 = 1;
//
//		int fibOP = 1;
//		int fibOP0 = 1;
//
//		int aux = this->state->humans->numAlive;
//
//		for (int i = 0; i < this->state->zombies->total; i++)
//		{
//			if (this->state->zombies->alive[i])
//			{
//				//Ash kills zombies
//				if (this->state->zombies->distanceTo(this->state->ashX, this->state->ashY, i) <= 2000)
//				{
//					this->state->zombies->alive[i] = false;
//					this->state->zombies->numAlive--;
//
//					//score update
//					this->state->score += pow(this->state->humans->numAlive, 2) * 10 * fib;
//					fib = fib + fib0;
//					fib0 = fib - fib0;
//				}
//				else//optimal score
//				{
//					this->optimalEval += pow(this->state->humans->numAlive, 2) * 10 * fibOP;
//					fibOP = fibOP + fibOP0;
//					fibOP0 = fibOP - fibOP0;
//				}
//
//				
//				
//
//				//Zombies kill humans
//				if (this->state->zombies->alive[i] && this->state->zombies->targetID[i] >= 0)
//				{
//					if (this->state->zombies->distToTarget[i] == 0 && this->state->humans->alive[this->state->zombies->targetID[i]])
//					{
//						this->state->humans->alive[this->state->zombies->targetID[i]] = false;
//						aux--;
//					}
//				}
//			}
//		}
//
//
//		this->state->humans->numAlive = aux;
//
//	}
//
//
//	CvZnode* clone()
//	{
//		CvZnode* cloned = new CvZnode();
//		cloned->state = this->state->clone();
//
//		cloned->setParent(this->getParent());
//		cloned->setDepth(this->getDepth());
//
//		return cloned;
//	}
//
//
//
//
//
//
//
//
//
//
//
//
//	// AStar Functions
//
//	/*
//
//	std::vector<NodeInterfaceAStar*> expand()
//	{
//
//		std::vector<NodeInterfaceAStar*> children;
//
//		//leaf node condition
//		if (this->state->zombies->numAlive == 0 || this->state->humans->numAlive == 0)
//		{
//			return children;
//		}
//
//
//		zombiesMove();
//
//
//		//Ash moves
//		float distance = 1000;
//		CvZnode* nodeNPclone;
//		for (float i = 0; i < 360; i += 360.0 / 16.0)
//		{
//
//			int ashMovey = floor(sin(i * PI / 180) * distance);
//			int ashMovex = floor(cos(i * PI / 180) * distance);
//
//			if (this->state->ashX + ashMovex <= 16000 && this->state->ashX + ashMovex >= 0 && this->state->ashY + ashMovey <= 9000 && this->state->ashY + ashMovey >= 0)
//			{
//				nodeNPclone = new CvZnode(this);
//				nodeNPclone->state->ashX = this->state->ashX + ashMovex;
//				nodeNPclone->state->ashY = this->state->ashY + ashMovey;
//
//				//Ash/zombies kill
//				nodeNPclone->killing();
//
//				children.push_back(nodeNPclone);
//			}
//
//		}
//
//
//
//		return children;
//	}
//
//
//
//	void evaluate()
//	{
//
//		int eval;
//
//
//		eval = -this->state->score;
//
//
//		int savableHumans = 0;
//		float dist = 0, distAsh = 0;
//		std::vector<int> humansChecked;
//
//		for (int i = 0; i < this->state->zombies->total; i++)
//		{
//			if (this->state->zombies->alive[i] == true)
//			{
//
//				if (this->state->zombies->targetID[i] >= 0 && this->state->humans->savable[this->state->zombies->targetID[i]])
//				{
//					dist = this->state->zombies->distToTarget[i];
//					distAsh = this->state->humans->distanceTo(this->state->ashX, this->state->ashY, this->state->zombies->targetID[i]);
//
//					if (dist < (distAsh * 4 / 10))
//					{
//						this->state->humans->savable[this->state->zombies->targetID[i]] = false;
//					}
//					else
//						savableHumans++;
//
//
//
//				}
//
//
//
//			}
//		}
//
//
//		if (savableHumans == 0)
//			eval = 0;
//
//
//		this->setEvaluation(eval);
//		this->setQuality(this->getDepth() + this->getEvaluation());
//
//	}
//
//
//	bool equalsState(NodeInterfaceAStar* b)
//	{
//		if(this->state->zombies->numAlive == 0 || this->state->humans->numAlive == 0)
//			return true;
//
//		return false;
//	}
//
//
//	void calculateHash() 
//	{
//		int hash = this->state->ashX * 761459 + this->state->ashY * 337313;
//
//
//		this->setHash(hash);
//	
//	}
//	*/
//
//
//
//
//
//
//
//	// MCTS Functions
//
//
//	
//	void playout()
//	{
//
//		//Store the state before the playout
//		CvZstate* aux = this->state->clone();
//
//		int counter = 0;
//
//		while (!leafCondition())
//		{
//			this->zombiesMove();
//
//			this->ashRandomMove();
//
//			this->killing();
//
//			counter++;
//		}
//
//
//		this->setDepth(this->getDepth() + counter);
//
//		this->evaluate();
//
//		this->setDepth(this->getDepth() - counter);
//
//		//Return the state back
//		this->state = aux;
//
//	}
//
//
//
//	bool leafCondition()
//	{
//		if (this->state->humans->numAlive > 0 && this->state->zombies->numAlive > 0)
//			return false;
//		return true;
//	}
//
//	void step()
//	{
//		this->zombiesMove();
//
//		this->ashRandomMove();
//
//		this->killing();
//	}
//
//	void saveState()
//	{
//		this->backup = this->state->clone();
//	}
//
//	void restoreState()
//	{
//		this->state = this->backup; 
//	}
//
//
//	NodeInterfaceMCTS* expand()
//	{
//
//
//		if (this->getExpanded())
//			std::cerr << "expanding more tha once warning\n";
//
//		this->setExpanded();
//
//		//leaf node condition
//		if (this->state->zombies->numAlive == 0 || this->state->humans->numAlive == 0)
//		{
//			this->setChildrenNum(0);
//			return nullptr;
//		}
//
//
//		zombiesMove();
//
//
//
//		int childNum = 0;
//		float distance = 1000;
//		CvZnode* nodeNPclone;
//
//		for (float i = 0; i < 360; i += 360.0 / 16.0)
//		{
//
//			int ashMovey = floor(sin(i * PI / 180) * distance);
//			int ashMovex = floor(cos(i * PI / 180) * distance);
//
//			if (this->state->ashX + ashMovex <= 16000 && this->state->ashX + ashMovex >= 0 && this->state->ashY + ashMovey <= 9000 && this->state->ashY + ashMovey >= 0)
//			{
//				
//				nodeNPclone = new CvZnode(this);
//				nodeNPclone->state->ashX = this->state->ashX + ashMovex;
//				nodeNPclone->state->ashY = this->state->ashY + ashMovey;
//
//				nodeNPclone->killing();
//
//				this->addChild(nodeNPclone, childNum);
//				
//				
//				childNum++;
//			}
//
//		}
//
//
//
//		this->setChildrenNum(childNum);
//
//
//		return nullptr;
//	}
//
//
//
//	void evaluate()
//	{
//		float eval = this->state->score;
//
//		if (!leafCondition())
//			eval += optimalEval;
//
//
//
//		int fib = 1;
//		int fib0 = 1;
//
//		int fibOP = 1;
//		int fibOP0 = 1;
//
//		int humans = this->state->humans->total;
//		float maxScore = 0;
//
//		for (int i = 0; i < this->state->zombies->total; i++)
//		{
//			//score update
//			maxScore += pow(humans, 2) * 10 * fib;
//			fib = fib + fib0;
//			fib0 = fib - fib0;
//
//		}
//
//		eval = eval / maxScore;
//
//
//		if (this->state->humans->numAlive == 0)
//			eval = 0;
//
//		this->setEvaluation(eval);
//
//	}
//
//	int calculateChilds()
//	{
//
//		int indx = 0;
//		float distance = 1000;
//
//		for (float i = 0; i < 360; i += 360.0 / 16.0)
//		{
//
//			int ashMovey = floor(sin(i * PI / 180) * distance);
//			int ashMovex = floor(cos(i * PI / 180) * distance);
//
//			if (this->state->ashX + ashMovex <= 16000 && this->state->ashX + ashMovex >= 0 && this->state->ashY + ashMovey <= 9000 && this->state->ashY + ashMovey >= 0)
//				indx++;
//
//		}
//
//		return indx;
//	}
//
//	NodeInterfaceMCTS* getChildren(int index)
//	{
//		CvZnode* nodeNPclone;
//		int aux = index;
//
//
//		if (this->NodeInterfaceMCTS::getChildren(index) != nullptr)
//		{
//			//std::cerr << "Child get again\n";
//			return this->NodeInterfaceMCTS::getChildren(index);
//		}
//
//
//
//		float distance = 1000;
//
//		for (float i = 0; i < 360; i += 360.0 / 16.0)
//		{
//
//			int ashMovey = floor(sin(i * PI / 180) * distance);
//			int ashMovex = floor(cos(i * PI / 180) * distance);
//
//			if (this->state->ashX + ashMovex <= 16000 && this->state->ashX + ashMovex >= 0 && this->state->ashY + ashMovey <= 9000 && this->state->ashY + ashMovey >= 0)
//			{
//
//				if (aux == 0)
//				{
//					nodeNPclone = new CvZnode(this);
//					nodeNPclone->state->ashX = this->state->ashX + ashMovex;
//					nodeNPclone->state->ashY = this->state->ashY + ashMovey;
//
//					nodeNPclone->killing();
//
//					this->addChild(nodeNPclone, index);
//
//					return nodeNPclone;
//				}
//
//				aux--;
//
//			}
//
//		}
//
//
//
//
//
//
//
//
//		if (true)
//			std::cerr << "Error returning nullptr index:" << index << "\n";
//
//		return nullptr;
//	}
//
//	
//
//};
//
