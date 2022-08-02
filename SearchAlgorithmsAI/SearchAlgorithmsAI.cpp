// SearchAlgorithmsAI.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <iomanip>
#include <random>
#include <chrono>
#include <Windows.h>
#include <bitset>

#include "DepthSearch.h"
#include "AStarSearch.h"
#include "Minimax.h"
#include "MCTS.h"
#include "CvZnode.h"
#include "ArenaFight.h"
#include "Fighter.h"
#include "RandomSearch.h"





void selfPlay(int startTime1, int startTime2, int time1, int time2, int type1, int type2);

void play(int mode);

void zombieProblem(int mode);

void selfPlayArena(struct player player1, struct player player2, int numGames, int timeMove);

void minimaxTest();

float evaluateMiniBoard(const char board[9], const int freePos);



int main()
{

	ArenaFight* af = new ArenaFight();

	int ms = 5;
	int player1T = ms;
	int player2T = ms;

	
	Fighter* f0 = new Fighter(new UTTTnodeMM(0, player2T), new Minimax(), player2T);

	Fighter* f1 = new Fighter(new UTTTnodeMM(1, player2T), new Minimax(), player2T);
	//Fighter* f2 = new Fighter(new UTTTnodeMM(2, player2T), new Minimax(), player2T);
	//Fighter* f4 = new Fighter(new UTTTnodeMM(4, player2T), new Minimax(), player2T);
	
	Fighter* f3 = new Fighter(new UTTTnode(0, player1T), new MCTS(), player1T);


	af->addPlayer(f0);
	af->addPlayer(f1);
	//af->addPlayer(f2);
	af->addPlayer(f3);
	/*af->addPlayer(f4);
	af->addPlayer(f5);*/
	/*af->addPlayer(f6);
	af->addPlayer(f7);*/
	//af->addPlayer(f8);

	af->roundRobin(1000);

	//for (int i = 0; i < 1000; i++)
	//{
	//	af->fight1v1(0, 1);
	//	std::cout << "Game number: " << i;
	//}


	//
	//int startTime1 = 1000;
	//int startTime2 = 1000;

	//int time1 = 100;
	//int time2 = 100;

	//unsigned seed = time(0);
	//srand(seed);

	//struct player player1;
	//player1.algorithm = 1;
	//player1.depth = 3;
	//player1.mctsType = 4;
	//player1.uctConstant = 0.6;
	//player1.biasConstant = 15; // 12->15
	//player1.time =10;
	//player1.mode = 14;

	//struct player player2;
	//player2.algorithm = 2;
	//player2.depth = 6;
	//player2.mctsType = 4;
	//player2.uctConstant = 0.6; //0.55 for 10+
	//player2.biasConstant = 15;
	//player2.time =10;
	//player2.mode = 14;




	//selfPlayArena(player1, player2, 1000, 90);



	//struct sharedCache* n = nullptr;

	//std::cerr << "Size: " << sizeof(n) << std::endl;


	//



}


