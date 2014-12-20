#ifndef BACKGROUNDTILE_H
#define BACKGROUNDTILE_H

#include "SDL.h"

class Texture;

class BackgroundTile {
    private:
        SDL_Rect srcBox;
        SDL_Rect destBox;

    public:
        BackgroundTile(int srcX, int srcY, int destX, int destY);
        void draw(Texture* texture, SDL_Rect cameraBox);

};

#endif
