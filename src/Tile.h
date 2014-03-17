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

    public:
        // Inits position and type
        Tile(int srcX, int srcY, int destX, int destY);

        // Shows the tile
        void render(int destX, int destY, SDL_Rect& camera);

        SDL_Rect getBox();
};

#endif
