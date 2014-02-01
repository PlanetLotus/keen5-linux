#ifndef TILE_H
#define TILE_H

#include "SDL.h"

const int TILE_WIDTH = 80;
const int TILE_HEIGHT = 80;

class Tile {
    private:
        SDL_Rect box;
        int type;

    public:
        // Inits position and type
        Tile(int x, int y, int tileType);

        // Shows the tile
        void render(SDL_Rect& camera);

        int getType();

        SDL_Rect getBox();
};

#endif
