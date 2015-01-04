#ifndef BLASTERSHOT_H
#define BLASTERSHOT_H

#include <vector>
#include "SDL.h"
#include "MovingSprite.h"

class Enemy;

class BlasterShot : public MovingSprite {
    private:
        int expireTimer;

        // Values are in pixels per second
        const int speed = 480;

        std::vector< std::vector<SDL_Rect> > anims;
        void animate(int nextState);
        static const std::vector<Enemy*>& enemyBatchRef;
        static std::vector<BlasterShot*>& blasterShotBatchRef;
    public:
        BlasterShot(int startX, int startY, float velocityX, float velocityY);
        void update();
        void draw(Texture* texture, SDL_Rect cameraBox);
        void expire();
        void die();
};

#endif
