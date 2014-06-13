#ifndef SPRITE_H
#define SPRITE_H

#include "SDL.h"
#include "TileCollisionInfo.h"

class Sprite {
    protected:
        SDL_Rect hitbox;
        SDL_Rect* srcClip;

        float xVel;
        float yVel;
        float xVelRem;
        float yVelRem;
    public:
        virtual ~Sprite();
        virtual TileCollisionInfo update();
        virtual void draw(TileCollisionInfo tci);
        TileCollisionInfo CheckTileCollisionLR();
        TileCollisionInfo CheckTileCollisionTB();
};

#endif