/*
void selfPlay(int startTime1, int startTime2, int time1, int time2, int type1, int type2)
{
	int mode = 0;



	struct board81 boards;
	struct movements81 movements;

	boards.initiate();
	movements.initiate();






	bool leaf = false;

	NodeInterfaceMCTS* player1Origin;
	NodeInterfaceMCTS* player2Origin;
	NodeInterfaceMCTS* result1;
	NodeInterfaceMCTS* result2;
	MCTS* mc1 = nullptr;
	MCTS* mc2 = nullptr;

	std::this_thread::sleep_for(std::chrono::milliseconds(100));









	//First Move(initiation)
	//player 1

	player1Origin = new UTTTnode(1, movements, boards, -1, -1, mode);

	mc1 = new MCTS(player1Origin);

	result1 = mc1->execute(startTime1, type1);

	result1->print();

	//player 2
	char boardmove = ((UTTTnode*)result1)->boards.currentBoard;
	char movex = ((UTTTnode*)result1)->currentMove.first;
	char movey = ((UTTTnode*)result1)->currentMove.second;


	boards.rowWin[boardmove][movex] += 1;
	boards.colWin[boardmove][movey] += 1;

	if (movex == movey)
		boards.diagWin[boardmove][0] += 1;
	if (movex == 2 - movey)
		boards.diagWin[boardmove][1] += 1;

	boards.currentBoard = ((UTTTnode*)result1)->boards.currentBoard;

	boards.setBoard(boardmove, movex, movey, 1);

	player2Origin = new UTTTnode(-1, ((UTTTnode*)result1)->moves, boards, movex, movey, mode);

	mc2 = new MCTS(player2Origin);

	result2 = mc2->execute(startTime2, type2);

	result2->print();


	int ender = 0;

	while (true)
	{

		//player 1

		//Find the move by player 2
		char board = ((UTTTnode*)result2)->boards.currentBoard;
		char movex = ((UTTTnode*)result2)->currentMove.first;
		char movey = ((UTTTnode*)result2)->currentMove.second;
		bool fullBoard = false;

		if (((UTTTnode*)result1)->boards.getBBoard(((UTTTnode*)result1)->currentMove.first, ((UTTTnode*)result1)->currentMove.second) != -2)
			fullBoard = true;


		int indx = ((UTTTnode*)result1)->moves.getIndex(board, movex, movey, fullBoard);

		//update tree with move
		result1 = ((UTTTnode*)result1)->getChildren(indx);
		mc1->enemyMove(result1);

		result1 = mc1->execute(time1, type1);





		//Print
		result1->print();




		if (result1->leafCondition())
		{
			ender = 1;
			break;
		}






		//player 2

		//find the move by player 1
		board = ((UTTTnode*)result1)->boards.currentBoard;
		movex = ((UTTTnode*)result1)->currentMove.first;
		movey = ((UTTTnode*)result1)->currentMove.second;
		fullBoard = false;

		if (((UTTTnode*)result2)->boards.getBBoard(((UTTTnode*)result2)->currentMove.first, ((UTTTnode*)result2)->currentMove.second) != -2)
			fullBoard = true;


		indx = ((UTTTnode*)result2)->moves.getIndex(board, movex, movey, fullBoard);

		//update tree with move
		result2 = ((UTTTnode*)result2)->getChildren(indx);
		mc2->enemyMove(result2);

		result2 = mc2->execute(time2, type2);


		//Print
		result2->print();





		if (result2->leafCondition())
		{
			ender = -1;
			break;
		}


	}


	int end = -10;

	if (ender == 1)
	{
		end = ((UTTTnode*)result1)->checkWin();

		if (end == 1)
			std::cout << "Player " << 1 << " wins" << std::endl;
		else if (end == -1)
			std::cout << "Player " << 2 << " wins" << std::endl;
		else
			std::cout << "TIEEEEE" << std::endl;
	}
	else if (ender == -1)
	{
		end = ((UTTTnode*)result2)->checkWin();

		if (end == 1)
			std::cout << "Player " << 2 << " wins" << std::endl;
		else if (end == -1)
			std::cout << "Player " << 1 << " wins" << std::endl;
		else
			std::cout << "TIEEEEE" << std::endl;
	}



	std::cout << "End" << std::endl;
}
*/



