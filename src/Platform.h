#ifndef PLATFORM_H
#define PLATFORM_H

#include "SDL.h"

class Platform {
    private:
        SDL_Rect hitbox;
        SDL_Rect srcRect;

        static const int speed = 4;
        float xVel;
        float yVel;

    public:
        Platform();
        void update();
        void draw(SDL_Rect cameraBox);
};

#endif
