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
        virtual void update();
        virtual void draw();
        TileCollisionInfo CheckTileCollisionLR();
        TileCollisionInfo CheckTileCollisionTB();
        Tile* GetTileCollidingWithRight(int minRow, int maxRow, SDL_Rect nextHitbox);
        Tile* GetTileCollidingWithLeft(int minRow, int maxRow, SDL_Rect nextHitbox);
        Tile* GetTileCollidingWithBottom(int minCol, int maxCol, SDL_Rect nextHitbox);
        Tile* GetTileCollidingWithTop(int minCol, int maxCol, SDL_Rect nextHitbox);
};

#endif