void selfPlayArena(struct player player1, struct player player2, int numGames, int timeMove)
{

	int winsPlayer1 = 0;
	int winsFirstP1 = 0;
	int winsSecondP1 = 0;
	
	int winsPlayer2 = 0;
	int winsFirstP2 = 0;
	int winsSecondP2 = 0;

	int ties = 0;


	bool reverse = false;

	std::this_thread::sleep_for(std::chrono::milliseconds(100));

	for (int i = 0; i < numGames; i++)
	{


		

		struct board81 boards;
		struct movements81 movements;

		boards.initiate();
		movements.initiate();

		int end = -10;
		bool leaf = false;

		NodeInterfaceMCTS* player1Origin = nullptr;
		NodeInterfaceMCTS* player2Origin = nullptr;

		NodeInterfaceMM* player1OriginMM = nullptr;
		NodeInterfaceMM* player2OriginMM = nullptr;

		NodeInterfaceMCTS* result1 = nullptr;
		NodeInterfaceMCTS* result2 = nullptr;

		NodeInterfaceMM* result1MM = nullptr;
		NodeInterfaceMM* result2MM = nullptr;


		MCTS* mc1 = nullptr;
		MCTS* mc2 = nullptr;

		Minimax* mm = new Minimax();


		char boardmove;
		char movex;
		char movey;
		bool fullBoard = false;
		int indx = -1;

		bool newGame = true; 

		int turn2Player1Back = 0;
		int turn2Player2Back = 0;
		int turn2Player1Nodes = 0;
		int turn2Player2Nodes = 0;

		int turnCount = 0;


		//First Move(initiation)
		//player 1
		if (player1.algorithm == 1)
		{
			player1Origin = new UTTTnode(player1.mode, player1.time);

			mc1 = new MCTS(player1Origin);//, player1.uctConstant, player1.biasConstant);

			result1 = mc1->execute(player1.time, player1.mctsType);

			boardmove = ((UTTTnode*)result1)->currentBoard;
			movex = ((UTTTnode*)result1)->currentMove/3;
			movey = ((UTTTnode*)result1)->currentMove%3;

			//movements = ((UTTTnode*)result1)->moves;
		}
		else
		{
			//player1OriginMM = new UTTTnodeMM(1, movements, boards, -1, -1, player1.mode, player1.time);

			player1OriginMM = new UTTTnodeMM(player1.mode, player1.time);

			//mm->minimax(player1OriginMM, player1.depth, -999, 999, true);

			//result1MM = mm->selectMove(player1OriginMM, true);

			result1MM = mm->IDMinimax(player1OriginMM, true, player1.time);

			boardmove = ((UTTTnodeMM*)result1MM)->currentBoard;
			movex = ((UTTTnodeMM*)result1MM)->currentMove/3;
			movey = ((UTTTnodeMM*)result1MM)->currentMove%3;

			//movements = ((UTTTnodeMM*)result1MM)->moves;
		}


		//Updating the board
		boards.rowWin[boardmove][movex] += 1;
		boards.colWin[boardmove][movey] += 1;

		if (movex == movey)
			boards.diagWin[boardmove][0] += 1;
		if (movex == 2 - movey)
			boards.diagWin[boardmove][1] += 1;

		boards.currentBoard = boardmove;


		boards.setBoard(boardmove, movex, movey, 1);


		//char indexOfMove = movements.getIndex(boardmove, movex, movey, true);
		movements.del(boardmove, movex*3+ movey);


		

		//player 2
		if (player2.algorithm == 1)
		{
			player2Origin = new UTTTnode(player2.mode, player2.time);
			player2Origin->expand();
			result2 = (NodeInterfaceMCTS*)player2Origin->getChildMove(std::to_string(movex * 3 + movey) + " " + std::to_string(boardmove));

			mc2 = new MCTS(result2);//, player2.uctConstant, player2.biasConstant);

			result2 = mc2->execute(player2.time, player2.mctsType);


			boardmove = ((UTTTnode*)result2)->currentBoard;
			movex = ((UTTTnode*)result2)->currentMove/3;
			movey = ((UTTTnode*)result2)->currentMove%3;

			

		}
		else
		{
			//player2OriginMM = new UTTTnodeMM(-1, movements, boards, movex, movey, player2.mode, player2.time);

			player2OriginMM = new UTTTnodeMM(player2.mode, player2.time);
			player2OriginMM->expand();
			result2MM = (NodeInterfaceMM*)player2OriginMM->getChildMove(std::to_string(movex * 3 + movey) + " " + std::to_string(boardmove));

			//mm->minimax(player2OriginMM, player2.depth, -999, 999, false);

			//result2MM = mm->selectMove(player2OriginMM, false);

			result2MM = mm->IDMinimax(result2MM, false, player2.time);

			boardmove = ((UTTTnodeMM*)result2MM)->currentBoard;
			movex = ((UTTTnodeMM*)result2MM)->currentMove/3;
			movey = ((UTTTnodeMM*)result2MM)->currentMove%3;
		}

		//result2->print();


		int ender = 0;

		while (true)
		{

			turnCount++;
			//player 1

			//Find the move by player 2
			
			if (player1.algorithm == 1)
			{

				//for (int i = 0; i < result1->getChildrenNum(); i++)
				//{
				//	NodeInterfaceMCTS* child = (NodeInterfaceMCTS*)result1->getChildren(i);

				//	if (((UTTTnode*)child)->currentMove/3 == movex && ((UTTTnode*)child)->currentMove%3 == movey && ((UTTTnode*)child)->currentBoard == boardmove)
				//	{
				//		indx = i;
				//		break;
				//	}
				//}
				//result1 = (NodeInterfaceMCTS*)result1->getChildren(indx);

				result1 = (NodeInterfaceMCTS*)result1->getChildMove(std::to_string(movex * 3 + movey) + " " + std::to_string(boardmove));

				//update tree with move
				
				mc1->enemyMove(result1);

				result1 = mc1->execute(player1.time, player1.mctsType);

				leaf = result1->leafCondition();

				boardmove = ((UTTTnode*)result1)->currentBoard;
				movex = ((UTTTnode*)result1)->currentMove /3;
				movey = ((UTTTnode*)result1)->currentMove%3;

				if (newGame)
				{
					if (reverse == false)
					{
						turn2Player1Back = mc1->backTurnCounter;
						turn2Player1Nodes = mc1->nodeTurnCounter;
					}
					else
					{
						turn2Player2Back = mc1->backTurnCounter;
						turn2Player2Nodes = mc1->nodeTurnCounter;
					}

				}

			}
			else
			{
				/*
				for (int i = 0; i < result1MM->getChildrenNum(); i++)
				{
					NodeInterfaceMM* child = result1MM->getChildren(i);

					if (((UTTTnodeMM*)child)->currentMove/3 == movex && ((UTTTnodeMM*)child)->currentMove%3 == movey && ((UTTTnodeMM*)child)->currentBoard == boardmove)
					{
						indx = i;
						break;
					}
				}


				result1MM = ((UTTTnodeMM*)result1MM)->getChildren(indx);
				*/

				result1MM = (NodeInterfaceMM*)result1MM->getChildMove(std::to_string(movex * 3 + movey) + " " + std::to_string(boardmove));
				

				//mm->minimax(result1MM, player1.depth, -999, 999, true);

				//result1MM = mm->selectMove(result1MM, true);

				result1MM = mm->IDMinimax(result1MM, true, player1.time);

				leaf = result1MM->leafCondition();

				boardmove = ((UTTTnodeMM*)result1MM)->currentBoard;
				movex = ((UTTTnodeMM*)result1MM)->currentMove/3;
				movey = ((UTTTnodeMM*)result1MM)->currentMove%3;

				if (newGame)
					if (reverse == false)
						turn2Player1Nodes = mm->turnNodes;
					else
						turn2Player2Nodes = mm->turnNodes;

				//result1MM->print();
			}





			//Print
			//result1->print();




			if (leaf)
			{
				if(player1.algorithm == 1)
					end = ((UTTTnode*)result1)->checkWin();
				else
					end = ((UTTTnodeMM*)result1MM)->checkWin();


				std::cout << "Game: " << i << std::endl;

				if (end == 1)
				{
					if (reverse == false)
					{
						std::cout << "Player " << 1 << " wins (going first)" << std::endl;
						winsPlayer1++;
						winsFirstP1++;
					}
					else
					{
						std::cout << "Player " << 2 << " wins (going first)" << std::endl;
						winsPlayer2++;
						winsFirstP2++;
					}
				}
				else if (end == -1)
				{
					if (reverse == false)
					{
						std::cout << "Player " << 2 << " wins (going second)" << std::endl;
						winsPlayer2++;
						winsSecondP2++;
					}
					else
					{
						std::cout << "Player " << 1 << " wins (going second)" << std::endl;
						winsPlayer1++;
						winsSecondP1++;
					}
				}
				else
				{
					std::cout << "TIEEEEE" << std::endl;
					ties++;
				}

				if (player1.algorithm == 1)
					result1->print();
				else
					result1MM->print();

				break;
			}






			//player 2

			//find the move by player 1
			if (player2.algorithm == 1)
			{

				//for (int i = 0; i < result2->getChildrenNum(); i++)
				//{
				//	NodeInterfaceMCTS* child = (NodeInterfaceMCTS*)result2->getChildren(i);

				//	if (((UTTTnode*)child)->currentMove/3 == movex && ((UTTTnode*)child)->currentMove%3 == movey && ((UTTTnode*)child)->currentBoard == boardmove)
				//	{
				//		indx = i;
				//		break;
				//	}
				//}
				////update tree with move
				//result2 = (NodeInterfaceMCTS*)result2->getChildren(indx);

				result2 = (NodeInterfaceMCTS*)result2->getChildMove(std::to_string(movex * 3 + movey) + " " + std::to_string(boardmove));


				mc2->enemyMove(result2);

				result2 = mc2->execute(player2.time, player2.mctsType);

				leaf = result2->leafCondition();

				boardmove = ((UTTTnode*)result2)->currentBoard;
				movex = ((UTTTnode*)result2)->currentMove/3;
				movey = ((UTTTnode*)result2)->currentMove%3;

				if (newGame)
				{
					if (reverse == false)
					{
						turn2Player2Back = mc2->backTurnCounter;
						turn2Player2Nodes = mc2->nodeTurnCounter;
					}
					else
					{
						turn2Player1Back = mc2->backTurnCounter;
						turn2Player1Nodes = mc2->nodeTurnCounter;
					}

					
				}


			}
			else
			{
				/*
				if (((UTTTnodeMM*)result2MM)->boards.getBBoard(((UTTTnodeMM*)result2MM)->currentMove.first, ((UTTTnodeMM*)result2MM)->currentMove.second) != -2)
					fullBoard = true;
				else
					fullBoard = false;

				indx = ((UTTTnodeMM*)result2MM)->moves.getIndex(boardmove, movex, movey, fullBoard);
				*/

				for (int i = 0; i < result2MM->getChildrenNum(); i++)
				{
					NodeInterfaceMM* child = (NodeInterfaceMM*)result2MM->getChildren(i);

					if (((UTTTnodeMM*)child)->currentMove/3 == movex && ((UTTTnodeMM*)child)->currentMove%3 == movey && ((UTTTnodeMM*)child)->currentBoard == boardmove)
					{
						indx = i;
						break;
					}
				}


				result2MM = (NodeInterfaceMM*)((UTTTnodeMM*)result2MM)->getChildren(indx);

				//mm->minimax(result2MM, player2.depth, -999, 999, false);

				//result2MM = mm->selectMove(result2MM, false);

				result2MM = mm->IDMinimax(result2MM, false, player2.time);

				leaf = result2MM->leafCondition();

				boardmove = ((UTTTnodeMM*)result2MM)->currentBoard;
				movex = ((UTTTnodeMM*)result2MM)->currentMove/3;
				movey = ((UTTTnodeMM*)result2MM)->currentMove%3;

				if (newGame)
					if (reverse == false)
						turn2Player2Nodes = mm->turnNodes;
					else
						turn2Player1Nodes = mm->turnNodes;

				//result2MM->print();
			}
			


			//Print
			//result2->print();
			newGame = false;




			if (leaf)
			{
				if (player2.algorithm == 1)
					end = ((UTTTnode*)result2)->checkWin() * -1;
				else
					end = ((UTTTnodeMM*)result2MM)->checkWin() * -1;


				std::cout << "Game: " << i << std::endl;

				if (end == 1)
				{
					if (reverse == false)
					{
						std::cout << "Player " << 2 << " wins (going second)" << std::endl;
						winsPlayer2++;
						winsSecondP2++;
					}
					else
					{
						std::cout << "Player " << 1 << " wins (going second)" << std::endl;
						winsPlayer1++;
						winsSecondP1++;
					}
				}
				else if (end == -1)
				{
					if (reverse == false)
					{
						std::cout << "Player " << 1 << " wins (going first)" << std::endl;
						winsPlayer1++;
						winsFirstP1++;
					}
					else
					{
						std::cout << "Player " << 2 << " wins (going first)" << std::endl;
						winsPlayer2++;
						winsFirstP2++;
					}
				}
				else
				{
					std::cout << "TIEEEEE" << std::endl;
					ties++;
				}

				
				if (player2.algorithm == 1)
					result2->print();
				else
					result2MM->print();

				break;
			}


		}


		float player1Back = 0;
		int player1Nodes = 0;

		float player2Back = 0;
		int player2Nodes = 0;

		if (reverse == false)
		{
			if (player1.algorithm == 1)
			{
				player1Back = mc1->backcounter;
				player1Nodes = mc1->nodecounter;
			}
			else
			{
				player1Nodes = mm->totalNodes;
				player1Back = mm->cutoffsTotal / mm->cutoffs;
			}
			if (player2.algorithm == 1)
			{
				player2Back = mc2->backcounter;
				player2Nodes = mc2->nodecounter;
			}
			else
			{
				player2Nodes = mm->totalNodes;
				player2Back = mm->cutoffsTotal / mm->cutoffs;
			}
		}
		else
		{
			if (player2.algorithm == 1)
			{
				player1Back = mc2->backcounter;
				player1Nodes = mc2->nodecounter;
			}
			else
			{
				player1Nodes = mm->totalNodes;
				player1Back = mm->cutoffsTotal / mm->cutoffs;
			}

			if (player1.algorithm == 1)
			{
				player2Back = mc1->backcounter;
				player2Nodes = mc1->nodecounter;
			}
			else
			{
				player2Nodes = mm->totalNodes;
				player2Back = mm->cutoffsTotal / mm->cutoffs;
			}
		}


		std::cout << "WR p1: " << (((float)winsPlayer1) / (i + 1)) * 100 << "%" << std::endl;
		std::cout << "WR p2: " << (((float)winsPlayer2) / (i + 1)) * 100 << "%" << std::endl;

		std::cout << "Turn 2 BackPs:    Player 1: " << turn2Player1Back << " " << ((float)turn2Player1Back)/player1.time << " b/ms" <<  "  Player2: " << turn2Player2Back << " " << ((float)turn2Player2Back) / player2.time << " b/ms" << std::endl;
		std::cout << "Turn 2 nodes :    Player 1: " << turn2Player1Nodes << " " << ((float)turn2Player1Nodes) / player1.time << " n/ms" << "  Player2: " << turn2Player2Nodes << " " << ((float)turn2Player2Nodes) / player2.time << " n/ms" << std::endl;

		std::cout << "Backpropags  :    Player 1: " << player1Back << "  Player2: " << player2Back << std::endl;
		std::cout << "Nodes Created:    Player 1: " << player1Nodes << " " << ((float)player1Nodes)/(player1.time*turnCount) << " n/ms" << "  Player2: " << player2Nodes << " " << ((float)player2Nodes) / (player2.time * turnCount) << " n/ms" << std::endl;








		int totalGames = winsPlayer1 + winsPlayer2 + ties;

		if (totalGames % 100 == 0)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		}



		if (player1.algorithm == 1)
		{
			delete player1Origin;
		}
		else
			delete player1OriginMM;

		if (player2.algorithm == 1)
		{
			delete player2Origin;
		}
		else
			delete player2OriginMM;

		struct player aux = player1;
		player1 = player2;
		player2 = aux;


		/*
		int aux = type1;
		type1 = type2;
		type2 = aux;
		
		float auxf = UCT1;
		UCT1 = UCT2;
		UCT2 = auxf;
		*/
		reverse = !reverse;


	}

	std::cout << std::endl << std::endl;
	std::cout << "Player 1: " << std::endl;
	std::cout << "Wins:        " << winsPlayer1 << " Win rate: " << (((float)winsPlayer1)/numGames)*100 << "%"  << std::endl;
	std::cout << "Wins first:  " << winsFirstP1 << " Win rate: " << (((float)winsFirstP1) / numGames) * 100 << "%" << std::endl;
	std::cout << "Wins second: " << winsSecondP1 << " Win rate: " << (((float)winsSecondP1) / numGames) * 100 << "%" << std::endl;

	std::cout << std::endl << std::endl;
	std::cout << "Player 2: " << std::endl;
	std::cout << "Wins:        " << winsPlayer2 << " Win rate: " << (((float)winsPlayer2)/numGames)*100 << "%" << std::endl;
	std::cout << "Wins first:  " << winsFirstP2 << " Win rate: " << (((float)winsFirstP2) / numGames) * 100 << "%" << std::endl;
	std::cout << "Wins second: " << winsSecondP2 << " Win rate: " << (((float)winsSecondP2) / numGames) * 100 << "%" << std::endl;

	std::cout << std::endl;
	std::cout << "Number of ties: " << ties << " Tie rate: " << (((float)ties) / numGames) * 100 << "%" << std::endl;



	
}










