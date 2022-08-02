#pragma once
#include <cmath>


class Humans
{

public:
	char* id;
	short* x;
	short* y;

	float* distToZombie;
	bool* alive;

	short numAlive;
	char total;


	Humans(int number)
	{
		this->id = new char[number];
		this->x = new short[number];
		this->y = new short[number];
		this->distToZombie = new float[number];
		this->alive = new bool[number];

		this->numAlive = number;
		this->total = number;
	}

	~Humans()
	{

		delete[] this->id;
		delete[] this->x;
		delete[] this->y;
		delete[] this->distToZombie;
		delete[] this->alive;

	}

	double distanceTo(int _x, int _y, int index)
	{
		return sqrt(pow(this->x[index] - _x, 2) + pow(this->y[index] - _y, 2));
	}

	Humans* clone()
	{
		Humans* cloned = new Humans(this->total);

		for (int i = 0; i < this->total; i++)
		{
			cloned->id[i] = this->id[i];
			cloned->x[i] = this->x[i];
			cloned->y[i] = this->y[i];
			cloned->distToZombie[i] = this->distToZombie[i];
			cloned->alive[i] = this->alive[i];
		}

		cloned->numAlive = this->numAlive;

		return cloned;


	}


};

