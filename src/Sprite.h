#ifndef SPRITE_H
#define SPRITE_H

#include "SDL.h"
#include "TileCollisionInfo.h"

class Sprite {
    protected:
        SDL_Rect hitbox;
        SDL_Rect* srcClip;

        /*
        bool isTopColliding;
        bool isBottomColliding;
        bool isLeftColliding;
        bool isRightColliding;
        */

        float xVel;
        float yVel;
        float xVelRem;
        float yVelRem;
    public:
        virtual ~Sprite();
        virtual TileCollisionInfo update();
        virtual void draw(TileCollisionInfo tci);
        TileCollisionInfo CheckTileCollision();
        /*
        void CheckTBCollision();
        void CheckLRCollision();
        bool CheckTopCollision(int minCol, int maxCol, SDL_Rect nextHitbox);
        virtual bool CheckBottomCollision(int minCol, int maxCol, SDL_Rect nextHitbox);
        bool CheckLeftCollision(int minRow, int maxRow, SDL_Rect nextHitbox);
        bool CheckRightCollision(int minRow, int maxRow, SDL_Rect nextHitbox);
        */
};

#endif
