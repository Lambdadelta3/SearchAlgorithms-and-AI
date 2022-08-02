
#include "NodeInterfaceMCTS.h"
#include <vector>
#include <array>
#include <iomanip>




struct movements
{
    std::array<char, 9> x;
    std::array<char, 9> y;
    int size = 9;


    void operator =(movements b)
    {
        x = b.x;
        y = b.y;
        size = b.size;
    }

    int sizeArray()
    {
        return size;
    }

    void del(int index)
    {
        x[index] = x[size - 1];
        y[index] = y[size - 1];
        size--;
    }

    void add(char a, char b, int indx)
    {
        x[indx] = a;
        y[indx] = b;
    }

};






class TTTnode :
    public NodeInterfaceMCTS
{
public:

    struct movements moves;
    char board[3][3]; 
    char moveType;
    bool leaf;

    char playerType;
    std::pair<char, char> currentMove;



    TTTnode(char movTyp, struct movements m, char** b)
    {
        this->initiate(0);
        

        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
                this->board[i][j] = b[i][j];

        this->moves = m;

        this->leaf = false;
        this->moveType = movTyp;
        this->playerType = movTyp;

        this->sizeChilds(this->moves.sizeArray());

    }

    TTTnode(TTTnode* p)
    {
        this->initiate(p);

        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
                this->board[i][j] = p->board[i][j];

        this->moves = p->moves;

        this->leaf = false;
        this->moveType = p->moveType;
        this->playerType = p->playerType;

        this->sizeChilds(this->moves.sizeArray());
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



        int childs = this->moves.sizeArray();

        this->setChildrenNum(childs);

        TTTnode* nodeNPclone;
        for (int i = 0; i < childs; i++)
        {

            nodeNPclone = new TTTnode(this);
            nodeNPclone->step(i);

            this->addChild(nodeNPclone, i);

        }
        


        return childs;
    }




    NodeInterfaceMCTS* getChildren(int index)
    {
        
        NodeInterfaceMCTS* node = (NodeInterfaceMCTS*)this->NodeInterfaceMCTS::getChildren(index);

        if (node != nullptr)
        {
            //std::cerr << "Child get again\n";
            return node;
        }

        TTTnode* nodeNPclone;
        nodeNPclone = new TTTnode(this);
        nodeNPclone->step(index);

        this->addChild(nodeNPclone, index);

        return nodeNPclone;


    }



    float evaluationFunction()
    {
        return 0;
    }
    std::string getLastMove() { return " no "; }
    int checkWin()
    {
        return 0;
    }

    






    void playout()
    {
        //store state
        struct movements back = this->moves;

        char backboard[3][3];

        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
                backboard[i][j] = this->board[i][j];

        bool auxl = this->leaf;
        char auxmt = this->moveType;


        //playout
        while (!this->leafCondition())
        {
            this->step();
            this->moveType = -this->moveType ;
        }

        //evaluate leaf state
        this->evaluate();


        //restore state
        this->moves = back;

        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
                this->board[i][j] = backboard[i][j];

        this->leaf = auxl;
        this->moveType = auxmt;

    }


    void step()
    {
        //get a random move
        int randMove = rand() % moves.sizeArray();

        //do the Move
        board[moves.x[randMove]][moves.y[randMove]] = this->moveType; // ??????

        //Delete from moves
        moves.del(randMove);

        //check if win/loss
        this->checkEnd();

    }

    void step(int indx)
    {
        //do the Move
        board[moves.x[indx]][moves.y[indx]] = this->moveType; // ??????

        //Save the move
        this->currentMove.first  = this->moves.x[indx];
        this->currentMove.second = this->moves.y[indx];

        //Delete from moves
        moves.del(indx);

        //check if win/loss
        if (!this->checkEnd())
            this->moveType = -this->moveType;
    }



    bool leafCondition()
    {
        return leaf;
    }


    void evaluate()
    {
        int eval = 1;// this->moveType* this->playerType;

        if (!this->checkWin())
            eval = 0;
        else
        {
            //this->print();
        }
            

        this->setEvaluation(eval);
    }


    void saveState()
    {
        
    }

    void restoreState()
    {
       
    }






    bool checkEnd()
    {
        //Check rows
        if ((board[0][0] == board[0][1] && board[0][0] == board[0][2] && board[0][0] != 0) ||
            (board[1][0] == board[1][1] && board[1][0] == board[1][2] && board[1][0] != 0) ||
            (board[2][0] == board[2][1] && board[2][0] == board[2][2] && board[2][0] != 0))
            this->leaf = true;

        //Check columns
        if ((board[0][0] == board[1][0] && board[0][0] == board[2][0] && board[0][0] != 0) ||
            (board[0][1] == board[1][1] && board[0][1] == board[2][1] && board[0][1] != 0) ||
            (board[0][2] == board[1][2] && board[0][2] == board[2][2] && board[0][2] != 0))
            this->leaf = true;

        //Check diagonals
        if ((board[0][0] == board[1][1] && board[0][0] == board[2][2] && board[0][0] != 0) ||
            (board[0][2] == board[1][1] && board[0][2] == board[2][0] && board[0][2] != 0))
            this->leaf = true;

        //Check out of moves
        if (this->moves.sizeArray() == 0)
            this->leaf = true;

        return leaf;

    }
    /*
    bool checkWin()
    {
        bool win = false;

        //Check rows
        if ((board[0][0] == board[0][1] && board[0][0] == board[0][2] && board[0][0] != 0) ||
            (board[1][0] == board[1][1] && board[1][0] == board[1][2] && board[1][0] != 0) ||
            (board[2][0] == board[2][1] && board[2][0] == board[2][2] && board[2][0] != 0))
            win = true;

        //Check columns
        if ((board[0][0] == board[1][0] && board[0][0] == board[2][0] && board[0][0] != 0) ||
            (board[0][1] == board[1][1] && board[0][1] == board[2][1] && board[0][1] != 0) ||
            (board[0][2] == board[1][2] && board[0][2] == board[2][2] && board[0][2] != 0))
            win = true;

        //Check diagonals
        if ((board[0][0] == board[1][1] && board[0][0] == board[2][2] && board[0][0] != 0) ||
            (board[0][2] == board[1][1] && board[0][2] == board[2][0] && board[0][2] != 0))
            win = true;

        return win;

    }
    */


    void print()
    {
        std::cout << "Move: " << (int)this->currentMove.first << " " << (int)this->currentMove.second << "   Type: " << (int)this->moveType << "  PlayerType: " << (int)this->playerType <<  std::endl;

        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                char val = ' ';
             

                if (this->board[i][j] == 1)
                    val = 'O';

                if (this->board[i][j] == -1)
                    val = 'X';

                std::cerr << " | " << val;


            }
            std::cerr << " |" << std::endl;
        }
    }

















};
