#include "game.h"
#include <iostream>

//Console Game Constructor:
Game::Game(Player &pp1, Player &pp2, Board &bb, char mode, int engineLevel) : p1(pp1), p2(pp2), b(bb)
{
    type = 'C'; //Console Interface
    gamemode = mode; //Mode: L - Local, E - Engine;
    if(gamemode=='E') Engine = new MyEngine(b, engineLevel); //Create Engine if EngineMode
}

//Graphics Game Constructor:
Game::Game(Player &pp1, Player &pp2, Board &bb, int argc, char* argv[]): p1(pp1), p2(pp2), b(bb){ // set obj player 1,2 and Engine (for reference);
    type = 'G'; //set type to Graphics
    view = "Start"; //set base window (Start Window)
    blinkedLast = false; //this is true when player or Engine draw symbol off the board;
    SDL_Init(SDL_INIT_EVERYTHING); //Initialize Graphics Version with SDL
    TTF_Init(); //Init TTF (text sub-libraries in SDL2);
    auto window = SDL_CreateWindow( //Create Window;
        "OX_GAME", //Set title
        SDL_WINDOWPOS_CENTERED, //Position
        SDL_WINDOWPOS_CENTERED,
        550, //Size
        550,
        0
    );
    renderer = SDL_CreateRenderer(window, -1, 0); //renderer - generate everything
    running_apk = true; //if application work
    running_game = false; //game work, is set window "Game" and player/Engine may do moves

    //Crete Base Color for blocks:
    //int colorWhite[3] = {255, 255, 255};
    int colorWhite2[3] = {192, 192, 192};
    int colorGray[3] = {64, 64, 64};
    int colorRed[3] = {192, 64, 64};
    int colorGreen[3] = {64, 192, 64};

    //Other color for text
    SDL_Color colorWhite={255, 255, 255};

    //Create All Button in game:
    //Navigate Button:
    ButtonTab[0] = new MyButton(*renderer, 50, 0, 450, 50, colorGray, 2, colorWhite2); //TopButton
    ButtonTab[1] = new MyButton(*renderer, 500, 50, 50, 450, colorGray, 2, colorWhite2); //RightButton
    ButtonTab[2] = new MyButton(*renderer, 50, 500, 450, 50, colorGray, 2, colorWhite2); //BottomButton
    ButtonTab[3] = new MyButton(*renderer, 0, 50, 50, 450, colorGray, 2, colorWhite2); //LeftButton
    //Button in Start View:
    ButtonTab[4] = new MyButton(*renderer, 50, 150, 450, 50, colorGreen, 2, colorWhite2); //NewGameButton
    ButtonTab[5] = new MyButton(*renderer, 50, 240, 450, 50, colorGreen, 2, colorWhite2); //Engine Button
    ButtonTab[6] = new MyButton(*renderer, 50, 330, 450, 50, colorGreen, 2, colorWhite2); //Multiplayer Button //soon
    ButtonTab[7] = new MyButton(*renderer, 50, 420, 450, 50, colorRed, 2, colorWhite2); //ExitButton, this button is used in Start and EngineLevel View;
    //Button in EngineLevel View;
    ButtonTab[8] = new MyButton(*renderer, 50, 150, 450, 50, colorGreen, 2, colorWhite2); //Engine Level 1 Button
    ButtonTab[9] = new MyButton(*renderer, 50, 240, 450, 50, colorGreen, 2, colorWhite2); //Engine Level 2 Button
    ButtonTab[10] = new MyButton(*renderer, 50, 330, 450, 50, colorGreen, 2, colorWhite2); //Engine Level 3 Button
    //Last Move Button:
    ButtonTab[11] = new MyButton(*renderer, 500, 500, 50, 50, colorGray, 2, colorWhite2); //Last Move Button

    //Define all Text in apk:
    //Used in Start View:
    Text[0] = new MyText(*renderer, "OX_Game", 30, 50, colorWhite);
    Text[1] = new MyText(*renderer, "2 player game", 30, 150, colorWhite);
    Text[2] = new MyText(*renderer, "Engine", 30, 240, colorWhite);
    Text[3] = new MyText(*renderer, "Soon", 30, 330, colorWhite);
    //Used in both View (Start and EngineLevel);
    Text[4] = new MyText(*renderer, "Exit", 30, 420, colorWhite);
    //Used in EngineLevel View
    Text[5] = new MyText(*renderer, "Easy (defense only) (1/3d)", 30, 150, colorWhite);
    Text[6] = new MyText(*renderer, "Medium 1 (1/3d+2/3o)", 30, 240, colorWhite);
    Text[7] = new MyText(*renderer, "Medium 2 (2/3d+2/3o)", 30, 330, colorWhite);
    Text[8] = new MyText(*renderer, "OX_Game - Engine", 30, 50, colorWhite);
    //Last Move Text:
    Text[9] = new MyText(*renderer, "Last ", 22, 510, colorWhite, 'r');

    whoTurn = 0; //0-'O', 1-'X' //if that variable is 0 turn do player 'O' otherwise turn have player 'X';

    //That variables are used to generate winning line if game over;
    minX=1000;
    minY=1000;

    //Crete Tiles (Board);
    //That loop define 81 (9x9) 'graphics' board:
    for(int i=0; i<9; i++){
        for(int j=0; j<9; j++){
            Tile[9*i+j] = new MyRect(*renderer, 50+50*i, 50+50*j, 50, 50, colorGray, 2, colorWhite2);
        }
    }

    //Mouse cords:
    //If that cords are not used their value is '-1'; (pixel)
    xMouse=-1; yMouse=-1;

    //this is Mouse Cords in 'block' scale OR/AND if game over that cord is used to generate second end of inning line;
    posX=-1; posY=-1;
}