/*

void play(int mode)
{


	struct board81 boards;
	struct movements81 movements;

	boards.initiate();
	movements.initiate();


	int nodeMode = 13;



	bool leaf = false;

	NodeInterfaceMCTS* playerOrigin;
	NodeInterfaceMCTS* result;
	MCTS* mc = nullptr;

	std::this_thread::sleep_for(std::chrono::milliseconds(100));



	unsigned seed = std::time(0);
	srand(seed);
	int time = 0;
	int first = 0;

	std::cerr << "You are facing the MCTS UTTT AI created by lambda" << std::endl;
	std::cerr << "Do you want to go first(1) or second(2)?" << std::endl;

	std::cin >> first;

	std::cerr << "You chose to go " << first << std::endl;

	std::cerr << "How much time per move will the AI have? (1-10 seconds)" << std::endl;


	std::cin >> time;

	std::cerr << "You chose: " << time << " seconds of thinking per turn" << std::endl;

	int playerType = 1;


	int boardopx;
	int boardopy;
	int moveopx = -1;
	int moveopy = -1;


	if (first == 1)
	{
		std::cerr << "Introduce the boardx of the move(0-2)" << std::endl;
		std::cin >> boardopx;
		std::cerr << "Introduce the boardy of the move(0-2)" << std::endl;
		std::cin >> boardopy;
		std::cerr << "Introduce your row move(0-2)" << std::endl;
		std::cin >> moveopx;
		std::cerr << "Introduce your col move(0-2)" << std::endl;
		std::cin >> moveopy;
		std::cerr << "You chose move: " << moveopx << " " << moveopy << std::endl;


		playerType = -1;
		boards.setBoard(boardopx * 3 + boardopy, moveopx, moveopy, 1);
		boards.colWin[boardopx * 3 + boardopy][moveopy] = 1;
		boards.rowWin[boardopx * 3 + boardopy][moveopx] = 1;

		if (moveopx == moveopy)
			boards.diagWin[boardopx * 3 + boardopy][0] = 1;

		if (moveopx == 2 - moveopy)
			boards.diagWin[boardopx * 3 + boardopy][1] = 1;

		movements.del(boardopx * 3 + boardopy, moveopx * 3 + moveopy);

	}








	//First Move by AI(initiation)
	playerOrigin = new UTTTnode(playerType, movements, boards, moveopx, moveopy, nodeMode);
	mc = new MCTS(playerOrigin, 0.6, 15);
	result = mc->execute(time * 1000, 4);
	result->print();




	while (true)
	{

		//Move by human
		if (((UTTTnode*)result)->boards.bigBoard[((UTTTnode*)result)->currentMove.first * 3 + ((UTTTnode*)result)->currentMove.second] == -2)
		{
			boardopx = ((UTTTnode*)result)->currentMove.first;
			boardopy = ((UTTTnode*)result)->currentMove.second;
		}
		else
		{
			std::cerr << "Introduce the boardx of the move(0-2)" << std::endl;
			std::cin >> boardopx;
			std::cerr << "Introduce the boardy of the move(0-2)" << std::endl;
			std::cin >> boardopy;
		}

		std::cerr << "Playing on board: " << boardopx << " " << boardopy << std::endl;

		std::cerr << "Introduce your row move(0-2)" << std::endl;
		std::cin >> moveopx;
		std::cerr << "Introduce your col move(0-2)" << std::endl;
		std::cin >> moveopy;
		std::cerr << "You chose move: " << moveopx << " " << moveopy << std::endl;


		//finding the move by human
		int opponentIndex = -2;
		bool fullBoard = false;


		if (((UTTTnode*)result)->boards.getBBoard(((UTTTnode*)result)->currentMove.first, ((UTTTnode*)result)->currentMove.second) != -2)
		{
			fullBoard = true;
			std::cerr << "Full board" << std::endl;
		}

		opponentIndex = ((UTTTnode*)result)->moves.getIndex(boardopx * 3 + boardopy, moveopx, moveopy, fullBoard);


		result = ((UTTTnode*)result)->getChildren(opponentIndex);

		if (result->leafCondition())
			break;

		mc->enemyMove(result);

		//Making the move of AI
		result = mc->execute(time * 1000, 4);

		result->print();

		if (result->leafCondition())
			break;

		std::cerr << "List of possible moves: " << std::endl;

		for (int i = 0; i < result->getChildrenNum(); i++)
		{
			std::cerr << "Move " << i << ": " << " Board: " << (int)((UTTTnode*)result->getChildren(i))->boards.currentBoard << " X:" << (int)((UTTTnode*)result->getChildren(i))->currentMove.first << " Y:" << (int)((UTTTnode*)result->getChildren(i))->currentMove.second << " Eval: " << ((UTTTnode*)result->getChildren(i))->getEvaluation() << std::endl;


		}


	}













}


*/









// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file













void zombieProblem(int mode)
{

	// Zombie problem

	MCTS* mc = nullptr;
	int score = 0;
	int counter = 0;
	NodeInterfaceMCTS* result;

	while (true)
	{
		auto startTimer = std::chrono::steady_clock::now();

		int x = 0;
		int y = 0;
		int humanCount = 1;

		Humans* humans = new Humans(humanCount);

		for (int i = 0; i < humanCount; i++)
		{
			int humanId = 0;
			int humanX = 8250;
			int humanY = 4500;


			humans->id[i] = humanId;
			humans->x[i] = humanX;
			humans->y[i] = humanY;
			humans->alive[i] = true;
			humans->distToZombie[i] = 100000;
		}

		int zombieCount = 1;

		Zombies* zombies = new Zombies(zombieCount);

		for (int i = 0; i < zombieCount; i++) {
			int zombieId = 0;
			int zombieX = 8250;
			int zombieY = 8999;



			zombies->id[i] = zombieId;
			zombies->targetID[i] = -2;
			zombies->x[i] = zombieX;
			zombies->y[i] = zombieY;
			zombies->alive[i] = true;
			zombies->distToTarget[i] = 100000;

		}

		NodeInterfaceMCTS* origin = nullptr;// new CvZnode(humans, zombies, x, y, score);

		if (mc == nullptr)
			mc = new MCTS(origin);

		auto endTime = std::chrono::steady_clock::now();

		int totalTime = 100;

		if (counter == 0)
		{
			totalTime = 1100;
			counter++;
		}
		auto timeSpent = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTimer).count();

		int remainingTime = totalTime - timeSpent;

		//std::cerr << "\nTime left: " << remainingTime << std::endl;

		result = mc->execute(remainingTime, mode);

		//std::cout << ((CvZnode*)result)->state->ashX << " " << ((CvZnode*)result)->state->ashY << std::endl;

		if (result->leafCondition())
			break;


	}



}



