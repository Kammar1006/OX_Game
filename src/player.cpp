#include "player.h"
#include "board.h"
#include <iostream>

using namespace std;

Player::Player(char i_symbol, Board &abc) : b(abc) //set reference in constructor
{
    symbol = i_symbol;
}

void Player::turn(){
    int x=0, y=0;
    //if player write wrong cords must do again
    do{
        cout<<"Player "<<symbol<<" Turn: Enter x, y: "; cin>>x>>y;
    }while(!b.setSymbol(x, y, symbol));
}
