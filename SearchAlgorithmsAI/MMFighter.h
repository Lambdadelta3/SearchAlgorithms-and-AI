#pragma once
#include "Fighter.h"
#include "Minimax.h"

//
//class MMFighter :
//    public Fighter
//{
//public:
//
//
//    Minimax* algorithm = nullptr;
//    NodeInterfaceMM* node = nullptr;
//
//    int timeMS=0;
//    int mode = 0;
//    bool first = false;
//
//
//    MMFighter(int t, int m)
//    {
//        this->mode = m;
//        this->timeMS = t;
//    }
//
//
//
//    void initiate()
//    {
//        
//    }
//
//
//    void prepareToFight(int order)
//    {
//        if (node != nullptr)
//        {
//            while (node->getParent() != nullptr)
//                node = node->getParent();
//
//            delete node;
//        }
//        if (algorithm != nullptr)
//            delete algorithm;
//
//
//        node = new UTTTnodeMM(order, timeMS, mode);
//        algorithm = new Minimax();
//        if (order == 1)
//            this->first = true;
//        else
//            this->first = false;
//    }
//
//    
//
//
//
//    std::string nextMove(std::string enemyMove)
//    {
//        
//        if (node->getChildrenNum() <= 0)
//        {
//            //If second move
//            if(enemyMove.compare("") != 0)
//            {
//                node->expand();
//                node = node->getChildMove(enemyMove);
//            }
//        }
//        //If after 2nd move
//        else
//            node = node->getChildMove(enemyMove);
//
//
//        node = algorithm->IDMinimax(node, first, timeMS);
//
//
//        if (node->leafCondition())
//            return "GAME OVER";
//
//
//        return node->getLastMove();
//
//    }
//
//    int checkWinner(int order)
//    {
//        if(order == 1)
//            return node->checkWin();
//        else if (order == 2)
//            return -node->checkWin();
//
//    }
//
//    void print() 
//    {
//        node->print();
//    }
//
//    void info() 
//    {
//    
//    }
//
//
//
//
//    virtual void terminate()
//    {
//
//    }
//
//
//
//
//};
//
