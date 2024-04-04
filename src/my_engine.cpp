#include "my_engine.h"
#include "board.h"
#include <vector>

MyEngine::MyEngine(Board &bb, int i_level) : b(bb)
{
    moveNumber=0;
    e_ch[0]=0;
    e_ch[1]=0;
    level = i_level;
}

MyEngine::~MyEngine()
{
    engineCordsHistory.clear();
    engineArrayHistory.clear();
    playerCordsHistory.clear();
    playerArrayHistory.clear();
}

void MyEngine::turn(){
    moveNumber++;
    if(moveNumber==1){
        p_lastX = b.getLastX();
        p_lastY = b.getLastY();
        e_lastX = p_lastX+1;
        e_lastY = p_lastY+1;
        engineCordsHistory.push_back(v2{e_lastX, e_lastY});
        playerCordsHistory.push_back(v2{p_lastX, p_lastY});
        b.setSymbol(e_lastX, e_lastY, 'X');
    }
    else if(level==1){
        std::cout<<"Engine Move: "<<moveNumber<<"\n";
        MoveCordsData tab[4];
        p_lastX = b.getLastX();
        p_lastY = b.getLastY();
        mapBoard(tab, p_lastX, p_lastY);
        //version def v1/2
        int max_i=0;
        int max_danger=0;
        for(int i=0; i<4; i++){
            if(tab[i].dangerousLevel>max_danger){
                max_danger = tab[i].dangerousLevel;
                max_i = i;
            }
        }
        std::cout<<"Direction: "<<max_i<<std::endl;
        char baseSymbol = b.getSymbol(p_lastX, p_lastY);
        char compSymbol = ((baseSymbol=='O') ? 'X' : 'O');
        if(compSymbol!=b.getSymbol(tab[max_i].xLimit[0], tab[max_i].yLimit[0])){
            e_lastX = tab[max_i].xLimit[0];
            e_lastY = tab[max_i].yLimit[0];
        }
        else{
            e_lastX = tab[max_i].xLimit[1];
            e_lastY = tab[max_i].yLimit[1];
        }
        std::cout<<"Comp Calc Move: "<<e_lastX<<" "<<e_lastY<<std::endl;
        b.setSymbol(e_lastX, e_lastY, compSymbol);
    }
    else if(level==2){
        //flag only defense
        bool defense = false;

        std::cout<<"Engine Move: "<<moveNumber<<"\n";
        //repeat for all move in this game
        for(unsigned int i=0; i<engineCordsHistory.size();i++){
            //if player turn is end and player chose (0,a) or (a, 0) cords and board size is change: that solution move e_lastX, e_lastY
            if(e_ch[0]!=b.getEx()){
                engineCordsHistory[i].x+=b.getEx()-e_ch[0];
            }
            if(e_ch[1]!=b.getEy()){
                engineCordsHistory[i].y+=b.getEy()-e_ch[1];
            }
            if(i==engineCordsHistory.size()-1){
                e_lastX = engineCordsHistory[i].x;
                e_lastY = engineCordsHistory[i].y;
            }
            //std::cout<<i+1<<". "<<engineCordsHistory[i].x<<" "<<engineCordsHistory[i].y<<"\n";
        }
        e_ch[0]=b.getEx();
        e_ch[1]=b.getEy();
        //std::cout<<"\n\n";

        MoveCordsData tab_def[4];
        MoveCordsData tab_att[4];

        p_lastX = b.getLastX();
        p_lastY = b.getLastY();
        mapBoard(tab_def, p_lastX, p_lastY);
        mapBoard(tab_att, e_lastX, e_lastY);

        //create history of move and add last move in this history
        arrayEngineMoveDecision helperTab;
        helperTab.tab[0] = tab_att[0];
        helperTab.tab[1] = tab_att[1];
        helperTab.tab[2] = tab_att[2];
        helperTab.tab[3] = tab_att[3];
        engineArrayHistory.push_back(helperTab);
        //std::cout<<"Nano: "<<engineCordsHistory.size()<<" "<<engineArrayHistory.size()<<"\n\n";

        //back into history and calc all engineArrayHistories
        for(int j=engineArrayHistory.size()-2; j>=0; j--){
            mapBoard(engineArrayHistory[j].tab, engineCordsHistory[j].x, engineCordsHistory[j].y);
        }

        //search maximum chance move
        //version def v1/3+1/3
        int max_i_def=0;
        int max_i_att=0;
        int max_j_att=0;
        int max_danger=0;
        int max_chance=0;
        //calculate danger level for defense
        for(int i=0; i<4; i++){
            if(tab_def[i].dangerousLevel>max_danger){
                max_danger = tab_def[i].dangerousLevel;
                max_i_def = i;
            }
        }
        //find the best chance
        for(int j=engineArrayHistory.size()-1; j>=0; j--){
            for(int i=0; i<4; i++){
                if(engineArrayHistory[j].tab[i].dangerousLevel>max_chance){
                    max_chance = engineArrayHistory[j].tab[i].dangerousLevel;
                    max_i_att = i;
                    max_j_att = j;
                }
            }
        }
        char baseSymbol = b.getSymbol(p_lastX, p_lastY);
        char compSymbol = ((baseSymbol=='O') ? 'X' : 'O');
        bool instantWin = false;
        std::cout<<max_i_def<<" "<<max_i_att<<"\n";
        if(engineArrayHistory[max_j_att].tab[max_i_att].noPlayerSymbol>=4 && engineArrayHistory[max_j_att].tab[max_i_att].noCompSymbol!=2){
            instantWin=true;
            std::cout<<"Instant win!!!\n";
        }
        if(((max_danger>3 || max_danger>max_chance) && !instantWin) || defense){
            std::cout<<"Direction: "<<max_i_def<<std::endl;
            if(compSymbol!=b.getSymbol(tab_def[max_i_def].xLimit[0], tab_def[max_i_def].yLimit[0])){
                e_lastX = tab_def[max_i_def].xLimit[0];
                e_lastY = tab_def[max_i_def].yLimit[0];
            }
            else{
                e_lastX = tab_def[max_i_def].xLimit[1];
                e_lastY = tab_def[max_i_def].yLimit[1];
            }
        }
        else{
            std::cout<<"Direction: "<<max_i_att<<std::endl;
            if(baseSymbol!=b.getSymbol(engineArrayHistory[max_j_att].tab[max_i_att].xLimit[0], engineArrayHistory[max_j_att].tab[max_i_att].yLimit[0])){
                e_lastX = engineArrayHistory[max_j_att].tab[max_i_att].xLimit[0];
                e_lastY = engineArrayHistory[max_j_att].tab[max_i_att].yLimit[0];
            }
            else{
                e_lastX = engineArrayHistory[max_j_att].tab[max_i_att].xLimit[1];
                e_lastY = engineArrayHistory[max_j_att].tab[max_i_att].yLimit[1];
            }
        }
        std::cout<<"Comp Calc Move: "<<e_lastX<<" "<<e_lastY<<std::endl;
        v2 a;
        a.x = e_lastX;
        a.y = e_lastY;
        engineCordsHistory.push_back(a);
        b.setSymbol(e_lastX, e_lastY, compSymbol);
    }
    else if(level==3){
        //flag only defense
        bool defense = false;

        std::cout<<"Engine Move: "<<moveNumber<<"\n";
        //repeat for all move in this game
        for(unsigned int i=0; i<engineCordsHistory.size();i++){
            //if player turn is end and player chose (0,a) or (a, 0) cords and board size is change: that solution move e_lastX, e_lastY
            if(e_ch[0]!=b.getEx()){
                engineCordsHistory[i].x+=b.getEx()-e_ch[0];
                playerCordsHistory[i].x+=b.getEx()-e_ch[0];
            }
            if(e_ch[1]!=b.getEy()){
                engineCordsHistory[i].y+=b.getEy()-e_ch[1];
                playerCordsHistory[i].y+=b.getEy()-e_ch[1];
            }
            if(i==engineCordsHistory.size()-1){
                e_lastX = engineCordsHistory[i].x;
                e_lastY = engineCordsHistory[i].y;
            }
            //std::cout<<i+1<<". "<<engineCordsHistory[i].x<<" "<<engineCordsHistory[i].y<<"\n";
        }
        e_ch[0]=b.getEx();
        e_ch[1]=b.getEy();
        //std::cout<<"\n\n";

        MoveCordsData tab_def[4];
        MoveCordsData tab_att[4];

        p_lastX = b.getLastX();
        p_lastY = b.getLastY();
        playerCordsHistory.push_back(v2{p_lastX, p_lastY});
        /*for(unsigned int i=0; i<playerCordsHistory.size();i++){
            std::cout<<i<<" "<<playerCordsHistory[i].x<<" "<<playerCordsHistory[i].y<<"\n";
        }*/

        if(moveNumber==2){
            mapBoard(tab_def, playerCordsHistory[0].x, playerCordsHistory[0].y);
            arrayEngineMoveDecision helperTab;
            helperTab.tab[0] = tab_def[0];
            helperTab.tab[1] = tab_def[1];
            helperTab.tab[2] = tab_def[2];
            helperTab.tab[3] = tab_def[3];
            playerArrayHistory.push_back(helperTab);
        }

        mapBoard(tab_def, p_lastX, p_lastY);
        mapBoard(tab_att, e_lastX, e_lastY);

        //create history of move and add last move in this history for engine
        arrayEngineMoveDecision helperTab;
        helperTab.tab[0] = tab_att[0];
        helperTab.tab[1] = tab_att[1];
        helperTab.tab[2] = tab_att[2];
        helperTab.tab[3] = tab_att[3];
        engineArrayHistory.push_back(helperTab);

        //create history of move and add last move in this history for player to defense
        helperTab.tab[0] = tab_def[0];
        helperTab.tab[1] = tab_def[1];
        helperTab.tab[2] = tab_def[2];
        helperTab.tab[3] = tab_def[3];
        playerArrayHistory.push_back(helperTab);

        //back into history and calc all engineArrayHistories for offense
        for(int j=engineArrayHistory.size()-2; j>=0; j--){
            mapBoard(engineArrayHistory[j].tab, engineCordsHistory[j].x, engineCordsHistory[j].y);
        }

        //back into history and calc all playerArrayHistory for defense
        for(int j=playerArrayHistory.size()-2; j>=0; j--){
            mapBoard(playerArrayHistory[j].tab, playerCordsHistory[j].x, playerCordsHistory[j].y);
        }

        //search maximum chance move
        int max_i_def=0;
        int max_j_def=0;
        int max_i_att=0;
        int max_j_att=0;
        int max_danger=0;
        int max_chance=0;
        //calculate danger level for defense
        for(int j=engineArrayHistory.size()-1; j>=0; j--){
            for(int i=0; i<4; i++){
                if(playerArrayHistory[j].tab[i].dangerousLevel>max_danger){
                    max_danger = playerArrayHistory[j].tab[i].dangerousLevel;
                    max_i_def = i;
                    max_j_def = j;
                }
            }
        }
        //find the best chance
        for(int j=engineArrayHistory.size()-1; j>=0; j--){
            for(int i=0; i<4; i++){
                if(engineArrayHistory[j].tab[i].dangerousLevel>max_chance){
                    max_chance = engineArrayHistory[j].tab[i].dangerousLevel;
                    max_i_att = i;
                    max_j_att = j;
                }
            }
        }
        char baseSymbol = b.getSymbol(p_lastX, p_lastY);
        char compSymbol = ((baseSymbol=='O') ? 'X' : 'O');
        bool instantWin = false;
        std::cout<<max_i_def<<" "<<max_i_att<<"\n";
        if(engineArrayHistory[max_j_att].tab[max_i_att].noPlayerSymbol>=4 && engineArrayHistory[max_j_att].tab[max_i_att].noCompSymbol!=2){
            instantWin=true;
            std::cout<<"Instant win!!!\n";
        }
        if(((max_danger>3 || max_danger>max_chance) && !instantWin) || defense){
            std::cout<<"Direction: "<<max_i_def<<std::endl;
            if(compSymbol!=b.getSymbol(playerArrayHistory[max_j_def].tab[max_i_def].xLimit[0], playerArrayHistory[max_j_def].tab[max_i_def].yLimit[0])){
                e_lastX = playerArrayHistory[max_j_def].tab[max_i_def].xLimit[0];
                e_lastY = playerArrayHistory[max_j_def].tab[max_i_def].yLimit[0];
            }
            else{
                e_lastX = playerArrayHistory[max_j_def].tab[max_i_def].xLimit[1];
                e_lastY = playerArrayHistory[max_j_def].tab[max_i_def].yLimit[1];
            }
        }
        else{
            std::cout<<"Direction: "<<max_i_att<<std::endl;
            if(baseSymbol!=b.getSymbol(engineArrayHistory[max_j_att].tab[max_i_att].xLimit[0], engineArrayHistory[max_j_att].tab[max_i_att].yLimit[0])){
                e_lastX = engineArrayHistory[max_j_att].tab[max_i_att].xLimit[0];
                e_lastY = engineArrayHistory[max_j_att].tab[max_i_att].yLimit[0];
            }
            else{
                e_lastX = engineArrayHistory[max_j_att].tab[max_i_att].xLimit[1];
                e_lastY = engineArrayHistory[max_j_att].tab[max_i_att].yLimit[1];
            }
        }
        std::cout<<"Comp Calc Move: "<<e_lastX<<" "<<e_lastY<<std::endl;
        v2 a;
        a.x = e_lastX;
        a.y = e_lastY;
        engineCordsHistory.push_back(a);
        b.setSymbol(e_lastX, e_lastY, compSymbol);
    }
}


