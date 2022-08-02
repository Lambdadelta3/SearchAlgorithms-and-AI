#pragma once
#include "NodeInterfaceMCTS.h"

#include <vector>
#include <array>
#include <iomanip>
#include <random>
#include <string>
#include <chrono>

#define NODESMSGAME 30000
#define RANDMAX 50000


#define RESET   "\033[0m"
#define RED     "\033[31m"      /* Red */
#define BLUE    "\033[34m"      /* Blue */


class UTTTnode;

struct movements81
{
	char move[9][9];
	char size[9];
	char fullSize = 81;


	void operator =(movements81 b)
	{

		for (int i = 0; i < 9; i++)
		{
			int lim = size[i] = b.size[i];

			for (int j = 0; j < lim; j++)
			{
				move[i][j] = b.move[i][j];
			}
		}
		fullSize = b.fullSize;
	}



	void del(int b, int index)
	{
		//x[b][index] = x[b][size[b] - 1];
		//y[b][index] = y[b][size[b] - 1];
		move[b][index] = move[b][size[b] - 1];
		size[b]--;
		fullSize--;
	}

	void boardEnd(char bx, char by)
	{
		fullSize = fullSize - size[bx * 3 + by];
		size[bx * 3 + by] = 0;
	}


	/*
	void add(char xval, char yval, int b, int indx)
	{
		x[b][indx] = xval;
		y[b][indx] = yval;
	}
	*/
	int sizeBoard(int a)
	{
		if (a >= 10)
			return fullSize;

		if (a < 0)
			return 0;

		return size[a];

	}

	void initiate()
	{

		for (int i = 0; i < 9; i++)
		{
			for (int j = 0; j < 9; j++)
			{
				move[i][j] = j;
			}
			size[i] = 9;
		}


	}


	int getIndex(char b, char movea, char moveb, bool fullBoard)
	{
		int indx = 0;


		if (fullBoard)
		{
			for (int i = 0; i < 9; i++)
			{
				for (int j = 0; j < size[i]; j++)
				{
					if (movea == move[i][j] / 3 && moveb == move[i][j] % 3 && b == i)
						return indx;

					indx++;
				}
			}

			std::cerr << "error        asldhashdlahsldhalshdlashdahlsdahsldahsldasldhalsdhlasdahsdlas" << std::endl;
		}
		else
		{
			for (int j = 0; j < size[b]; j++)
			{
				if (movea == move[b][j] / 3 && moveb == move[b][j] % 3)
					return indx;

				indx++;
			}
		}




		return -1;
	}

	int getBoardOfIndex(int indx)
	{

		for (int i = 0; i < 9; i++)
		{
			indx -= size[i];
			if (indx < 0)
				return i;
		}
	}









};



struct board81
{
	char board[81];
	char currentBoard = -1;
	char bigBoard[9];
	bool leaf = false;


	char rowWin[9][3];
	char colWin[9][3];
	char diagWin[9][2];

	char rowBBwin[3];
	char colBBwin[3];
	char diagBBwin[2];



	void operator =(board81 b)
	{
		for (int i = 0; i < 9; i++)
		{
			for (int j = 0; j < 9; j++)
			{
				board[i * 9 + j] = b.board[i * 9 + j];
			}
			bigBoard[i] = b.bigBoard[i];


			diagWin[i][0] = b.diagWin[i][0];
			diagWin[i][1] = b.diagWin[i][1];

			rowWin[i][0] = b.rowWin[i][0];
			rowWin[i][1] = b.rowWin[i][1];
			rowWin[i][2] = b.rowWin[i][2];

			colWin[i][0] = b.colWin[i][0];
			colWin[i][1] = b.colWin[i][1];
			colWin[i][2] = b.colWin[i][2];
		}

		for (int i = 0; i < 3; i++)
		{
			rowBBwin[i] = b.rowBBwin[i];
			colBBwin[i] = b.colBBwin[i];
			if (i < 2)
				diagBBwin[i] = b.diagBBwin[i];
		}



		currentBoard = b.currentBoard;
		leaf = b.leaf;
	}


