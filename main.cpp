#include "board.h"
#include "player.h"
#include "game.h"
#include <SDL2/SDL.h>
#include <vector>
#include "myrect.h"
#include <SDL2/SDL_ttf.h>

/* Logs  */
///  16.03.2023  //
/**
    [START] Create first object "Board";
    [ADD]   Board Class;
    [ADD]   Add Change Board Size Function V1;
*/
///  17.03.2023   //
/**
    [FIX]   Change "Change Board Function" (V2);
    [ADD]   Player, Game Classes;
    [LOG]   Console Game is End;
    [ADD]   Add Base Window
*/
///  18.03.2023  //
/**
    [ADD] Add Graphics Tile v1;
*/
///  19.03.2023  //
/**
    [ADD] Mouse Cords
    [ADD] 'O' and 'X' symbol in graphical version
*/
///  20.03.2023  //
/**
    [FIX] Graphics Tile v2
    [FIX] Game Class
    [ADD] Connection between console and graphics version
    [ADD] Transformation Logical MxN Console Board to 9x9 Window Board
    [ADD] Change sx, sy to move 9x9 board;
*/
///  09.05.2023  //
/**
    [FIX] WinLine (using board transformation)
    [ADD] StartView & GameView;
    [ADD] NewGame Opt when player won the game;
*/
///  10.05.2023  //
/**
    [ADD] New Text Library SDL_TTF
    [ADD] New Class MyText
    [ADD] Text to Game
*/
///  11.05.2023  //
/**
    [CRE] Create Engine Concept
    [ADD] Class MyEngine, struct MoveCordsData
    [ADD] More Text and Block (for Engine and (soon) Multi)
    [FIX] Change LoopGame for EngineMode
*/
///  12.05.2023  //
/**
    [ADD] Base Engine MapFunction
    [ADD] Base Engine Defense Algorithm (~1/2)
    [FIX] Few Bugs
    [ADD] Base Engine to Console Version
*/
///  13.05.2023  //
/**
    [ADD] Base Engine Attack Algorithm (~1/3)
    [ADD] Base Decision System (~)
    [FIX] Bags

*/
///  14.05.2023  //
/**
    [ADD] Engine Move History
    [ADD] Find the best chance for attack in all history.
    [FIX] Base Engine Attack Algorithm (~2/3)
*/
///  16.05.2023  //
/**
    [ADD] New Class MyButton (Inheritance MyRect)
    [FIX] All compiler warning
*/

///  18.05.2023  //
/**
    [ADD] New Platform Kali-Linux
*/
///  21.05.2023  //
/**
    [FIX] Add New Platform Linux: Ubuntu, Kali, Debian [v3]
    [ADD] New View "EngineLevel"
    [ADD] History of Player Move
    [ADD] Engine Level:{
        [1] Defense Only (Last player move)
        [2] Offense with engine history move + base def
        [3] Offense and Defense with history
    }
*/
///  25.05.2023  //
/**
    [FIX] Bugs and Warnings
*/

///  28.05.2023  //
/**
    [FIX] Change system("cls||clear") to std::cout <<"\033[2J\033[1;1H"<<std::endl;
    [ADD] Blinked Button and Rectangle:
        If Engine set symbol off the 9x9 board blinked navigation button
        If player change sx, sy (nav button) blinked 'O' or 'X' symbol
*/

///  29.05.2023  //
/**
    [ADD] Show Last Move Button
*/


/// To Do  //
/**
    [TODO]  Console Game OX Infinity    [T:xx.03.23][V:17.03.23];
    [TODO]  Add Window Apk Opt v1       [T:xx.03.23][V:20.03.23 - beta];
    [TODO]  Add Window Apk Opt v2       [T:17.05.23][V:10.05.23];
    {
        [V] 2 Window: StartView & GameView
        [V] NewGame Opt. (when player win)
        [V] Add Text: OX_Game, NewGame, Exit;
    }
    [TODO]  Add Engine Opt              [T:xx.05.23][V:21.05.2023];
    [TODO]  Add Multiplayer Opt         [T:xx.xx.23][V:];
*/


int main(int argc, char* argv[])
{
    int n;
    Board b;
    Player p1{'O', b}, p2{'X', b};

    //better clear console
    //using system("cls||clear") is dangerous...
    //but in windows system "escape sequence" must be init by system(" ");
    system(" ");
    //clear console:
    std::cout <<"\033[2J\033[1;1H"<<std::endl;


    //Choose interface (Console OR Graphics);
    std::cout<<" --- OX Game --- "<<std::endl;
    std::cout<<" 1. Console Version: "<<std::endl;
    std::cout<<" 2. Graphics Version: "<<std::endl;
    std::cout<<">> "; std::cin>>n;
    if(n==1){
        //In Console Interface - Choose 2 player OR Engine Mode (Soon Maybe...)
        std::cout <<"\033[2J\033[1;1H"<<std::endl;
        std::cout<<" --- Choose GameMode --- "<<std::endl;
        std::cout<<" 1. 2 Player Version: "<<std::endl;
        std::cout<<" 2. vs Engine Version: "<<std::endl;
        std::cout<<">> "; std::cin>>n;
        if(n==1){
            Game g{p1, p2, b};
            while(g.loop());
            return 0;
        }
        if(n==2){
            //Choose Engine Level:
            std::cout <<"\033[2J\033[1;1H"<<std::endl;
            std::cout<<" --- Choose EngineLevel --- "<<std::endl;
            std::cout<<" 1. Easy (only defense): "<<std::endl;
            std::cout<<" 2. Medium1 (1/3d+2/3o): "<<std::endl;
            std::cout<<" 3. Medium2 (2/3d+2/3o):"<<std::endl;
            std::cout<<">> "; std::cin>>n;
            if(n==1 || n==2 || n==3){
                Game g{p1, p2, b, 'E', n};
                while(g.loop());
                return 0;
            }
        }
    }
    else if(n==2){
        //Graphics Interface Initialize:
        Game g{p1, p2, b, argc, argv};
        std::cout <<"\033[2J\033[1;1H"<<std::endl;
        b.show();
        while(g.loopG());
    }
    return 0;
}
