#ifndef OX_GAME_GAME_H
#define OX_GAME_GAME_H

#include "board.h"
#include "player.h"
#include "myrect.h"
#include "my_text.h"
#include "my_engine.h"
#include "my_button.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

class Game
{
    //console version:
    private:
        Player &p1, &p2;
        Board &b;
    public:
        Game(Player &pp1, Player &pp2, Board &bb, char mode='L', int engineLevel = 1);
        bool loop();
        int isEnd();

    //graphics version:
    private:
        char type; //'C' OR 'G' //console or graphics
        char gamemode; //in graphic interface 'L' - local game 2 player, 'N' - Networking (Soon), 'E' - Engine
        //if graphic apk is open
        bool running_apk;
        //if game is running - true, if player won the game - false;
        bool running_game;
        std::string view; //{"Start", "Game"}
        SDL_Event e;
        MyRect** Tile = new MyRect* [81]; //graphical tab1D is showing as tab2D
        MyButton** ButtonTab = new MyButton* [12]; //Navigation Button using to change board var sx, sy and newgame button and exitbutton;
        MyText** Text = new MyText* [10]; //All text using in that project
        int xMouse, yMouse; //running mouse position [px]
        int posX, posY; //running mouse position [tiles] or in endgame is use to draw win line
        int whoTurn; // 'O'=0 or 'X'=1;
        SDL_Renderer *renderer;
        bool blinkedLast;

        //require to draw winning line
        int minX, minY;

        //engine:
        MyEngine* Engine;

    public:
        Game(Player &pp1, Player &pp2, Board &bb, int argc, char* argv[]);
        bool loopG();
        ~Game();

};

#endif // OX_GAME_GAME_H
