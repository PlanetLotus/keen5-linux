#ifndef BLASTERSHOT_H
#define BLASTERSHOT_H

#include <vector>
#include "SDL.h"
#include "Sprite.h"

class BlasterShot : public Sprite {
    private:
        int expireTimer;

        unsigned int frame;
        int animState;
        std::vector<SDL_Rect> anims[2];
    public:
        BlasterShot(int startX, int startY, float velocityX, float velocityY);
        TileCollisionInfo update();
        void animate(int nextState);
        void draw(TileCollisionInfo tci);
        void expire();
        void die();
};

#endif
