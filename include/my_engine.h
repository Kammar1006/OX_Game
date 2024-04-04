#ifndef OX_GAME_MYENGINE_H
#define OX_GAME_MYENGINE_H

#include "board.h"
#include <vector>

struct MoveCordsData{
    int noCompSymbol;
    int noPlayerSymbol;
    int dangerousLevel;
    int xLimit[2];
    int yLimit[2];
};

struct v2{
    int x;
    int y;
};

struct arrayEngineMoveDecision{
    MoveCordsData tab[4];
};

class MyEngine
{
    public:
        MyEngine(Board &bb, int i_level=2);
        void turn();
        ~MyEngine();

    private:
        Board &b;
        std::vector<arrayEngineMoveDecision> engineArrayHistory;
        std::vector<arrayEngineMoveDecision> playerArrayHistory;
        std::vector<v2> engineCordsHistory;
        std::vector<v2> playerCordsHistory;
        int p_lastX;
        int p_lastY;
        int e_lastX;
        int e_lastY;
        int e_ch[2];
        int level;
        int moveNumber;
        void mapBoard(MoveCordsData *tab, int lastX, int lastY);
};

#endif // OX_GAME_MYENGINE_H
