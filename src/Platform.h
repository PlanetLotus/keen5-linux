#ifndef PLATFORM_H
#define PLATFORM_H

#include "Player.h"
#include "SDL.h"

class Platform {
    private:
        SDL_Rect hitbox;
        SDL_Rect srcRect;
        Player* keen;
        Texture* gKeenTexture;

        static const int speed = 4;
        float xVel;
        float yVel;

    public:
        Platform(Player* player);
        void update();
        void draw(SDL_Rect cameraBox);
};

#endif
