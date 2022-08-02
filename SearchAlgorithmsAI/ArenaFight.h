#pragma once

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
#include "Fighter.h"



struct player
{
	int algorithm = 0; // 1 = MCTS 2 = MM
	int time = 0;




	int mctsType = 0;
	float uctConstant = 0;
	int biasConstant = 0;
	int mode = 0;

	int depth = 0;

};




class ArenaFight
{
public:

	std::vector<Fighter*> fighters;
	int fighterCount = 0;

	std::string announcementOrder[2] = { "First", "Second"};

	std::vector<std::vector<int>> wins;
	std::vector<std::vector<int>> loses;
	std::vector<std::vector<int>> ties;



	ArenaFight()
	{

	}

	void addPlayer(Fighter* f)
	{

		for (int i = 0; i < this->fighters.size(); i++)
		{
			wins[i].push_back(0);
			loses[i].push_back(0);
			ties[i].push_back(0);
		}


		f->initiate();
		this->fighters.push_back(f);

		


		std::vector<int> w(this->fighters.size()+1, 0);
		std::vector<int> l(this->fighters.size()+1, 0);
		std::vector<int> t(this->fighters.size()+1, 0);

		this->wins.push_back(w);
		this->loses.push_back(l);
		this->ties.push_back(t);


		this->fighterCount++;

	}

	void resetArena()
	{
		this->fighters.clear();
		this->fighterCount = 0;

	}


	void fight1v1(int f1, int f2)
	{
		fighters[f1]->prepareToFight(1);
		fighters[f2]->prepareToFight(-1);

		std::string move = "";
		while (true)
		{
			move = fighters[f1]->nextMove(move);
			if (move.compare("GAME OVER") == 0)
			{
				announceWinner(f1, 1, f2, 2);
				break;
			}
			//fighters[f1]->print();

			move = fighters[f2]->nextMove(move);
			if (move.compare("GAME OVER") == 0)
			{
				announceWinner(f2, 2, f1, 1);
				break;
			}
			//fighters[f2]->print();
		}



		fighters[f1]->prepareToFight(-1);
		fighters[f2]->prepareToFight(1);

		move = "";
		while (true)
		{
			move = fighters[f2]->nextMove(move);
			if (move.compare("GAME OVER") == 0)
			{
				announceWinner(f2, 1, f1, 2);
				break;
			}
			//fighters[f2]->print();

			move = fighters[f1]->nextMove(move);
			if (move.compare("GAME OVER") == 0)
			{
				announceWinner(f1, 2, f2, 1);
				break;
			}
			//fighters[f1]->print();
		}


	}



	void roundRobin(int numRounds)
	{
		for (int g = 0; g < numRounds; g++)
		{
			
			for (int i = 0; i < this->fighters.size(); i++)
			{
				for (int j = i + 1; j < this->fighters.size(); j++)
				{
					this->fight1v1(i, j);
					//this->matrixScore();
				}
			}

		}

		
	}













	void announceWinner(int fighter, int order, int fighter2, int order2)
	{
		int result = fighters[fighter]->checkWinner(order);
		int ender = fighter;

		if (fighter > fighter2)
		{
			int aux = fighter;
			fighter = fighter2;
			fighter2 = aux;

			aux = order;
			order = order2;
			order2 = aux;

			result = -result;
		}

		std::cout << "\n\n";

		switch(result)
		{
			case 1:
				std::cout << "P" << fighter << " vs P" << fighter2 << " Player " << fighter << " WINS! going " << announcementOrder[order - 1] << "\n";
				this->updateScore(fighter, fighter2, false);
				break;
			case -1:
				std::cout << "P" << fighter << " vs P" << fighter2 << " Player " << fighter2 << " WINS! going " << announcementOrder[order2 - 1] << "\n";
				this->updateScore(fighter2, fighter, false);
				break;
			case 0:
				std::cout << "P" << fighter << " vs P" << fighter2 << " The match is a TIE!" << "\n";
				this->updateScore(fighter, fighter2, true);
				break;
		}


		this->showScore(fighter, fighter2);
		fighters[ender]->print();

		this->matrixScore();

		fighters[fighter]->info();
		fighters[fighter2]->info();

		
	}


	void updateScore(int winner, int loser, bool tie)
	{
		if (tie)
		{
			this->ties[winner][loser+1]++;
			this->ties[winner][0]++;
			this->ties[loser][winner+1]++;
			this->ties[loser][0]++;
		}
		else
		{
			this->wins[winner][loser+1]++;
			this->wins[winner][0]++;
			this->loses[loser][winner+1]++;
			this->loses[loser][0]++;
		}
	}

	void showScore(int p1, int p2)
	{
		float wr1 = 100 * this->wins[p1][p2 + 1] / (float)(this->wins[p1][p2 + 1] + this->ties[p1][p2 + 1] + this->loses[p1][p2 + 1]);
		float wr2 = 100 * this->wins[p2][p1 + 1] / (float)(this->wins[p2][p1 + 1] + this->ties[p2][p1 + 1] + this->loses[p2][p1 + 1]);

		std::cout << "P" << p1 << " WR: " << wr1 << "%\n";
		std::cout << "P" << p2 << " WR: " << wr2 << "%\n";
	}

	void matrixScore()
	{
		
		std::cout << std::fixed;
		std::cout << std::setprecision(1);

		std::cout << "\n\nPlayer        ";

		for (int i = 0; i < this->fighters.size(); i++)
			std::cout << "P" << i << "      ";

		std::cout << "   Total WR" << "\n";

		for (int i = 0; i < this->fighters.size(); i++)
		{
			std::cout << "Player " << i << ": ";

			for (int j = 0; j < this->fighters.size(); j++)
			{
				float totalGames = (float)(this->wins[i][j + 1] + this->ties[i][j + 1] + this->loses[i][j + 1]);
				if (totalGames == 0)
					totalGames++;
				float score = this->wins[i][j+1] + ((float)this->ties[i][j + 1])/2;
				
				std::cout << std::setw(5) << score << "   ";
			}

			float fullGames = (float)(this->wins[i][0] + this->ties[i][0] + this->loses[i][0]);
			if (fullGames == 0)
				fullGames++;
			float fullScore = this->wins[i][0] + ((float)this->ties[i][0]) / 2;

			std::cout << "       " << fullScore << "\n";
		}
	}


};