float evaluateMiniBoard(const char board[9],const int freePos)
{
	
	//Calculate if board is won and by who
	char col[3] = { 0,0,0 };
	char row[3] = { 0,0,0 };
	char diags[3] = { 0,0,0 };

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (board[i * 3 + j] == 1)
			{
				col[i]++;
				row[j]++;

				if (col[i] == 3 || row[j] == 3)
					return 1;

				if (i == j)
				{
					diags[0]++;
					if (diags[0] == 3)
						return 1;
				}
				if (i == 2 - j)
				{
					diags[1]++;
					if (diags[1] == 3)
						return 1;
				}
			}
			else if (board[i * 3 + j] == 2)
			{
				col[i]--;
				row[j]--;

				if (col[i] == -3 || row[j] == -3)
					return -1;

				if (i == j)
				{
					diags[0]--;
					if (diags[0] == -3)
						return -1;
				}
				if (i == 2 - j)
				{
					diags[1]--;
					if (diags[1] == -3)
						return -1;
				}

			}
		}
	}

	if (freePos == 0)
		return 0;

	char boardaux[9];

	

	float value = 0;
	bool flag = true;

	char colaux[3];
	char rowaux[3];
	char diagaux[3];

	float addValue = 0.001;
	int rand = std::rand();

	for (int i = 0; i < 1000; i++)
	{
		int auxFreePos = freePos;
		flag = true;

		for (int i = 0; i < 9; i++)
		{
			boardaux[i] = board[i];

			if (i < 4)
			{
				colaux[i] = col[i];
				rowaux[i] = row[i];
				diagaux[i] = diags[i];

			}

		}

		while(auxFreePos > 0 && flag)
		{

			
			int move = rand % auxFreePos;
			int play = rand % 2;

			for (int j = 0; j < 9; j++)
			{
				

				if (boardaux[j] == 0)
				{
					if (move == 0)
					{
						boardaux[j] = play;

						int x = j / 3;
						int y = j % 3;

						if (play == 0)
						{
							colaux[y]++;
							rowaux[x]++;

							if (colaux[y] == 3 || rowaux[x] == 3)
							{
								flag = false;
								value += addValue;
								break;
							}

							if (x == y)
							{
								diagaux[0]++;
								if (diagaux[0] == 3)
								{
									flag = false;
									value += addValue;
									break;
								}
							}
							if (x == 2 - y)
							{
								diagaux[1]++;
								if (diagaux[1] == 3)
								{
									flag = false;
									value += addValue;
									break;
								}
							}
						}
						else
						{
							colaux[y]--;
							rowaux[x]--;

							if (colaux[y] == -3 || rowaux[x] == -3)
							{
								flag = false;
								value -= addValue;
								break;
							}

							if (x == y)
							{
								diagaux[0]--;
								if (diagaux[0] == -3)
								{
									flag = false;
									value -= addValue;
									break;
								}
							}
							if (x == 2 - y)
							{
								diagaux[1]--;
								if (diagaux[1] == -3)
								{
									flag = false;
									value -= addValue;
									break;
								}
							}
						}

						break;
					}

					move--;
				}
			}
			
			auxFreePos--;


		}



	}





	if (value == 0)
		value = 0.00001;

	if (value > 0.95)
		value = 0.95;

	if (value < -0.95)
		value = -0.95;


	return value;

}

