#pragma once
#include "Humans.h"
#include "Zombies.h"

class CvZstate
{
public:
	Humans* humans;
	Zombies* zombies;

	short ashX;
	short ashY;

	int score;

	CvZstate()
	{
		this->humans = nullptr;
		this->zombies = nullptr;
		this->ashX = 0;
		this->ashY = 0;
		this->score = 0;
	}

	CvZstate(Humans* h, Zombies* z, int _ashX, int _ashY, int _score)
	{
		this->humans = h;
		this->zombies = z;
		this->ashX = _ashX;
		this->ashY = _ashY;
		this->score = _score;

	}

	~CvZstate()
	{
		delete humans;
		delete zombies;
	}

	bool insecure()
	{

		if (ashX < 0 || ashY < 0 || ashX >16000 || ashY >9000)
			return true;

		return false;
	}

	CvZstate* clone()
	{
		CvZstate* cloned = new CvZstate();

		cloned->humans = this->humans->clone();
		cloned->zombies = this->zombies->clone();

		cloned->ashX = this->ashX;
		cloned->ashY = this->ashY;
		cloned->score = this->score;

		return cloned;
	}


};