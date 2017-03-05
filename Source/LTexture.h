#pragma once
#include "header.h"

class LTexture
{
    public:
        LTexture();

        ~LTexture();

        bool loadFromFile(SDL_Renderer* gRenderer, std::string path, bool ChangeSize=false, int x=0, int y=0 );

        bool loadFromRenderedText(SDL_Renderer* gRenderer, std::string textureText, SDL_Color textColor );

        void free();

        void setColor( Uint8 red, Uint8 green, Uint8 blue );

        void setBlendMode( SDL_BlendMode blending );

        void setAlpha( Uint8 alpha );

        void render(SDL_Renderer* gRenderer, int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE );

        void setFont(TTF_Font* gF);

        int getWidth();
        int getHeight();
        void setSize(int w, int h);


    private:
        SDL_Texture* mTexture;

        int mWidth;
        int mHeight;
        TTF_Font* gFont;
};
