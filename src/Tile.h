#ifndef TILE_H
#define TILE_H

#include "SDL.h"

const int TILE_WIDTH = 32;
const int TILE_HEIGHT = 32;

class Tile {
    private:
        SDL_Rect srcBox;
        int type;

    public:
        // Inits position and type
        Tile(int x, int y, int tileType);

        // Shows the tile
        void render(int destX, int destY, SDL_Rect& camera);

        int getType();

        SDL_Rect getBox();
};

#endif
