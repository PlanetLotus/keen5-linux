#ifndef SPRITE_H
#define SPRITE_H

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

        bool isUnitColliding(SDL_Rect unitBox);
        static const std::vector< std::vector<Tile*> >& tilesRef;
    public:
        virtual void update() = 0;
        virtual void draw(Texture* texture, SDL_Rect cameraBox) = 0;
        virtual void takeShotByPlayer(); // If I ever have an "Enemy" base class, this should go there instead
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
        bool getIsStunned();
};

#endif
