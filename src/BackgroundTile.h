#ifndef BACKGROUNDTILE_H
#define BACKGROUNDTILE_H

#include "SDL.h"
#include "Texture.h"

const int TILE_WIDTH = 32;
const int TILE_HEIGHT = 32;
const int TILE_OFFSET = 0;

class BackgroundTile {
    private:
        SDL_Rect srcBox;
        SDL_Rect destBox;

    public:
        BackgroundTile(int srcX, int srcY, int destX, int destY);
        void draw(Texture* texture, SDL_Rect cameraBox);

};

#endif
