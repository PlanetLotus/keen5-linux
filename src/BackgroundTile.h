#ifndef BACKGROUNDTILE_H
#define BACKGROUNDTILE_H

#include "SDL.h"

class Texture;

class BackgroundTile {
    private:
        SDL_Rect srcBox;
        SDL_Rect destBox;
        int layer;

    public:
        BackgroundTile(int srcX, int srcY, int destX, int destY, int layer);
        void draw(Texture* texture, SDL_Rect cameraBox);
        int getLayer();

};

#endif
