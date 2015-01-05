#ifndef MOVINGSPRITE_H
#define MOVINGSPRITE_H

#include <vector>
#include "SDL.h"
#include "Sprite.h"
#include "TileCollisionInfo.h"
#include "Timer.h"

class Level;

class MovingSprite : public Sprite {
    protected:
        bool isStunned;

        float xVel;
        float yVel;
        float xVelRem;
        float yVelRem;

        Timer timer;

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
        SDL_Rect getNextHitboxX();
        SDL_Rect getNextHitboxXY();

        bool isUnitColliding(SDL_Rect unitBox);
        static const std::vector< std::vector<Tile*> >& tilesRef;
        static Level*& currentLevelRef;
    public:
        bool getIsStunned();
        static float timeDelta;
};

#endif
