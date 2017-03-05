#include "LButton.h"

LButton::LButton()
{
    mPosition.x = 0;
    mPosition.y = 0;

    mCurrentSprite = BUTTON_SPRITE_MOUSE_OUT;
}

void LButton::setSize(SDL_Rect &Box)
{
    mPosition.x = Box.x;
    mPosition.y = Box.y;
    BUTTON_HEIGHT = Box.h;
    BUTTON_WIDTH = Box.w;
}

void LButton::handleEvent( SDL_Event* e )
{
	if( e->type == SDL_MOUSEMOTION || e->type == SDL_MOUSEBUTTONDOWN || e->type == SDL_MOUSEBUTTONUP )
	{
		int x, y;
		SDL_GetMouseState( &x, &y );

		bool inside = true;

		if( x < mPosition.x )
		{
			inside = false;
		}
		else if( x > mPosition.x + BUTTON_WIDTH )
		{
			inside = false;
		}
		else if( y < mPosition.y )
		{
			inside = false;
		}
		else if( y > mPosition.y + BUTTON_HEIGHT )
		{
			inside = false;
		}

		if( !inside )
		{
            mCurrentSprite = BUTTON_SPRITE_MOUSE_OUT;
		}
		else
		{
			switch( e->type )
			{
				case SDL_MOUSEMOTION:
				mCurrentSprite = BUTTON_SPRITE_MOUSE_OVER_MOTION;
				break;

				case SDL_MOUSEBUTTONDOWN:
				mCurrentSprite = BUTTON_SPRITE_MOUSE_DOWN;
				break;

				case SDL_MOUSEBUTTONUP:
				mCurrentSprite = BUTTON_SPRITE_MOUSE_UP;
				break;
			}
		}
	}
}

void LButton::renderFromSpriteSheet(SDL_Renderer* gRenderer, LTexture& gButtonSpriteSheetTexture, SDL_Rect gSpriteClips[4])
{
	gButtonSpriteSheetTexture.render(gRenderer, mPosition.x, mPosition.y, &gSpriteClips[ mCurrentSprite ] );
}

//void LButton::NormalRender(SDL_Renderer* gRenderer, SDL_Texture gBtnSprite[], SDL_Rect Button)
//{
//    SDL_RenderCopy(gRenderer,&gBtnSprite[mCurrentSprite],NULL,&Button);
//}

int LButton::GetCurrentSprite()
{
    return (mCurrentSprite);
}

int LButton::GetWidth()
{
    return (BUTTON_WIDTH);
}

int LButton::GetHeight()
{
    return (BUTTON_HEIGHT);
}

bool LButton::Clicked()
{
    if (mCurrentSprite==BUTTON_SPRITE_MOUSE_DOWN) return true;
    return false;
}
