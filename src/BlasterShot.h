#ifndef BLASTERSHOT_H
#define BLASTERSHOT_H

#include <vector>
#include "SDL.h"
#include "Sprite.h"

class BlasterShot : public Sprite {
    private:
        int xVel;
        int yVel;

        SDL_Rect* srcClip;
        SDL_Rect hitbox;

        unsigned int frame;
        int animState;
        std::vector<SDL_Rect> anims[2];
    public:
        BlasterShot(int startX, int startY, int velocityX, int velocityY);
        ~BlasterShot();
        void update();
        void animate(int nextState);
        void draw();
};

#endif
