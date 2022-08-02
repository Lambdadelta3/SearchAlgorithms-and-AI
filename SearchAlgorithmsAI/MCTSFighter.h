#pragma once
#include "Fighter.h"
#include "MCTS.h"
#include "NodeInterfaceMCTS.h"


class MCTSFighter :
    public Fighter
{





    MCTS* algorithm = nullptr;
    NodeInterfaceMCTS* node = nullptr;

    int timeMS = 0;

    int mode = 0;

    float uct = 0;
    int bias = 0;

    bool first = false;


    MCTSFighter(int t, int m, float u, int b)
    {
        this->mode = m;
        this->timeMS = t;
        this->uct = u;
        this->bias = b;
    }



    void initiate()
    {

    }


    void prepareToFight(int order)
    {
        if (node != nullptr)
        {
            while (node->getParent() != nullptr)
                node = node->getParent();

            delete node;
        }
        if (algorithm != nullptr)
            delete algorithm;


        node = new UTTTnode(order, timeMS, mode);
        algorithm = new MCTS(node, uct, bias);

        if (order == 1)
            this->first = true;
        else
            this->first = false;
    }





    std::string nextMove(std::string enemyMove)
    {

        if (node->getChildrenNum() <= 0)
        {
            //If second move
            if (enemyMove.compare("") != 0)
            {
                node->expand();
                node = node->getChildMove(enemyMove);
                algorithm->updateNode(node);
            }
        }
        //If after 2nd move
        else
            node = node->getChildMove(enemyMove);


        node = algorithm->execute(timeMS, 4);


        if (node->leafCondition())
            return "GAME OVER";


        return node->getLastMove();

    }

    int checkWinner(int order)
    {
        if (order == 1)
            return node->checkWin();
        else if (order == 2)
            return -node->checkWin();

    }

    void print()
    {
        node->print();
    }

    void info()
    {

    }




    virtual void terminate()
    {

    }








};

