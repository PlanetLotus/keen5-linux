#ifndef SPRITE_H
#define SPRITE_H

#include "SDL.h"

class Sprite {
    private:
        SDL_Rect* srcClip;
        unsigned int frame;
        int animState;
        //std::vector<SDL_Rect> anims[0];
    public:
        virtual void update();
        void animate(int nextState);
        virtual void draw();
};

#endif
