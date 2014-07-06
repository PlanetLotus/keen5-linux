#ifndef SPRITE_H
#define SPRITE_H

#include "SDL.h"
#include "TileCollisionInfo.h"

class Sprite {
    protected:
        SDL_Rect hitbox;
        SDL_Rect* srcClip;

        enum TileProperty { LEFT, RIGHT, TOP, BOTTOM, ISPOLE };

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
        bool IsTileColliding(Tile* tile, TileProperty tileProperty);
        std::vector<Tile*> GetTilesToLeft();
        std::vector<Tile*> GetTilesToRight();
        std::vector<Tile*> GetTilesToTop();
        std::vector<Tile*> GetTilesToBottom();
        Tile* GetTileCollidingWithRight();
        Tile* GetTileCollidingWithLeft();
        Tile* GetTileCollidingWithBottom(int minCol, int maxCol, SDL_Rect nextHitbox);
        Tile* GetTileCollidingWithTop(int minCol, int maxCol, SDL_Rect nextHitbox);
};

#endif
