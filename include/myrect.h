#ifndef OX_GAME_MYRECT_H
#define OX_GAME_MYRECT_H

#include <SDL2/SDL.h>

class MyRect
{
    public:
        //MyRect();
        MyRect(SDL_Renderer &i_renderer, int i_sx, int i_sy, int i_x, int i_y, int i_objectColor[3], int i_borderSize, int i_borderColor[3]);
        ~MyRect();
        //void setSymbol(char s);
        void rh_setPos(char s);
        void draw() const;
        void changeBlinkStatus(bool iBlink);

    protected:
        SDL_Renderer &renderer;
        int sx;
        int sy;
        int dx;
        int dy;
        int borderSize;
        int borderColor[3];
        int objectColor[3];
        SDL_Rect *object;
        SDL_Rect *border;
        char symbol;
        bool blink;

};

#endif // OX_GAME_MYRECT_H
