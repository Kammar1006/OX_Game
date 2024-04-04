#ifndef OX_GAME_MYBUTTON_H
#define OX_GAME_MYBUTTON_H

#include "myrect.h"
#include <SDL2/SDL.h>


class MyButton : public MyRect
{
    public:
        MyButton(SDL_Renderer &i_renderer, int i_sx, int i_sy, int i_x, int i_y, int i_objectColor[3], int i_borderSize, int i_borderColor[3]);
        ~MyButton();
        bool isOnClick(int i_x, int i_y) const;
};

#endif // OX_GAME_MYBUTTON_H
