#ifndef TILE_H
#define TILE_H

#include "SDL.h"

const int TILE_WIDTH = 32;
const int TILE_HEIGHT = 32;
const int TILE_OFFSET = 0;

class Tile {
    private:
        SDL_Rect srcBox;
        SDL_Rect destBox;

        bool hasCollision;
        bool collideTop;
        bool collideRight;
        bool collideBottom;
        bool collideLeft;

    public:
        enum TileProperty { LEFT, RIGHT, TOP, BOTTOM, ISPOLE };

        // Inits position and type
        Tile(int srcX, int srcY, int destX, int destY, bool collideT, bool collideR, bool collideB, bool collideL, int layer, bool isPole);

        // Shows the tile
        void render(SDL_Rect& camera);

        SDL_Rect getBox();
        bool IsColliding(TileProperty tileProperty, SDL_Rect hitbox, SDL_Rect nextHitbox);
        bool HasCollision();
        bool CollideTop();
        bool CollideRight();
        bool CollideBottom();
        bool CollideLeft();

        bool IsPole;
        int Layer;
};

#endif
