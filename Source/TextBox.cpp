#include "TextBox.h"


void TextBox::setSize(SDL_Rect &rect)
{
    Box=rect;
}

void TextBox::handleEvent(SDL_Event* e)
{
    if( e->type == SDL_MOUSEMOTION || e->type == SDL_MOUSEBUTTONDOWN || e->type == SDL_MOUSEBUTTONUP )
	{
		int x, y;
		SDL_GetMouseState( &x, &y );

		bool inside = true;

		if( x < Box.x )
		{
			inside = false;
		}
		else if( x > Box.x + Box.w )
		{
			inside = false;
		}
		else if( y < Box.y )
		{
			inside = false;
		}
		else if( y > Box.y + Box.h )
		{
			inside = false;
		}

		if( !inside )
		{
		    if (e->type == SDL_MOUSEBUTTONDOWN)
                mCurrentState = MOUSE_OUT_DOWN;
            else mCurrentState = MOUSE_OUT;
		}
		else
		{
			switch( e->type )
			{
				case SDL_MOUSEMOTION:
				mCurrentState = MOUSE_OVER_MOTION;
				break;

				case SDL_MOUSEBUTTONDOWN:
				mCurrentState = MOUSE_DOWN;
				break;

				case SDL_MOUSEBUTTONUP:
				mCurrentState = MOUSE_UP;
				break;
			}
		}
	}
}

int TextBox::getCurrentState()
{
    return (mCurrentState);
}