void MyEngine::mapBoard(MoveCordsData *tab, int lastX, int lastY){
    int maxX = b.getMaxX();
    int maxY = b.getMaxY();
    int NoSymbol = 1; //player symbol
    int NoSymbol_2 = 0; //comp block
    char baseSymbol = b.getSymbol(lastX, lastY); //'0'
    char compSymbol = ((baseSymbol=='O') ? 'X' : 'O');
    int i=0, j=0;

    //var to choose symbol position
    int minX = lastX;
    int minY = lastY;
    int posX = lastX;
    int posY = lastY;

    //std::cout<<baseSymbol<<std::endl;

    //direction: W-E:
    if(lastX>0){
        for(int i=lastX-1; i>=0; i--){
                /*
            if(baseSymbol==b.getSymbol(i, lastY)){
                NoSymbol++;
                minX = i;
            }
            else if(compSymbol==b.getSymbol(i, lastY)){
                NoSymbol_2++;
                minX = i;
                break;
            }
            else break;
                */

            minX = i;
            if(compSymbol==b.getSymbol(i, lastY)) NoSymbol_2++;
            if(baseSymbol==b.getSymbol(i, lastY)) NoSymbol++;
            else break;
        }
    }
    if(lastX<maxX-1){
        for(int i=lastX+1; i<maxX; i++){
            posX = i;
            if(compSymbol==b.getSymbol(i, lastY)) NoSymbol_2++;
            if(baseSymbol==b.getSymbol(i, lastY)) NoSymbol++;
            else break;
        }
    }
    tab[2].noPlayerSymbol = NoSymbol;
    tab[2].noCompSymbol = NoSymbol_2;
    tab[2].dangerousLevel = ((NoSymbol_2==2) ? 0 : NoSymbol-NoSymbol_2+1);
    tab[2].xLimit[0]=minX;
    tab[2].xLimit[1]=posX;
    tab[2].yLimit[0]=minY;
    tab[2].yLimit[1]=posY;

    //direction: N-S:
    NoSymbol=1;
    NoSymbol_2=0;
    minX = lastX;
    minY = lastY;
    posX = lastX;
    posY = lastY;
    if(lastY>0){
        for(i=lastY-1; i>=0; i--){
            minY=i;
            if(compSymbol==b.getSymbol(lastX, i)) NoSymbol_2++;
            if(baseSymbol==b.getSymbol(lastX, i)) NoSymbol++;
            else break;
        }
    }
    if(lastY<maxY-1){
        for(i=lastY+1; i<maxY; i++){
            posY = i;
            if(compSymbol==b.getSymbol(lastX, i)) NoSymbol_2++;
            if(baseSymbol==b.getSymbol(lastX, i)) NoSymbol++;
            else break;
        }
    }
    tab[0].noPlayerSymbol = NoSymbol;
    tab[0].noCompSymbol = NoSymbol_2;
    tab[0].dangerousLevel = ((NoSymbol_2==2) ? 0 : NoSymbol-NoSymbol_2+1);
    tab[0].xLimit[0]=minX;
    tab[0].xLimit[1]=posX;
    tab[0].yLimit[0]=minY;
    tab[0].yLimit[1]=posY;

    //direction: NW-SE:
    NoSymbol=1;
    NoSymbol_2=0;
    minX = lastX;
    minY = lastY;
    posX = lastX;
    posY = lastY;
    if(lastY>0 && lastX>0){
        i=lastX-1;
        j=lastY-1;
        while(i>=0 && j>=0){
            minX--; minY--;
            if(compSymbol==b.getSymbol(i, j)) NoSymbol_2++;
            if(baseSymbol==b.getSymbol(i, j)) NoSymbol++;
            else break;

            i--; j--;
        }
    }
    if(lastX<maxX-1 && lastY<maxY-1){
        i=lastX+1;
        j=lastY+1;
        while(i<maxX && j<maxY){
            posX++; posY++;
            if(compSymbol==b.getSymbol(i, j)) NoSymbol_2++;
            if(baseSymbol==b.getSymbol(i, j)) NoSymbol++;
            else break;

             i++; j++;
        }
    }
    tab[3].noPlayerSymbol = NoSymbol;
    tab[3].noCompSymbol = NoSymbol_2;
    tab[3].dangerousLevel = ((NoSymbol_2==2) ? 0 : NoSymbol-NoSymbol_2+1);
    tab[3].xLimit[0]=minX;
    tab[3].xLimit[1]=posX;
    tab[3].yLimit[0]=minY;
    tab[3].yLimit[1]=posY;


    //direction: NE-SW:
    NoSymbol=1;
    NoSymbol_2=0;
    minX=lastX;
    minY=lastY;
    posX = lastX;
    posY = lastY;
    if(lastY>0 && lastX<maxX-1){
        i=lastX+1;
        j=lastY-1;
        while(i<maxX && j>=0){
            minY--;
            minX++;
            if(compSymbol==b.getSymbol(i, j)) NoSymbol_2++;
            if(baseSymbol==b.getSymbol(i, j)) NoSymbol++;
            else break;

            i++; j--;
        }
    }
    if(lastX>0 && lastY<maxY-1){
        i=lastX-1;
        j=lastY+1;
        while(i>=0 && j<maxY){
            posX--; posY++;
            if(compSymbol==b.getSymbol(i, j)) NoSymbol_2++;
            if(baseSymbol==b.getSymbol(i, j)) NoSymbol++;
            else break;

            i--; j++;
        }
    }
    tab[1].noPlayerSymbol = NoSymbol;
    tab[1].noCompSymbol = NoSymbol_2;
    tab[1].dangerousLevel = ((NoSymbol_2==2) ? 0 : NoSymbol-NoSymbol_2+1);
    tab[1].xLimit[0]=minX;
    tab[1].xLimit[1]=posX;
    tab[1].yLimit[0]=minY;
    tab[1].yLimit[1]=posY;

    /*
    std::cout<<lastX<<" "<<lastY<<"\n";
    for(int i=0; i<4; i++) std::cout<<tab[i].noPlayerSymbol<<"\t";
    std::cout<<"\n";
    for(int i=0; i<4; i++) std::cout<<tab[i].noCompSymbol<<"\t";
    std::cout<<"\n";
    for(int i=0; i<4; i++) std::cout<<tab[i].dangerousLevel<<"\t";
    std::cout<<"\n";
    for(int i=0; i<4; i++) std::cout<<tab[i].xLimit[0]<<"\t";
    std::cout<<"\n";
    for(int i=0; i<4; i++) std::cout<<tab[i].yLimit[0]<<"\t";
    std::cout<<"\n";
    for(int i=0; i<4; i++) std::cout<<tab[i].xLimit[1]<<"\t";
    std::cout<<"\n";
    for(int i=0; i<4; i++) std::cout<<tab[i].yLimit[1]<<"\t";
    std::cout<<"\n\n";
    */

}
