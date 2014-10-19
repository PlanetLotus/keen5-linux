#ifndef SPRITE_H
#define SPRITE_H

#include <vector>
#include "Level.h"
#include "SDL.h"
#include "TileCollisionInfo.h"

class Sprite {
    protected:
        SDL_Rect hitbox;
        SDL_Rect* srcClip;
        bool isStunned;

        unsigned int frame;
        int animState;

        float xVel;
        float yVel;
        float xVelRem;
        float yVelRem;

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

        bool isUnitColliding(SDL_Rect unitBox);
        static const std::vector< std::vector<Tile*> >& tilesRef;
        static Level*& currentLevelRef;
    public:
        virtual void update() = 0;
        virtual void draw(Texture* texture, SDL_Rect cameraBox) = 0;
        SDL_Rect getBox();
        bool getIsStunned();
};

#endif
