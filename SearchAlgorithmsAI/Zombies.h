#pragma once
#include <cmath>

class Zombies
{
public:
	char* id;
	char* targetID;

	short* x;
	short* y;

	float* distToTarget;

	bool* alive;

	char numAlive;
	char total;

	Zombies(int number)
	{
		this->id = new char[number];
		this->targetID = new char[number];
		this->x = new short[number];
		this->y = new short[number];
		this->distToTarget = new float[number];
		this->alive = new bool[number];

		this->numAlive = number;
		this->total = number;


	}

	~Zombies()
	{

		delete[] this->id;
		delete[] this->targetID;
		delete[] this->x;
		delete[] this->y;
		delete[] this->distToTarget;
		delete[] this->alive;


	}

	double distanceTo(int _x, int _y, int index)
	{
		return sqrt(pow(this->x[index] - _x, 2) + pow(this->y[index] - _y, 2));
	}

	Zombies* clone()
	{
		Zombies* cloned = new Zombies(this->total);

		for (int i = 0; i < this->total; i++)
		{
			cloned->id[i] = this->id[i];
			cloned->targetID[i] = this->targetID[i];
			cloned->x[i] = this->x[i];
			cloned->y[i] = this->y[i];
			cloned->distToTarget[i] = this->distToTarget[i];
			cloned->alive[i] = this->alive[i];
		}

		cloned->numAlive = this->numAlive;

		return cloned;
	}
};

