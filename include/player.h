#ifndef OX_GAME_PLAYER_H
#define OX_GAME_PLAYER_H

#include "board.h"

//This Class is used only in console game interface, in graphics interface that class is replaced by mouse event listener;
class Player
{
    public:
        Player(char i_symbol, Board &abc); //reference board and player symbol ('O' or 'X');
        void turn();

    private:
        char symbol; // 'O' or 'X' symbol
        Board &b; //reference must be set initially, but maybe set in constructor in alternative method;


};

#endif // OX_GAME_PLAYER_H
