#ifndef LASER_H
#define LASER_H

#include <vector>
#include "SDL.h"
#include "MovingSprite.h"

class Laser : public MovingSprite {
    protected:
        int expireTimer;

        // Values are in pixels per second
        const int speed = 480;

        std::vector< std::vector<SDL_Rect> > anims;
        void animate(int nextState);
        static std::vector<Laser*>& laserBatchRef;
    public:
        Laser(int startX, int startY, float velocityX, float velocityY);
        void expire();
        void die();
};

#endif
