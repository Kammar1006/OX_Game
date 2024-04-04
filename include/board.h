#ifndef OX_GAME_BOARD_H
#define OX_GAME_BOARD_H

#include <iostream>
#include <vector>


class Board
{
    std::vector< std::vector<char> > vect; // 2D m*n vector [y, x];
    int lastX, lastY; //last cords, this is require to isEnd function and engine
    int e_chX, e_chY; //last cords, this is require to engine
    int sx, sy; //graphical transformation - n*m logical board to 9x9 graphical board
    public:
        Board();
        void newBoard();
        void show() const;
        void changeSize(int dx, int dy, bool before = true);
        bool setSymbol(int x, int y, char symbol, bool graphicsModificator = false); //symbol: O or X;
        char getSymbol(int x, int y, bool graphicsModificator = false) const;
        int getLastX() const;
        int getMaxX() const {return vect[0].size();};
        int getMaxY() const {return vect.size();};
        int getLastY() const;
        void changeSxSy(int a, int b);
        int getSx() const { return sx; };
        int getSy() const { return sy; };
        int getEx() const { return e_chX; };
        int getEy() const { return e_chY; };
        ~Board();
};

#endif // OX_GAME_BOARD_H