//Game Destructor - destroy all graphic objects define using pointer or in console mode delete engine (in Engine Mode);
Game::~Game()
{
    if(type=='G'){
        //delete renderer;
        SDL_DestroyRenderer(renderer);

        //delete tiles:
        for(int i=0; i<81; i++){
            delete Tile[i];
        }
        delete Tile;

        //delete buttons:
        for(int i=0; i<12; i++){
            delete ButtonTab[i];
        }
        delete ButtonTab;

        //delete text:
        for(int i=0; i<10; i++){
            delete Text[i];
        }
        delete Text;

        std::cout<<minX<<" "<<minY<<" "<<posX<<" "<<posY<<std::endl;
    }
    if(type=='C' && gamemode=='E'){//console interface and Engine Mode;
        //Delete Engine:
        delete Engine;
    }
}

//loop Game for graphics interface:
bool Game::loopG(){
    //char array:
    char tabH[2]={'O', 'X'};
    //event Listener
    while(SDL_PollEvent(&e)){
        //Quit event listener is activated if player click close button 'X';
        if(e.type==SDL_QUIT) running_apk = false;

        //Event is activated if player click mouse button in game screen;
        if(e.type == SDL_MOUSEBUTTONDOWN)
        {
            //return Mouse Cords:
            SDL_GetMouseState(&xMouse,&yMouse);
            //std::cout<<xMouse<<" "<<yMouse<<std::endl;
            if(xMouse>=0){
                if(view=="Start"){
                    whoTurn=0;
                    std::cout<<view<<std::endl;
                    //In Start View if player clicked: 2 Player Mode:
                    if(ButtonTab[4]->isOnClick(xMouse, yMouse)){
                        view="Game";
                        running_game = true;
                        gamemode = 'L'; //set game mode to Local (2 Player)
                        b.newBoard(); //generate new board is used if players play second or next game;
                        for(int i=0; i<81; i++) Tile[i]->rh_setPos(' '); //set all Tile to null symbol (other symbol is 'O' and 'X');

                    }
                    //If player choose Engine Mode:
                    if(ButtonTab[5]->isOnClick(xMouse, yMouse)){
                        view="EngineLevel";
                    }
                    //If ....
                    if(ButtonTab[6]->isOnClick(xMouse, yMouse)){ //xMouse>50 && xMouse<500 && yMouse>330 && yMouse<380
                        std::cout<<"Maybe Soon"<<std::endl;
                        if(false){
                            view="Game";
                            running_game = true;
                            gamemode = 'N';
                            b.newBoard();
                            for(int i=0; i<81; i++) Tile[i]->rh_setPos(' ');
                        }
                    }
                    //If player choose exit:
                    else if(ButtonTab[7]->isOnClick(xMouse, yMouse)){
                        std::cout<<view<<std::endl;
                        running_apk = false;
                    }
                }
                //if EngineLevel View is active player choose level:
                else if(view=="EngineLevel"){
                    if(ButtonTab[8]->isOnClick(xMouse, yMouse)){
                        view="Game";
                        running_game = true;
                        gamemode = 'E';
                        b.newBoard();
                        Engine = new MyEngine(b, 1); //Create Engine (First Level only defense);
                        for(int i=0; i<81; i++) Tile[i]->rh_setPos(' ');
                    }
                    else if(ButtonTab[9]->isOnClick(xMouse, yMouse)){
                        view="Game";
                        running_game = true;
                        gamemode = 'E';
                        b.newBoard();
                        Engine = new MyEngine(b, 2); //Create Engine (Second Level - medium 1);
                        for(int i=0; i<81; i++) Tile[i]->rh_setPos(' ');
                    }
                    else if(ButtonTab[10]->isOnClick(xMouse, yMouse)){
                        view="Game";
                        running_game = true;
                        gamemode = 'E';
                        b.newBoard();
                        Engine = new MyEngine(b, 3); //Create Engine (Third Level - medium 2);
                        for(int i=0; i<81; i++) Tile[i]->rh_setPos(' ');
                    }
                    else if(ButtonTab[7]->isOnClick(xMouse, yMouse)){ //else if player click exit:
                        std::cout<<view<<std::endl;
                        running_apk = false;
                    }
                }
                else if(view=="Game"){ //If game is active player may choose tile where may set symbol or may change board position using navigation button;
                    if(xMouse>50 && xMouse<500 && yMouse>50 && yMouse<500){ //If player clicked in MainGame (Tiles):
                        posX = (xMouse-50)/50; //Change Mouse Cords from pixel to index of tiles:
                        posY = (yMouse-50)/50;
                        if(gamemode=='L' || (gamemode=='E' && whoTurn==0)){ //For Local and Engine Mode:
                            if(b.setSymbol(posX, posY, tabH[whoTurn], true)){ //if that condition is true -> that tile is free and player may draw/set symbol:
                                std::cout <<"\033[2J\033[1;1H"<<std::endl; //console clear function for windows and Linux system:
                                b.show(); //show board (console, for logs):
                                blinkedLast = false;
                                if(isEnd()){ //that function checked status of game (end or running):
                                    std::cout<<"Player "<<tabH[whoTurn]<<" won the game!"<<std::endl;
                                    running_game = false;
                                }
                                whoTurn=1-whoTurn; //change turn flag to otherwise value:

                                //In actual version of game engine play always 'X' symbol so that that method work and condition in 208 line is true in engine mode:
                                //engine move:
                                if(gamemode=='E' && whoTurn==1 && running_game){
                                    Engine->turn(); //Active engine turn:
                                    if(isEnd()){
                                        std::cout <<"\033[2J\033[1;1H"<<std::endl; //console clear function for windows and Linux system:
                                        b.show(); //show board (console, for logs):
                                        std::cout<<"Last Cords: "<<b.getLastX()<<" "<<b.getLastY()<<std::endl;
                                        std::cout<<"Comp ("<<tabH[whoTurn]<<") won the game!"<<std::endl;
                                        running_game = false;
                                    }
                                    whoTurn=1-whoTurn;
                                }
                            }
                        }
                    }//Those if are activated if player clicked navigation button
                    if(ButtonTab[3]->isOnClick(xMouse, yMouse)){
                        //Left Button;
                        b.changeSxSy(-1, 0);
                    }
                    if(ButtonTab[1]->isOnClick(xMouse, yMouse)){
                        //Right Button;
                        b.changeSxSy(1, 0);
                    }
                    if(ButtonTab[0]->isOnClick(xMouse, yMouse)){
                        //Top Button;
                        b.changeSxSy(0, -1);
                    }
                    if(ButtonTab[2]->isOnClick(xMouse, yMouse)){
                        //Bottom Button;
                        b.changeSxSy(0, 1);
                    }
                    if(ButtonTab[11]->isOnClick(xMouse, yMouse)){
                        //Show Last Move:
                        blinkedLast = true;
                    }

                    //if player clicked navigation button 'graphics' board must be change...
                    for(int i=0; i<9; i++){
                        for(int j=0; j<9; j++){
                            Tile[9*i+j]->rh_setPos(b.getSymbol(i, j, true));
                        }
                    }
                }
                //set Mouse Cord to basic value:
                xMouse=-1;
                yMouse=-1;
            }
        }
    }


    //Draw all objects:
    //clear board
    SDL_SetRenderDrawColor(renderer, 32, 32, 32, 255);
    SDL_RenderClear(renderer);

    //In Game View draw 4 Navigation buttons and tiles:
    if(view=="Game"){
        //Draw Nav Button
        for(int i=0; i<4; i++) ButtonTab[i]->draw();

        //Draw Last Move Button and text:
        ButtonTab[11]->draw();
        Text[9]->draw();


        //Draw Tiles:
        for(int i=0; i<81; i++){
            Tile[i]->draw();
        }

        //Reset Navigation Button status
        for(int i=0; i<4; i++) ButtonTab[i]->changeBlinkStatus(false);

        //set navigation button status:
        //if Engine or 2nd Player (soon) draw symbol without 9x9 board blinked navigation button
        int blinkedX = b.getLastX()-b.getSx();
        int blinkedY = b.getLastY()-b.getSy();

        //if navigation button is/are blinked and player clicked that button to change sx-sy
        //and last cords is in the board blinked symbol;
        if(blinkedX<0){
            ButtonTab[3]->changeBlinkStatus(true);
            blinkedLast = true;
        }
        else if(blinkedX>=9){
            ButtonTab[1]->changeBlinkStatus(true);
            blinkedLast = true;
        }

        if(blinkedY<0){
            ButtonTab[0]->changeBlinkStatus(true);
            blinkedLast = true;
        }
        else if(blinkedY>=9){
            ButtonTab[2]->changeBlinkStatus(true);
            blinkedLast = true;
        }


        int blinkedId = 9*(b.getLastX()-b.getSx())+b.getLastY()-b.getSy();
        //std::cout<<blinkedId<<std::endl;
        if(blinkedX>=0 && blinkedX<9 && blinkedY>=0 && blinkedY<9 && blinkedLast){
            Tile[blinkedId]->changeBlinkStatus(true);
            Tile[blinkedId]->draw();
            Tile[blinkedId]->changeBlinkStatus(false);

        }
    }//In Start View draw Buttons and Text:
    else if(view=="Start"){
        ButtonTab[4]->draw(); //NewGame
        ButtonTab[5]->draw(); //Engine
        ButtonTab[6]->draw(); //Multi
        ButtonTab[7]->draw(); //Exit

        Text[0]->draw(); //OX_game
        Text[1]->draw(); //NewGame
        Text[2]->draw(); //Engine
        Text[3]->draw(); //Multi
        Text[4]->draw(); //Exit
    }
    else if(view=="EngineLevel"){
        ButtonTab[8]->draw(); //Engine Level 1
        ButtonTab[9]->draw(); //Engine Level 2
        ButtonTab[10]->draw(); //Engine Level 2
        ButtonTab[7]->draw(); //Exit

        Text[4]->draw();//Exit
        Text[5]->draw();//Easy (defense only)
        Text[6]->draw();//Medium 1 (1/3d+2/3o)
        Text[7]->draw();//Medium 2 (2/3d+2/3o)
        Text[8]->draw();//OX_Game - Engine
    }

    //if game over change view status and draw end winning-line;
    if(!running_game && view=="Game" && isEnd()){ //endgame
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        //get sx, sy in board obj; using in translation logical nxm tab to 9x9 graphical tab;
        view="New";
        int a[2]={0, 0};
        a[0]=b.getSx();//get Sx and Sy board value:
        a[1]=b.getSy();
        SDL_RenderDrawLine(renderer, 50*(1+minX-a[0])+25, 50*(1+minY-a[1])+25, 50*(1+posX-a[0])+25, 50*(1+posY-a[1])+25);
        if(gamemode=='E') delete Engine;//Delete Engine;
    }
    //That function show all object in renderer (draw all element);
    SDL_RenderPresent(renderer);

    //If game is end and view is 'New' (logical view used to delete object, and delay(wait));
    if(view=="New"){
        SDL_Delay(1900); //wait ~2s
        view="Start";
    }
    SDL_Delay(100);
    if(running_apk) return true; //if apk is run continue loop game;
    return false;//otherwise break loop game and close apk;
}

