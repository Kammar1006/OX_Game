#include "board.h"
#include <iostream>
#include <vector>

Board::Board()
{
    newBoard();
}

void Board::newBoard()
{
    vect.clear();
    int baseSize = 9;
    std::vector<char> base(baseSize, '.');
    for(int i=0; i<baseSize; i++) vect.push_back(base);
    sx=0;
    sy=0;
    lastX=0;
    lastY=0;
    e_chX = 0;
    e_chY = 0;
}

void Board::show() const{
    //cords:
    std::cout<<"   ";
    for(unsigned int j=0; j<vect[0].size();j++) std::cout<<(j<10 ? "0" : "")<<j<<" ";
    std::cout<<std::endl;

    //show table
    for(unsigned int i=0; i<vect.size(); i++){
        //show number (cords):
        std::cout<<(i<10 ? "0" : "")<<i<<" ";
        //show table:
        for(unsigned int j=0; j<vect[i].size(); j++){
            std::cout<<" "<<vect[i][j]<<" ";
        }
        std::cout<<std::endl;
    }
    std::cout<<std::endl;
}

void Board::changeSize(int dx, int dy, bool before){

    int sizeY = vect.size();
    int sizeX = vect[0].size();
    char symbol='.';

    std::vector<char> baseX(sizeX+dx, symbol);
    std::vector<char> baseY(sizeY+dy, symbol);

    if(before){
        for(int i=0; i<dy; i++){
            vect.push_back(baseX);
        }
        for(int i=0;i<sizeY;i++){
            for(int j=0; j<dx; j++) vect[i].push_back(symbol);
        }
    }
    else{
        for(int i=0; i<dy; i++){

            /* vect.push_front(baseX): => vect.insert(vect.begin(), baseX); */
            vect.insert(vect.begin(), baseX);
        }
        for(int i=0;i<sizeY;i++){
            for(int j=0; j<dx; j++) vect[i+dy].insert(vect[i+dy].begin(), symbol);
        }
    }
}

bool Board::setSymbol(int x, int y, char symbol, bool graphicsModificator){

    //std::cout<<"SET"<<std::endl;

    if(graphicsModificator){
        x+=sx;
        y+=sy;
    }

    //sprawdzenie czy x i y sa w zakresie
    if(y>=(int)vect.size() || x>=(int)vect[0].size()) return false;

    //std::cout<<"SET2"<<std::endl;

    //sprawdzenie czy dokola pola wybranego znajduja sie znaki oraz ...



    //sprawdzenie czy nie znajduje sie znak na polu wybranym:
    if(vect[y][x]!='.' && vect[y][x]!=' '){
        return false;
    }

    //std::cout<<"SET3"<<std::endl;

    vect[y][x] = symbol;
    lastY = y;
    lastX = x;
    if(y==0){
        changeSize(0, 1, 0);
        lastY++;
        e_chY++;
    }
    if(y==(int)vect.size()-1) changeSize(0, 1, 1);
    if(x==0){
        changeSize(1, 0, 0);
        lastX++;
        e_chX++;
    }
    if(x==(int)vect[0].size()-1) changeSize(1, 0, 1);

    return true;
}

char Board::getSymbol(int x, int y, bool graphicsModificator) const{
    if(graphicsModificator) return vect[y+sy][x+sx];
    return vect[y][x];
}

int Board::getLastX() const {
    return lastX;
}

int Board::getLastY() const {
    return lastY;
}

void Board::changeSxSy(int a, int b){
    sx+=a;
    sy+=b;
    if(sx<0) sx=0;
    if(sy<0) sy=0;
    if(sx>(int)vect[0].size()-9) sx=vect[0].size()-9;
    if(sy>(int)vect.size()-9) sy=vect.size()-9;
}


Board::~Board()
{
    vect.clear();
}

