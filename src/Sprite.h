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
        std::vector<Tile*> GetTilesToLeft();
        std::vector<Tile*> GetTilesToRight();
        std::vector<Tile*> GetTilesToTop();
        std::vector<Tile*> GetTilesToBottom();
        Tile* GetTileCollidingWithRight();
        Tile* GetTileCollidingWithLeft();
        Tile* GetTileCollidingWithBottom();
        Tile* GetTileCollidingWithTop();
};

#endif
