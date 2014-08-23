#ifndef SPRITE_H
#define SPRITE_H

#include "SDL.h"
#include "TileCollisionInfo.h"

class Sprite {
    protected:
        SDL_Rect hitbox;
        SDL_Rect* srcClip;

        unsigned int frame;
        int animState;

        float xVel;
        float yVel;
        float xVelRem;
        float yVelRem;

        bool isUnitColliding(SDL_Rect unitBox);
    public:
        virtual ~Sprite();
        virtual void update() = 0;
        virtual void draw(SDL_Rect cameraBox) = 0;
        TileCollisionInfo checkTileCollisionLR();
        TileCollisionInfo checkTileCollisionTB();
        std::vector<Tile*> getTilesToLeft();
        std::vector<Tile*> getTilesToRight();
        std::vector<Tile*> getTilesToTop();
        std::vector<Tile*> getTilesToBottom();
        Tile* getTileCollidingWithRight();
        Tile* getTileCollidingWithLeft();
        Tile* getTileCollidingWithBottom(bool checkOnlyTouching = false);
        Tile* getTileCollidingWithTop();
        SDL_Rect getBox();
};

#endif
