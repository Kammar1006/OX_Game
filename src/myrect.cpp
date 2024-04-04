#include "myrect.h"
#include <iostream>

//SDL2 Draw Circle Function:
#include "circle.h"


//constructor of MyRect:
MyRect::MyRect(SDL_Renderer &i_renderer, int i_sx, int i_sy, int i_x, int i_y, int i_objectColor[], int i_borderSize, int i_borderColor[]) : renderer{i_renderer}
{
    border = new SDL_Rect {i_sx, i_sy, i_x, i_y};
    object = new SDL_Rect {i_sx+i_borderSize, i_sy+i_borderSize, i_x-2*i_borderSize, i_y-2*i_borderSize};
    sx = i_sx;
    sy = i_sy;
    dx = i_x;
    dy = i_y;
    borderSize = i_borderSize;
    for(int i=0; i<3; i++){
        borderColor[i] = i_borderColor[i];
        objectColor[i] = i_objectColor[i];
    }
    blink=false;
    std::cout<<sx<<" "<<sy<<" "<<dx<<" "<<dy<<std::endl;
    symbol=' ';
}

void MyRect::draw() const{
    //draw border:
    SDL_SetRenderDrawColor(&renderer, borderColor[0], borderColor[1], borderColor[2], 255);
    SDL_RenderFillRect(&renderer, border);
    //draw obj:
    SDL_SetRenderDrawColor(&renderer, objectColor[0], objectColor[1], objectColor[2], 255);
    if(blink && symbol==' ') SDL_SetRenderDrawColor(&renderer, 200, 40, 40, 255);
    SDL_RenderFillRect(&renderer, object);
    //std::cout<<"DRAW: "<<sx<<" "<<sy<<" "<<dx<<" "<<dy<<std::endl;
    if(symbol=='O'){
        if(blink) SDL_SetRenderDrawColor(&renderer, 230, 30, 30, 255);
        else SDL_SetRenderDrawColor(&renderer, borderColor[0], borderColor[1], borderColor[2], 255);
        SDL_RenderFillCircle(&renderer, sx+dx/2, sy+dy/2, 20);
        SDL_SetRenderDrawColor(&renderer, objectColor[0], objectColor[1], objectColor[2], 255);
        SDL_RenderFillCircle(&renderer, sx+dx/2, sy+dy/2, 18);
    }
    if(symbol=='X'){
        if(blink) SDL_SetRenderDrawColor(&renderer, 230, 30, 30, 255);
        else SDL_SetRenderDrawColor(&renderer, borderColor[0], borderColor[1], borderColor[2], 255);
        SDL_RenderDrawLine(&renderer, sx+10, sy+10, sx+dx-10, sy+dy-10);
        SDL_RenderDrawLine(&renderer, sx+dx-10, sy+10,sx+10, sy+dy-10);
        for(int a=1; a<=1; a++){
            SDL_RenderDrawLine(&renderer, sx+10, sy+10+a, sx+dx-10, sy+dy-10+a);
            SDL_RenderDrawLine(&renderer, sx+10+a, sy+10, sx+dx-10+a, sy+dy-10);
            SDL_RenderDrawLine(&renderer, sx+dx-10, sy+10+a,sx+10, sy+dy-10+a);
            SDL_RenderDrawLine(&renderer, sx+dx-10+a, sy+10,sx+10+a, sy+dy-10);
        }
    }
}

void MyRect::changeBlinkStatus(bool iBlink){
    blink = iBlink;
}

void MyRect::rh_setPos(char s){
    symbol = s;
}

MyRect::~MyRect(){
    //std::cout<<"del"<<std::endl;
    delete border;
    delete object;
}