	char getBoard(char xb, char yb, char x, char y)
	{
		return board[xb * 27 + yb * 9 + x * 3 + y];
	}

	char getBoard(char boardmove, char x, char y)
	{
		return board[boardmove * 9 + x * 3 + y];
	}


	void setBoard(char b, char posx, char posy, char val)
	{
		board[b * 9 + posx * 3 + posy] = val;
	}



	char getBBoard(char a, char b)
	{
		return bigBoard[a * 3 + b];
	}

	char getBBoard(char ab)
	{
		return bigBoard[ab];
	}


	void initiate()
	{
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				for (int k = 0; k < 3; k++)
				{
					for (int l = 0; l < 3; l++)
					{
						board[i * 27 + j * 9 + k * 3 + l] = 0;
					}

				}
				bigBoard[i * 3 + j] = -2;
			}


			rowBBwin[i] = 0;
			colBBwin[i] = 0;
			if (i < 2)
				diagBBwin[i] = 0;

		}


		for (int i = 0; i < 9; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				colWin[i][j] = 0;
				rowWin[i][j] = 0;
				if (j < 2)
					diagWin[i][j] = 0;
			}
		}



	}


};



struct miniBoard
{
	float eval = 0;
	short hash = 0;

	char* moves;
	char movesSize = 0;
	short* hashP1;
	short* hashP2;

	char captureNumP1;
	char* capturesP1;
	char captureNumP2;
	char* capturesP2;

	char board[9];

	bool terminal = false;
};


struct sharedCacheMC
{
	std::array<struct miniBoard, 20000> TTTpositions;

	UTTTnode* nodePool;
	int nodeIndx = 0;
	int maxNodes;

	int randomPool[RANDMAX];
	int randIndx = 0;

};





