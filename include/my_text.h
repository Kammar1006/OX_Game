#ifndef OX_GAME_MYTEXT_H
#define OX_GAME_MYTEXT_H


#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>


class MyText
{
    public:
       // MyText();
        MyText(SDL_Renderer &i_renderer, const char* iText, int iSize, int iY, SDL_Color iColor, char iAlign='c', std::string iFontPath="OpenSans.ttf");
        void draw() const;
        ~MyText();

    protected:

    private:
        SDL_Renderer &renderer;
        char Align;
        SDL_Color color;
        TTF_Font* font;
        SDL_Surface* surfaceMessage;
        SDL_Texture* Message;
        SDL_Rect Message_rect; //0, 0, dx, dy
};

#endif // OX_GAME_MYTEXT_H
