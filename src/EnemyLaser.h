#ifndef ENEMYLASER_H
#define ENEMYLASER_H

#include <vector>
#include "Laser.h"
#include "SDL.h"

class Player;
class Texture;

class EnemyLaser : public Laser {
    private:
        int expireTimer;

        // Values are in pixels per second
        const int speed = 480;

        Player* keen;
        bool isCollidingWithPlayer();

        std::vector<std::vector<SDL_Rect>> anims;
    public:
        EnemyLaser(int startX, int startY, float velocityX, float velocityY);
        void update();
        void draw(Texture* texture, SDL_Rect cameraBox);
        void setPlayer(Player* player); // Ideally this would be done at instantiation, not later
};

#endif
