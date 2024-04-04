#include "my_text.h"

MyText::MyText(SDL_Renderer &i_renderer, const char* iText, int iSize, int iY, SDL_Color iColor, char iAlign, std::string iFontPath) : renderer{i_renderer}
{
    int windowSize = 550;

    //this opens a font style and sets a size
    font = TTF_OpenFont("OpenSans.ttf", iSize);

    color = iColor;
    Align = iAlign;

    // as TTF_RenderText_Solid could only be used on
    // SDL_Surface then you have to create the surface first
    surfaceMessage = TTF_RenderText_Solid(font, iText, color);

    // now you can convert it into a texture
    Message = SDL_CreateTextureFromSurface(&renderer, surfaceMessage);

    Message_rect.x = 0;  //controls the rect's x coordinate
    Message_rect.y = iY; // controls the rect's y coordinte
    Message_rect.w = surfaceMessage->w; // controls the width of the rect
    Message_rect.h = surfaceMessage->h; // controls the height of the rect

    if(Align=='c'){
        Message_rect.x = windowSize/2 - Message_rect.w/2;
    }
    if(Align=='r'){
        Message_rect.x = windowSize - Message_rect.w;
    }

}

void MyText::draw() const{
    SDL_RenderCopy(&renderer, Message, NULL, &Message_rect);
}

MyText::~MyText()
{
    TTF_CloseFont(font);
    SDL_DestroyTexture( Message );
    SDL_FreeSurface( surfaceMessage );
}
