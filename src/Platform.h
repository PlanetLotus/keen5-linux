#ifndef PLATFORM_H
#define PLATFORM_H

#include "Player.h"
#include "SDL.h"
#include "Texture.h"

class Player;

class Platform {
    private:
        SDL_Rect hitbox;
        SDL_Rect srcRect;
        Player* keen;

        static const int speed = 4;
        float xVel;
        float yVel;

        bool playerIsStandingOnThis(SDL_Rect keenBox);

    public:
        Platform(Player* player);
        void update();
        void draw(Texture* texture, SDL_Rect cameraBox);
        SDL_Rect getBox();
        float getXVel();
        float getYVel();
};

#endif
