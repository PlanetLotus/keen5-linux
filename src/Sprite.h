#ifndef SPRITE_H
#define SPRITE_H

#include "SDL.h"

class Texture;

class Sprite {
    protected:
        SDL_Rect hitbox;
        SDL_Rect* srcClip;

        unsigned int frame;
        int animState;
    public:
        virtual void update(float timeDelta) = 0;
        virtual void draw(Texture* texture, SDL_Rect cameraBox) = 0;
        SDL_Rect getBox();
};

#endif
