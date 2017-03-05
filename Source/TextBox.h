#pragma once
#include "header.h"

class TextBox
{
    public:
        enum MouseEvent
        {
            MOUSE_OUT = 0,
            MOUSE_OVER_MOTION = 1,
            MOUSE_DOWN = 2,
            MOUSE_UP = 3,
            MOUSE_OUT_DOWN = 4
        };
        void setSize(SDL_Rect &srect);
        void handleEvent(SDL_Event* e);
        int getCurrentState();

    private:
        SDL_Rect Box;
        MouseEvent mCurrentState;

};
