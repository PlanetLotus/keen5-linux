#ifndef BLASTERSHOT_H
#define BLASTERSHOT_H

#include "globals.h"

using namespace std;

class BlasterShot : public Sprite {
    private:
        int xVel;
        int yVel;

        SDL_Rect* srcClip;
        SDL_Rect hitbox;

        unsigned int frame;
        int animState;
        vector<SDL_Rect> anims[2];
    public:
        BlasterShot(int startX, int startY, int velocityX, int velocityY);
        void update();
        void animate(int nextState);
        void draw();
};

#endif
