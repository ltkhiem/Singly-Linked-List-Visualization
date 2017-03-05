#pragma once
#include "header.h"
#include "LTexture.h"

class LButton
{
	public:
	    enum LButtonSprite
        {
            BUTTON_SPRITE_MOUSE_OUT = 0,
            BUTTON_SPRITE_MOUSE_OVER_MOTION = 1,
            BUTTON_SPRITE_MOUSE_DOWN = 2,
            BUTTON_SPRITE_MOUSE_UP = 3,
        };

		LButton();

		void setSize(SDL_Rect &Box);

		void handleEvent( SDL_Event* e );

        void renderFromSpriteSheet(SDL_Renderer* gRenderer, LTexture& gButtonSpriteSheetTexture, SDL_Rect gSpriteClips[4]);
        //void NormalRender(SDL_Renderer* gRenderer, SDL_Texture gBtnSprite[], SDL_Rect Button);
        int GetCurrentSprite();

		int GetWidth();
		int GetHeight();
        bool Clicked();

	private:
		SDL_Point mPosition;

		LButtonSprite mCurrentSprite;

		int BUTTON_WIDTH = 100;
        int BUTTON_HEIGHT = 50;

};
