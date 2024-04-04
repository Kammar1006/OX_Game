#include "my_button.h"

MyButton::MyButton(SDL_Renderer &i_renderer, int i_sx, int i_sy, int i_x, int i_y, int i_objectColor[3], int i_borderSize, int i_borderColor[3])
    : MyRect(i_renderer, i_sx, i_sy, i_x, i_y, i_objectColor, i_borderSize, i_borderColor){

}

MyButton::~MyButton()
{
    this->~MyRect();
}

bool MyButton::isOnClick(int i_x, int i_y) const{
    if(i_x>sx && i_x<(sx+dx) && i_y>sy && i_y<(dy+sy)) return true;
    return false;
}