class UTTTnode :
	public NodeInterfaceMCTS
{




	



public:

	//struct movements81 moves;
	//struct board81 boards;




	char currentBoard = -1;
	char currentMove = -1;

	std::array<short, 9> miniHashes = { 0,0,0,0,0,0,0,0,0 };

	struct sharedCacheMC* sharedCache = nullptr;


	UTTTnode()
	{

	}



	UTTTnode(int mode, int timeMove)
	{

		std::cerr << "started initiation MCTS" << std::endl;
		auto startTime = std::chrono::steady_clock::now();

		this->NodeInterfaceMCTS::initiate(mode);

		this->currentBoard = 10;
		this->currentMove = -1;

		this->initiateSharedCache();
		this->initiateRand();
		this->initiateHashMap();
		//this->initiateNodePool(timeMove);
		this->initiateHashes();


		auto endTime = std::chrono::steady_clock::now();
		std::chrono::duration<float> time = endTime - startTime;
		std::chrono::milliseconds timemilli = std::chrono::duration_cast<std::chrono::milliseconds>(time);
		std::cerr << "Done MCTS, Time: " << time.count() << "  Time millis: " << timemilli.count() << std::endl;

	}







	UTTTnode(UTTTnode* p)
	{
		this->NodeInterfaceMCTS::initiate(p);


		//this->playerType = p->playerType;

		this->sharedCache = p->sharedCache;
		this->miniHashes = p->miniHashes;

	}

	void initiateFromPool(UTTTnode* p)
	{
		this->NodeInterfaceMCTS::initiate(p);

		//this->playerType = p->playerType;

		this->sharedCache = p->sharedCache;
		this->miniHashes = p->miniHashes;
	}



	~UTTTnode()
	{
		if (this->getParent() == nullptr)
		{
			for (int i = 0; i < 20000; i++)
			{
				if (this->sharedCache->TTTpositions[i].movesSize > 0)
				{
					delete[] this->sharedCache->TTTpositions[i].hashP1;
					delete[] this->sharedCache->TTTpositions[i].hashP2;
					delete[] this->sharedCache->TTTpositions[i].moves;
				}
			}
			delete[] this->sharedCache->nodePool;
			delete this->sharedCache;
		}
	}




	int expand()
	{


		if (this->getExpanded())
			std::cerr << "expanding more tha once warning\n";

		this->setExpanded();

		//leaf node condition
		if (this->leafCondition())
		{
			this->setChildrenNum(0);
			return 0;
		}


		char currboard = -1;


		int childs = 0;

		if (this->currentMove < 0)
			childs = 0;
		else
			childs = (this->sharedCache->TTTpositions)[this->miniHashes[this->currentMove]].movesSize;

		if (childs == 0)
		{
			for (int i = 0; i < 9; i++)
				childs += (this->sharedCache->TTTpositions)[this->miniHashes[i]].movesSize;

		}

		this->setChildrenNum(childs);

		this->sizeChilds(childs);


		return childs;


	}




	Node* getChildren(int index)
	{

		NodeInterfaceMCTS* node = (NodeInterfaceMCTS*)this->Node::getChildren(index);

		if (node != nullptr)
		{
			//std::cerr << "Child get again\n";
			return node;
		}


		char currboard = -1;


		int childs = 0;

		if (this->currentMove < 0)
			childs = 0;
		else
			childs = (this->sharedCache->TTTpositions)[this->miniHashes[this->currentMove]].movesSize;//childs = this->miniBoards[movex * 3 + movey]->movesSize;
		bool fullMoves = false;

		if (childs == 0)
		{
			childs = this->getChildrenNum();
			fullMoves = true;
		}
		else
			currboard = this->currentMove;


		UTTTnode* nodeNPclone = nullptr;


		if (fullMoves)
		{
			int aux = 0;
			for (int i = 0; i < 9; i++)
			{
				int auxSize = this->sharedCache->TTTpositions[this->miniHashes[i]].movesSize;

				if (index >= aux + auxSize)
					aux += auxSize;
				else
				{
					currboard = i;
					int move = index - aux;
					/*
					if (true)//this->mode == 14)
					{
						nodeNPclone = &this->sharedCache->nodePool[this->sharedCache->nodeIndx];
						nodeNPclone->initiateFromPool(this);
						this->sharedCache->nodeIndx++;
					}
					else
					*/
					nodeNPclone = new UTTTnode(this);

					nodeNPclone->step(currboard, move);
					this->addChild(nodeNPclone, index);

					break;
				}
			}
		}
		else
		{
			/*
			if (true)//this->mode == 14)
			{
				nodeNPclone = &this->sharedCache->nodePool[this->sharedCache->nodeIndx];
				nodeNPclone->initiateFromPool(this);
				this->sharedCache->nodeIndx++;
			}
			else
			*/
			nodeNPclone = new UTTTnode(this);
			nodeNPclone->step(currboard, index);
			this->addChild(nodeNPclone, index);

		}

		return nodeNPclone;




		std::cerr << "Entering area that shouldnt enter" << std::endl;
		std::cerr << "Number of childs of this node: " << this->getChildrenNum() << std::endl;

		getchar();

		return nullptr;

	}



	Node* getChildMove(std::string childMove)
	{
		std::string delimiter = " ";

		int move = std::stoi(childMove.substr(0, childMove.find(delimiter)));
		childMove.erase(0, childMove.find(delimiter) + delimiter.length());
		int board = std::stoi(childMove);

		for (int i = 0; i < this->getChildrenNum(); i++)
		{
			NodeInterfaceMCTS* child = (NodeInterfaceMCTS*)this->getChildren(i);

			if (((UTTTnode*)child)->currentMove == move && ((UTTTnode*)child)->currentBoard == board)
			{
				return child;
			}
		}

	}


	std::string getLastMove() { return std::to_string(currentMove) + " " + std::to_string(currentBoard); }

	void reset()
	{
		this->NodeInterfaceMCTS::reset();


		this->currentBoard = 10;
		this->currentMove = -1;
		this->miniHashes = { 0,0,0,0,0,0,0,0,0 };
		this->sharedCache->randIndx = rand() % 10000;
	}




	void playout()
	{
		float eval = 0;


		//store state
		std::array<short, 9> auxHash = this->miniHashes;
		char auxmt = this->getMoveType();
		char moveaux = this->currentMove;
		char currentBoardaux = this->currentBoard;
		bool l = this->leafCondition();

		this->checkEndRand();

		while (!this->leafCondition())
		{
			this->setMoveType(-this->getMoveType());
			this->step();
		}


		//evaluate leaf state
		eval = this->evaluateEndState();




		//restore state
		this->miniHashes = auxHash;
		this->setMoveType(auxmt);
		this->currentMove = moveaux;
		this->currentBoard = currentBoardaux;
		this->setleaf(l);






		this->setBias(evaluationFunction() * this->getMoveType());


		this->setEvaluation(eval * this->getMoveType());

	}



	void step()
	{
		bool fullBoard = false;
		int moveIndx = -1;


		char boardPlay = this->currentMove;

		int size = 0;

		if (this->sharedCache->TTTpositions[this->miniHashes[boardPlay]].terminal)
		{
			fullBoard = !fullBoard;
			for (int i = 0; i < 9; i++)
				size += this->sharedCache->TTTpositions[this->miniHashes[i]].movesSize;
		}
		else
		{
			size = this->sharedCache->TTTpositions[this->miniHashes[boardPlay]].movesSize;
		}


		//get a random move
		int randMove = 0;
		randMove = this->getRand() % size;


		//Find move
		if (!fullBoard) //case of playing on a board 1/9
		{
			moveIndx = randMove;
		}
		else//case of playing on one of all the boards 1/81
		{

			int aux = 0;
			for (int i = 0; i < 9; i++)
			{
				int auxSize = this->sharedCache->TTTpositions[this->miniHashes[i]].movesSize;

				if (randMove >= aux + auxSize)
					aux += auxSize;
				else
				{
					boardPlay = i;
					moveIndx = randMove - aux;

					break;
				}
			}
		}

		//Make move
		this->currentMove = this->sharedCache->TTTpositions[this->miniHashes[boardPlay]].moves[moveIndx];
		this->currentBoard = boardPlay;

		//Updating hashes
		if (this->getMoveType() == 1)
			this->miniHashes[boardPlay] = this->sharedCache->TTTpositions[this->miniHashes[boardPlay]].hashP1[moveIndx];
		else
			this->miniHashes[boardPlay] = this->sharedCache->TTTpositions[this->miniHashes[boardPlay]].hashP2[moveIndx];


		//check if win/loss
		if (this->sharedCache->TTTpositions[this->miniHashes[boardPlay]].terminal)
			this->checkEnd(boardPlay / 3, boardPlay % 3);

	}




	/*
	void step()
	{
		int currboard = -1;
		int boardPlay = -1;
		int moveIndx = -1;
		char movex = -1, movey = -1;


		movex = this->currentMove.first;
		movey = this->currentMove.second;

		currboard = movex * 3 + movey;
		boardPlay = currboard;

		int size;

		if (this->boards.bigBoard[currboard] != -2)
		{
			currboard = 10;
			size = this->moves.fullSize;
		}
		else
		{
			size = this->moves.size[currboard];
		}
		int randMove = 0;
		//get a random move


		randMove = this->getRand() % size;


		//Find move
		if (currboard < 10) //case of playing on a board 1/9
		{
			moveIndx = randMove;
		}
		else//case of playing on one of all the boards 1/81
		{

			int aux = 0;
			for (int i = 0; i < 9; i++)
			{
				int auxSize = moves.size[i];

				if (randMove >= aux + auxSize)
					aux += auxSize;
				else
				{
					boardPlay = i;
					moveIndx = randMove - aux;

					break;
				}
			}
		}

		//Make move
		movex = this->moves.move[boardPlay][moveIndx]/3;
		movey = this->moves.move[boardPlay][moveIndx]%3;

		this->boards.board[boardPlay*9 + movex*3 + movey] = this->moveType; // ??????

		//Save the move
		this->currentMove.first = movex;
		this->currentMove.second = movey;
		this->boards.currentBoard = boardPlay;


		//Delete from moves
		//moves.del(boardPlay, moveIndx);
		//this->moves.x[boardPlay][moveIndx] = this->moves.x[boardPlay][this->moves.size[boardPlay] - 1];
		//this->moves.y[boardPlay][moveIndx] = this->moves.y[boardPlay][this->moves.size[boardPlay] - 1];
		this->moves.move[boardPlay][moveIndx] = this->moves.move[boardPlay][this->moves.size[boardPlay] - 1];
		this->moves.size[boardPlay]--;
		this->moves.fullSize--;




		//check if win/loss
		this->checkEndBoard(boardPlay, movex, movey);

	}

	*/




	void step(int boardIndx, int moveIndx)
	{
		//do the Move
		this->currentMove = this->sharedCache->TTTpositions[this->miniHashes[boardIndx]].moves[moveIndx];
		this->currentBoard = boardIndx;

		//Updating hashes
		if (this->getMoveType() == 1)
			this->miniHashes[boardIndx] = this->sharedCache->TTTpositions[this->miniHashes[boardIndx]].hashP1[moveIndx];
		else
			this->miniHashes[boardIndx] = this->sharedCache->TTTpositions[this->miniHashes[boardIndx]].hashP2[moveIndx];

		//check if win/loss
		if (this->sharedCache->TTTpositions[this->miniHashes[boardIndx]].terminal)
			this->checkEnd(boardIndx / 3, boardIndx % 3);

	}



	float evaluateEndState()
	{
		int eval = this->checkWin();

		if (eval == -2)
		{
			std::cerr << std::endl << "*************Error no one won and eval was set to -2*************" << std::endl << std::endl;
			getchar();
		}


		return eval;
	}


	float evaluationFunction()
	{
		if (this->leafCondition())
			return this->checkWin();


		float evalCount = 0;
		float constantCount = 1;

		float evalThree = 0;
		float constantBB = 0.05;
		float limit = 0.9;




		float boardValues[3][3] = { 0,0,0,0,0,0,0,0,0 };


		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				boardValues[i][j] = (this->sharedCache->TTTpositions)[this->miniHashes[i * 3 + j]].eval;
				evalCount += boardValues[i][j] * constantCount;
			}
		}
		if (evalCount > limit)
			evalCount = limit;
		if (evalCount < -limit)
			evalCount = -limit;







		float row[3] = { 0,0,0 };
		float col[3] = { 0,0,0 };
		float diag = 0;
		float rdiag = 0;




		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{

				row[i] += boardValues[i][j];
				col[j] += boardValues[i][j];

				if (i == j)
					diag += boardValues[i][j];

				if (i == 2 - j)
					rdiag += boardValues[i][j];

			}
		}

		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				if (boardValues[i][j] == 0)
				{

					row[i] = 0;
					col[j] = 0;

					if (i == j)
						diag = 0;

					if (i == 2 - j)
						rdiag = 0;

				}
			}
		}


		for (int i = 0; i < 3; i++)
		{
			evalThree += row[i] * abs(row[i]) * abs(row[i]) * constantBB;
			evalThree += col[i] * abs(col[i]) * abs(col[i]) * constantBB;
		}
		evalThree += diag * abs(diag) * abs(diag) * constantBB;
		evalThree += rdiag * abs(rdiag) * abs(rdiag) * constantBB;


		if (evalThree > limit)
			evalThree = limit;

		if (evalThree < -limit)
			evalThree = -limit;





		//evalCount = evalCount * this->getMoveType();
		//evalThree = evalThree * this->getMoveType();




		float eval = evalThree;


			if (evalThree * this->getMoveType() > 0)
				if (evalCount * this->getMoveType() > evalThree * this->getMoveType())
					eval = evalCount;



			if (this->sharedCache->TTTpositions[this->miniHashes[this->currentMove]].terminal)
				;// eval *= 0.8;



		




		return eval;
	}




	bool checkEndBoard(char boardmove, char movex, char movey)
	{

		float boardEval = (this->sharedCache->TTTpositions)[this->miniHashes[boardmove]].eval;

		if (boardEval == this->getMoveType() || boardEval == 0)
		{
			this->checkEnd(boardmove / 3, boardmove % 3);
			return true;
		}

		return false;

	}







	bool checkEnd(char movex, char movey)
	{


		char col = 0, row = 0, diag = 0, rdiag = 0;

		for (int i = 0; i < 3; i++)
		{
			if (this->sharedCache->TTTpositions[this->miniHashes[movex * 3 + i]].eval == this->getMoveType()) col++;
			if (this->sharedCache->TTTpositions[this->miniHashes[i * 3 + movey]].eval == this->getMoveType()) row++;
			if (this->sharedCache->TTTpositions[this->miniHashes[i * 3 + i]].eval == this->getMoveType()) diag++;
			if (this->sharedCache->TTTpositions[this->miniHashes[i * 3 + 3 - i - 1]].eval == this->getMoveType()) rdiag++;
		}


		if (row == 3 || col == 3 || diag == 3 || rdiag == 3)
		{
			this->setleaf();
			return true;
		}

		char size = 0;
		for (int i = 0; i < 9; i++)
			size += (this->sharedCache->TTTpositions)[this->miniHashes[i]].movesSize;

		if (size == 0)
			this->setleaf();




		return this->leafCondition();

	}

	int checkWin()
	{
		char col = 0, row = 0, diag = 0, rdiag = 0;
		char movex = this->currentBoard / 3;
		char movey = this->currentBoard % 3;


		for (int i = 0; i < 3; i++)
		{
			if ((this->sharedCache->TTTpositions)[this->miniHashes[movex * 3 + i]].eval == this->getMoveType()) col++;
			if ((this->sharedCache->TTTpositions)[this->miniHashes[i * 3 + movey]].eval == this->getMoveType()) row++;
			if ((this->sharedCache->TTTpositions)[this->miniHashes[i * 3 + i]].eval == this->getMoveType()) diag++;
			if ((this->sharedCache->TTTpositions)[this->miniHashes[i * 3 + 3 - i - 1]].eval == this->getMoveType()) rdiag++;
		}



		if (row == 3 || col == 3 || diag == 3 || rdiag == 3)
			return this->getMoveType();


		int count = 0;

		for (int i = 0; i < 9; i++)
		{
			count += this->sharedCache->TTTpositions[miniHashes[i]].eval;

			if (this->sharedCache->TTTpositions[miniHashes[i]].eval != -1 && this->sharedCache->TTTpositions[miniHashes[i]].eval != 1 && this->sharedCache->TTTpositions[miniHashes[i]].eval != 0)
				return -2;
		}

		if (count > 0)
			return 1;
		else if (count < 0)
			return -1;
		else
			return 0;
	}








	void print()
	{
		char type;
		if ((int)this->getMoveType() == 1)
			type = 'X';
		else
			type = 'O';

		struct board81 boards;

		for (int i = 0; i < 9; i++)
		{
			if (this->sharedCache->TTTpositions[this->miniHashes[i]].eval == 1)
				boards.bigBoard[i] = 1;
			else if (this->sharedCache->TTTpositions[this->miniHashes[i]].eval == -1)
				boards.bigBoard[i] = -1;
			else if (this->sharedCache->TTTpositions[this->miniHashes[i]].eval == 0)
				boards.bigBoard[i] = 0;
			else
				boards.bigBoard[i] = -2;

			for (int j = 0; j < 9; j++)
			{
				if(this->sharedCache->TTTpositions[this->miniHashes[i]].board[j] == 1)
					boards.board[i * 9 + j] = 1;
				else 
					if(this->sharedCache->TTTpositions[this->miniHashes[i]].board[j] == 2)
						boards.board[i * 9 + j] = -1;
					else
						boards.board[i * 9 + j] = 0;
			}
		}


		std::cerr << "Board: " << (int)this->currentBoard << " Move: " << (int)this->currentMove / 3 << " " << (int)this->currentMove % 3 << "   Type: " << type << "  PlayerType: " << (int)this->getMoveType() << std::endl;

		std::cerr << "\n --------------------------------------------" << std::endl;
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				for (int k = 0; k < 3; k++)
				{
					for (int l = 0; l < 3; l++)
					{

						char val = ' ';


						switch (boards.bigBoard[i * 3 + k])
						{
						case 0:
							val = 'T';
							break;
						case 1:
							val = 'X';
							break;
						case -1:
							val = 'O';
							break;
						case -2:
							if (boards.getBoard(i * 3 + k, j, l) == 1)
								val = 'X';

							if (boards.getBoard(i * 3 + k, j, l) == -1)
								val = 'O';
							break;
						default:
							val = 'E';
							break;
						}


						if (val == 'X')
							std::cerr << " | " << RED << val << RESET;
						else if (val == 'O')
							std::cerr << " | " << BLUE << val << RESET;
						else
							std::cerr << " | " << val;
					}
					std::cerr << " ||";
				}
				std::cerr << std::endl;

			}
			std::cerr << " --------------------------------------------" << std::endl;
		}
	}




	void initiateRand()
	{

		std::srand(time(NULL));

		for (int i = 0; i < RANDMAX; i++)
		{
			this->sharedCache->randomPool[i] = std::rand();
		}
	}

	int getRand()
	{
		int value = this->sharedCache->randomPool[this->sharedCache->randIndx];
		this->sharedCache->randIndx++;

		return value;
	}

	void checkEndRand()
	{

		if (this->sharedCache->randIndx > RANDMAX-100)
		{
			this->sharedCache->randIndx = 0;
		}

	}



	void initiateSharedCache()
	{
		this->sharedCache = new struct sharedCacheMC();
	}


	void initiateHashes(struct board81 b)
	{
		for (int i = 0; i < 9; i++)
		{
			this->miniHashes[i] = 0;
			for (int j = 0; j < 9; j++)
			{
				if (b.board[i * 9 + j] == 1)
					this->miniHashes[i] += pow(3, j);
				else if (b.board[i * 9 + j] == -1)
					this->miniHashes[i] += pow(3, j) * 2;
			}
		}
	}

	void initiateHashes()
	{
		for (int i = 0; i < 9; i++)
		{
			this->miniHashes[i] = 0;
		}
	}



	void initiateNodePool(int timeMove)
	{
		this->sharedCache->nodeIndx = 0;
		this->sharedCache->maxNodes = timeMove * NODESMSGAME;


		this->sharedCache->nodePool = new UTTTnode[this->sharedCache->maxNodes];

	}


	void initiateHashMap()
	{

		char board[9] = { 0, 0, 0, 0, 0, 0, 0 ,0 ,0 };

		//inserting the empty board
		struct miniBoard emptyb = evaluateMiniBoard(board, 9, 0);
		this->sharedCache->TTTpositions[0] = emptyb;



		char boardRotations[8][9] = {};

		int counter = 0;
		int counterWins = 0;

		for (int i = 0; i < 9; i++)
		{
			for (; ; )
			{
				board[i]++;

				if (board[i] > 2)
				{
					board[i] = 0;
					break;
				}

				counter++;


				short hashValue = 0;
				int openSpaces = 0;
				for (int k = 0; k < 9; k++)
				{
					if (board[k] == 0)
						openSpaces++;
					else
						hashValue = hashValue + pow(3, k) * board[k];

				}


				struct miniBoard minib;

				minib = evaluateMiniBoard(board, openSpaces, hashValue);

				if (hashValue > 20000 || hashValue < 0)
					getchar();

				this->sharedCache->TTTpositions.at(hashValue) = minib;



				if (i != 0)
				{
					i = -1;
					break;
				}

			}
		}
	}




	struct miniBoard evaluateMiniBoard(const char board[9], const int freePos, const short hash)
	{

		//Calculate if board is won and by who
		char col[3] = { 0,0,0 };
		char row[3] = { 0,0,0 };
		char diags[3] = { 0,0,0 };

		struct miniBoard minib;
		minib.movesSize = 0;
		minib.hash = hash;
		minib.terminal = true;

		for (int i = 0; i < 9; i++)
			minib.board[i] = board[i];


		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				if (board[i * 3 + j] == 1)
				{
					col[j]++;
					row[i]++;

					if (col[j] == 3 || row[i] == 3)
					{
						minib.eval = 1;
						return minib;
					}
					if (i == j)
					{
						diags[0]++;
						if (diags[0] == 3)
						{
							minib.eval = 1;
							return minib;
						}
					}
					if (i == 2 - j)
					{
						diags[1]++;
						if (diags[1] == 3)
						{
							minib.eval = 1;
							return minib;
						}
					}
				}
				else if (board[i * 3 + j] == 2)
				{
					col[j]--;
					row[i]--;

					if (col[j] == -3 || row[i] == -3)
					{
						minib.eval = -1;
						return minib;
					}

					if (i == j)
					{
						diags[0]--;
						if (diags[0] == -3)
						{
							minib.eval = -1;
							return minib;
						}
					}
					if (i == 2 - j)
					{
						diags[1]--;
						if (diags[1] == -3)
						{
							minib.eval = -1;
							return minib;
						}
					}

				}
			}
		}

		if (freePos == 0)
		{
			minib.eval = 0;
			return minib;
		}

		minib.movesSize = freePos;
		minib.terminal = false;
		minib.moves = new char[freePos];
		minib.hashP1 = new short[freePos];
		minib.hashP2 = new short[freePos];

		int cnt = 0;
		for (int i = 0; i < 9; i++)
		{
			if (board[i] == 0)
			{
				minib.moves[cnt] = i;

				minib.hashP1[cnt] = minib.hash + pow(3, i);
				minib.hashP2[cnt] = minib.hash + pow(3, i) * 2;




				


				cnt++;
			}
		}



		float value = 0;
		float sbconstant = 0.05;


		float multipleX = 1;
		float multipleO = 1;
		float multiple = 1;

		for (int i = 0; i < 3; i++)
		{

			if (row[i] > 0)
				multiple = multipleX;
			else
				multiple = multipleO;


			value += row[i] * pow(abs(row[i]), 2) * multiple;

			if (row[i] == 2)
				multipleX = 0.33;
			if (row[i] == -2)
				multipleO = 0.33;





			if (col[i] > 0)
				multiple = multipleX;
			else
				multiple = multipleO;


			value += col[i] * pow(abs(col[i]), 2) * multiple;


			if (col[i] == 2)
				multipleX = 0.33;
			if (col[i] == -2)
				multipleO = 0.33;





			if (diags[i] > 0)
				multiple = multipleX;
			else
				multiple = multipleO;

			value += diags[i] * pow(abs(diags[i]), 2) * multiple;


			if (diags[i] == 2)
				multipleX = 0.33;
			if (diags[i] == -2)
				multipleO = 0.33;

		}


		value = value * sbconstant;

		float limit = 0.9;

		if (value == 0)
			value = 0.00001;

		if (value > limit)
			value = limit;

		if (value < -limit)
			value = -limit;


		minib.eval = value;

		return minib;

	}














};//UTTnode