//loop game for console version:
bool Game::loop(){
    std::cout <<"\033[2J\033[1;1H"<<std::endl; //clear console;
    b.show(); //show board;
    p1.turn();
    int st = isEnd(); //get game status;
    //if game over show board and information who won;
    if(st){
        b.show();
        if(st==1) std::cout<<"Player 'O' won the game!"<<std::endl;
        if(st==2) std::cout<<"Player 'X' won the game!"<<std::endl;
        //if endgame break loop game and close apk;
        return false;
    }

    std::cout <<"\033[2J\033[1;1H"<<std::endl;
    b.show();
    //If engine mode -> Engine turn otherwise second player turn:
    if(gamemode=='E') Engine->turn();
    else p2.turn();


    st = isEnd();
    if(st){
        b.show();
        if(st==1) std::cout<<"Player 'O' won the game!"<<std::endl;
        if(st==2) std::cout<<"Player 'X' won the game!"<<std::endl;
        return false;
    }

    //continue loop game:
    return true;
}

//That method return status of game, if game is end return true (1, 2 - which player won) or if game run return false (0):
int Game::isEnd(){
    int lastX = b.getLastX(); //get last move index X and Y:
    int lastY = b.getLastY();
    int maxX = b.getMaxX(); //get actual board size:
    int maxY = b.getMaxY();
    int NoSymbol = 1; //Number of symbol in row/column/diagonal
    char baseSymbol = b.getSymbol(lastX, lastY); //check which symbol must to calculate
    int i=0, j=0; //set helper index:

    //var to draw win line:
    minX = lastX;
    minY = lastY;
    posX = lastX;
    posY = lastY;

    //direction: W-E:
    if(lastX>0){
        for(int i=lastX-1; i>=0; i--){
            if(baseSymbol==b.getSymbol(i, lastY)){
                NoSymbol++;
                minX = i;
            }
            else break;
        }
    }
    if(lastX<maxX-1){
        for(int i=lastX+1; i<maxX; i++){
            if(baseSymbol==b.getSymbol(i, lastY)){
                NoSymbol++;
                posX = i;
            }
            else break;
        }
    }

    if(NoSymbol>=5){
        if(baseSymbol=='O') return 1;
        if(baseSymbol=='X') return 2;
    }

    //direction: N-S:
    NoSymbol=1;
    minX = lastX;
    minY = lastY;
    posX = lastX;
    posY = lastY;
    if(lastY>0){
        for(i=lastY-1; i>=0; i--){
            if(baseSymbol==b.getSymbol(lastX, i)){
                NoSymbol++;
                minY=i;
            }
            else break;
        }
    }
    if(lastY<maxY-1){
        for(i=lastY+1; i<maxY; i++){
            if(baseSymbol==b.getSymbol(lastX, i)){
                NoSymbol++;
                posY = i;
            }
            else break;
        }
    }

    if(NoSymbol>=5){
        if(baseSymbol=='O') return 1;
        if(baseSymbol=='X') return 2;
    }

    //direction: NW-SE:
    NoSymbol=1;
    minX = lastX;
    minY = lastY;
    posX = lastX;
    posY = lastY;
    if(lastY>0 && lastX>0){
        i=lastX-1;
        j=lastY-1;
        while(i>=0 && j>=0){
            if(baseSymbol==b.getSymbol(i, j)) NoSymbol++;
            else break;
            //std::cout<<NoSymbol<<std::endl;
            i--; j--;
            minX--; minY--;
        }
    }
    if(lastX<maxX-1 && lastY<maxY-1){
        i=lastX+1;
        j=lastY+1;
        while(i<maxX && j<maxY){
            if(baseSymbol==b.getSymbol(i, j)) NoSymbol++;
            else break;
            //std::cout<<NoSymbol<<std::endl;
            i++; j++;
            posX++; posY++;
        }
    }

    if(NoSymbol>=5){
        if(baseSymbol=='O') return 1;
        if(baseSymbol=='X') return 2;
    }


    //direction: NE-SW:
    NoSymbol=1;
    minX=lastX;
    minY=lastY;
    posX = lastX;
    posY = lastY;
    if(lastY>0 && lastX<maxX-1){
        i=lastX+1;
        j=lastY-1;
        while(i<maxX && j>=0){
            if(baseSymbol==b.getSymbol(i, j)) NoSymbol++;
            else break;
            i++; j--;
            minY--;
            minX++;
        }
    }
    if(lastX>0 && lastY<maxY-1){
        i=lastX-1;
        j=lastY+1;
        while(i>=0 && j<maxY){
            if(baseSymbol==b.getSymbol(i, j)) NoSymbol++;
            else break;
            //std::cout<<NoSymbol<<std::endl;
            i--; j++;
            posX--; posY++;
        }
    }

    if(NoSymbol>=5){
        if(baseSymbol=='O') return 1;
        if(baseSymbol=='X') return 2;
    }

    return 0;
}
