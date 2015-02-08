#ifndef PLATFORM_H
#define PLATFORM_H

#include <vector>
#include "SDL.h"

class Player;
class Texture;

class Platform {
    private:
        SDL_Rect hitbox;
        SDL_Rect srcRect;
        Player* keen;

        static const int speed = 96;
        float xVel;
        float yVel;
        std::vector<std::pair<int, int>> path;

        bool playerIsStandingOnThis(SDL_Rect keenBox);

    public:
        Platform(int spawnX, int spawnY, std::vector<std::pair<int, int>> dests);
        void update();
        void draw(Texture* texture, SDL_Rect cameraBox);
        SDL_Rect getBox();
        float getXVel();
        float getYVel();
        void setPlayer(Player* player); // Ideally this would be done at instantiation, not later
};

#endif
