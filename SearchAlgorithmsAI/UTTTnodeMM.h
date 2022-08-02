#pragma once
#include "NodeInterfaceMCTS.h"
#include "NodeInterfaceMM.h"
#include "UTTTnode.h"
#include <vector>
#include <array>
#include <iomanip>
#include <random>

#define nodeDebug false

#define NODEMSGAMEMM 400000

#define RESET   "\033[0m"
#define RED     "\033[31m"      /* Red */
#define BLUE    "\033[34m"      /* Blue */

class UTTTnodeMM;



struct sharedCache
{
	std::array<struct miniBoard, 20000> TTTpositions;

	UTTTnodeMM* nodePool = nullptr;
	int nodeIndx = -1;
	int maxNodes = 0;
};



class hashFunction
{
public:

	size_t operator()(const Node* n) const
	{
		
		return n->hashValue();
	}
};




class UTTTnodeMM :
	public NodeInterfaceMM
{
public:


	char currentBoard = -1;
	char currentMove = -1;

	std::array<short, 9> miniHashes = { 0,0,0,0,0,0,0,0,0 };

	struct sharedCache* sharedCache = nullptr;



	UTTTnodeMM()
	{

	}



	UTTTnodeMM(int mode, int timeMove)
	{

		std::cerr << "started initiation MiniMax" << std::endl;
		auto startTime = std::chrono::steady_clock::now();

		this->initiate(mode);
		this->currentBoard = 10;
		this->currentMove = -1;

		this->initiateSharedCache();
		this->initiateHashMap();
		//this->initiateNodePool(timeMove);
		this->initiateHashes();

		auto endTime = std::chrono::steady_clock::now();
		std::chrono::duration<float> time = endTime - startTime;
		std::chrono::milliseconds timemilli = std::chrono::duration_cast<std::chrono::milliseconds>(time);
		std::cerr << "Done MM, Time: " << time.count() << "  Time millis: " << timemilli.count() << std::endl;
	}


	UTTTnodeMM(UTTTnodeMM* p)
	{
		this->initiate(p);

		this->sharedCache = p->sharedCache;

		this->miniHashes = p->miniHashes;

	}

	void initiateFromPool(UTTTnodeMM* p)
	{
		this->initiate(p);

		this->sharedCache = p->sharedCache;

		this->miniHashes = p->miniHashes;
	}




	~UTTTnodeMM()
	{

		if (this->getParent() == nullptr)
		{
			//delete this->sharedCache->TTTpositions;
			for (int i = this->sharedCache->nodeIndx; i < this->sharedCache->maxNodes; i++)
			{
				//delete this->sharedCache->nodePool[i];
			}

			for (int i = 0; i < 20000; i++)
			{
				if (this->sharedCache->TTTpositions[i].movesSize > 0)
				{
					delete[] this->sharedCache->TTTpositions[i].hashP1;
					delete[] this->sharedCache->TTTpositions[i].hashP2;
					delete[] this->sharedCache->TTTpositions[i].moves;
					delete[] this->sharedCache->TTTpositions[i].capturesP1;
					delete[] this->sharedCache->TTTpositions[i].capturesP2;
				}

			}
			//getchar();
			//std::cout << "\n\n\n\ndeleteddddddddddddddddd\n\n\n\n\n\n\n";

			//delete this->sharedCache->nodePool;
			delete[] this->sharedCache->nodePool;
			delete this->sharedCache;




		}



	}


	void reset()
	{
		this->Node::reset();


		this->currentBoard = 10;
		this->currentMove = -1;
		this->miniHashes = { 0,0,0,0,0,0,0,0,0 };
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

		NodeInterfaceMM* node = (NodeInterfaceMM*)this->Node::getChildren(index);

		if (node != nullptr)
		{
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


		UTTTnodeMM* nodeNPclone = nullptr;


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

					nodeNPclone = new UTTTnodeMM(this);
					/*
					nodeNPclone = &this->sharedCache->nodePool[this->sharedCache->nodeIndx];
					nodeNPclone->initiateFromPool(this);
					this->sharedCache->nodeIndx++;
					*/
					nodeNPclone->step(currboard, move);
					this->addChild(nodeNPclone, index);

					break;
				}
			}
		}
		else
		{

			nodeNPclone = new UTTTnodeMM(this);
			/*
			nodeNPclone = &this->sharedCache->nodePool[this->sharedCache->nodeIndx];
			nodeNPclone->initiateFromPool(this);
			this->sharedCache->nodeIndx++;
			*/
			nodeNPclone->step(currboard, index);
			this->addChild(nodeNPclone, index);

		}

		return nodeNPclone;




		std::cerr << "Entering area that shouldnt enter" << std::endl;
		std::cerr << "Number of childs of this node: " << this->getChildrenNum() << std::endl;

		(void)getchar();



	}

	Node* getChildMove(std::string childMove)
	{
		std::string delimiter = " ";

		int move = std::stoi(childMove.substr(0, childMove.find(delimiter)));
		childMove.erase(0, childMove.find(delimiter) + delimiter.length());
		int board = std::stoi(childMove);

		for (int i = 0; i < this->getChildrenNum(); i++)
		{
			NodeInterfaceMM* child = (NodeInterfaceMM*)this->getChildren(i);

			if (((UTTTnodeMM*)child)->currentMove == move && ((UTTTnodeMM*)child)->currentBoard == board)
			{
				return child;
			}
		}

	}


	std::string getLastMove() { return std::to_string(currentMove) + " " + std::to_string(currentBoard); }


	void step() {}


	bool operator==(const Node* n) const
	{
		for (int i = 0; i < 9; i++)
		{
			if (this->miniHashes[i] != ((UTTTnodeMM*)n)->miniHashes[i]);
				return false;
		}
		return true;

	}

	const int hashValue() const
	{
		int sum = 0;
		for (int i = 0; i < 9; i++)
			sum += this->miniHashes[i] + i;
		return sum;
	}


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




	float captureSearch(int depth, float alpha, float beta)
	{
		int captureNum = this->captureCount();
		float standPat = this->evaluationFunction() * this->getPlayerType();

		if (standPat >= beta)
			return beta;

		if (alpha < standPat)
			alpha = standPat;

		if (this->leafCondition() || captureNum == 0 || depth == 0)
		{
			return alpha;
		}


		float value = -9999;

		struct miniBoard b = (this->sharedCache->TTTpositions)[this->miniHashes[this->currentMove]];

		bool terminal = false;
		if (b.terminal)
			terminal = true;

		//Save the state of the node
		int board = this->currentBoard;
		int move = this->currentMove;
		int type = this->getPlayerType();
		bool leaf = this->leafCondition();
		short hash;

		this->setPlayerType(-type);






		if (terminal)
		{

			for (int i = 0; i < 9; i++)
			{
				b = this->sharedCache->TTTpositions[this->miniHashes[i]];
				hash = this->miniHashes[i];
				int captures = 0;

				if (this->getMoveType() == 1)
					captures = b.captureNumP1;
				else
					captures = b.captureNumP2;

				for (int j = 0; j < captures; j++)
				{
					//make the step i
					if (this->getMoveType() == 1)
						this->step(i, b.capturesP1[j]);
					else
						this->step(i, b.capturesP2[j]);

					//Evaluate it
					value = (std::max)(value, -captureSearch(depth - 1, -beta, -alpha));

					
					//Restore the state
					this->miniHashes[i] = hash;
					this->setleaf(leaf);
					
					alpha = (std::max)(alpha, value);
					if (alpha >= beta)
					{
						this->currentBoard = board;
						this->currentMove = move;
						this->setPlayerType(type);
						return beta;
					}

					

				}

			}

		}
		else
		{

			hash = this->miniHashes[move];
			for (int i = 0; i < captureNum; i++)
			{
				//make the step i
				if (this->getMoveType() == 1)
					this->step(move, b.capturesP1[i]);
				else
					this->step(move, b.capturesP2[i]);

				//Evaluate it
				value = (std::max)(value, -captureSearch(depth - 1, -beta, -alpha));

				//Restore the state
				this->miniHashes[move] = hash;
				this->setleaf(leaf);

				alpha = (std::max)(alpha, value);
				if (alpha >= beta)
				{
					this->currentBoard = board;
					this->currentMove = move;
					this->setPlayerType(type);
					return beta;
				}

				
			}

		}

		this->currentBoard = board;
		this->currentMove = move;
		this->setPlayerType(type);

		return alpha;
	}




	float evaluateEndState()
	{
		int eval = this->checkWin();

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
				boardValues[i][j] = (this->sharedCache->TTTpositions)[this->miniHashes[(int)i * 3 + j]].eval;
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



		float eval = evalThree;


		//if (evalThree > 0)
		//	if (evalCount > evalThree)
		//		eval = evalCount;

			eval = evalThree + evalCount * (0.9 - abs(evalThree)) / (float)4;



			if (this->sharedCache->TTTpositions[this->miniHashes[this->currentMove]].terminal)
			{
				if (this->getMoveType() == 1)
				{
					eval += -0.5;
					if (eval < -limit)
						eval = -limit - 0.001;
				}
				else
				{
					eval += 0.5;
					if (eval > limit)
						eval = limit + 0.001;
				}
			}







		return eval;
	}



	bool checkEnd(char movex, char movey)
	{



		char col = 0, row = 0, diag = 0, rdiag = 0;

		for (int i = 0; i < 3; i++)
		{
			if (this->sharedCache->TTTpositions[this->miniHashes[(int)movex * 3 + i]].eval == this->getMoveType()) col++;
			if (this->sharedCache->TTTpositions[this->miniHashes[(int)i * 3 + movey]].eval == this->getMoveType()) row++;
			if (this->sharedCache->TTTpositions[this->miniHashes[(int)i * 3 + i]].eval == this->getMoveType()) diag++;
			if (this->sharedCache->TTTpositions[this->miniHashes[(int)i * 3 + 3 - i - 1]].eval == this->getMoveType()) rdiag++;
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
			if ((this->sharedCache->TTTpositions)[this->miniHashes[(int)movex * 3 + i]].eval == this->getMoveType()) col++;
			if ((this->sharedCache->TTTpositions)[this->miniHashes[(int)i * 3 + movey]].eval == this->getMoveType()) row++;
			if ((this->sharedCache->TTTpositions)[this->miniHashes[(int)i * 3 + i]].eval == this->getMoveType()) diag++;
			if ((this->sharedCache->TTTpositions)[this->miniHashes[(int)i * 3 + 3 - i - 1]].eval == this->getMoveType()) rdiag++;
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
				if (this->sharedCache->TTTpositions[this->miniHashes[i]].board[j] == 1)
					boards.board[i * 9 + j] = 1;
				else
					if (this->sharedCache->TTTpositions[this->miniHashes[i]].board[j] == 2)
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




	void initiateSharedCache()
	{
		this->sharedCache = new struct sharedCache();
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
		this->sharedCache->maxNodes = timeMove * NODEMSGAMEMM;

		if (nodeDebug)
			this->sharedCache->maxNodes *= 0.6;

		this->sharedCache->nodePool = new UTTTnodeMM[this->sharedCache->maxNodes];

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
		minib.captureNumP1 = 0;
		minib.captureNumP2 = 0;

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

		

		char capP1[9];
		char capP2[9];

		int cnt = 0;
		for (int i = 0; i < 9; i++)
		{
			if (board[i] == 0)
			{
				minib.moves[cnt] = i;

				minib.hashP1[cnt] = minib.hash + pow(3, i);
				minib.hashP2[cnt] = minib.hash + pow(3, i) * 2;


				int movex = i / 3;
				int movey = i % 3;
				int col = 0, row = 0, diag = 0, rdiag = 0;

				for (int j = 0; j < 3; j++)
				{
					if (board[movex * 3 + j] == 1) col++;
					if (board[j * 3 + movey] == 1) row++;
					if (board[j * 3 + j] == 1) diag++;
					if (board[j * 3 + 3 - j - 1] == 1) rdiag++;
				}

				for (int j = 0; j < 3; j++)
				{
					if (board[movex * 3 + j] == 2) col--;
					if (board[j * 3 + movey] == 2) row--;
					if (board[j * 3 + j] == 2) diag--;
					if (board[j * 3 + 3 - j - 1] == 2) rdiag--;
				}



				if (row == 2 || col == 2 || diag == 2 || rdiag == 2 )//this position produces a capture
				{
					capP1[minib.captureNumP1] = cnt;
					minib.captureNumP1++;
				}

				if (row == -2 || col == -2 || diag == -2 || rdiag == -2)
				{
					capP2[minib.captureNumP2] = cnt;
					minib.captureNumP2++;
				}

				cnt++;
			}
		}

		minib.capturesP1 = new char[minib.captureNumP1];
		minib.capturesP2 = new char[minib.captureNumP2];

		for (int i = 0; i < minib.captureNumP1; i++)
			minib.capturesP1[i] = capP1[i];

		for (int i = 0; i < minib.captureNumP2; i++)
			minib.capturesP2[i] = capP2[i];


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

	int captureCount() 
	{ 	
		int captures = 0;
		int childs = (this->sharedCache->TTTpositions)[this->miniHashes[this->currentMove]].movesSize;

		if (childs == 0)
		{
			if(this->getPlayerType() == 1)
				for (int i = 0; i < 9; i++)
					captures += (this->sharedCache->TTTpositions)[this->miniHashes[i]].captureNumP1;
			else
				for (int i = 0; i < 9; i++)
					captures += (this->sharedCache->TTTpositions)[this->miniHashes[i]].captureNumP2;

		}
		else
			if (this->getPlayerType() == 1)
				captures = (this->sharedCache->TTTpositions)[this->miniHashes[this->currentMove]].captureNumP1;
			else
				captures = (this->sharedCache->TTTpositions)[this->miniHashes[this->currentMove]].captureNumP2;

		return captures;
	};

	//std::vector<NodeInterfaceMM*> getChildrenCaptures() 
	//{
	//	std::vector<NodeInterfaceMM*> n;

	//	UTTTnodeMM* captureChild;

	//	struct miniBoard b = (this->sharedCache->TTTpositions)[this->miniHashes[this->currentMove]];

	//	int childs = b.movesSize;

	//	if (childs == 0)
	//	{


	//		int aux = 0;
	//		for (int i = 0; i < 9; i++)
	//		{

	//			b = this->sharedCache->TTTpositions[this->miniHashes[i]];
	//			for (int j = 0; j < b.captureNum; j++)
	//			{
	//				captureChild = new UTTTnodeMM(this);

	//				captureChild->step( i , b.captures[j]);
	//				this->addChild(captureChild, aux + b.captures[j]);
	//				n.push_back(captureChild);
	//			}
	//			aux += b.movesSize;

	//		}



	//	}
	//	else
	//	{
	//		for (int i = 0; i < b.captureNum; i++)
	//		{
	//			captureChild = new UTTTnodeMM(this);

	//			captureChild->step(this->currentMove, b.captures[i]);
	//			this->addChild(captureChild, b.captures[i]);
	//			n.push_back(captureChild);
	//		}
	//	}


	//	
	//		
	//	

	//	return n;
	//}






};//UTTTnodeMM




