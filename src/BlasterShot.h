#ifndef BLASTERSHOT_H
#define BLASTERSHOT_H

#include "globals.h"

class BlasterShot {
    private:
        int speed;
        int startX;
        int startY;

        SDL_Rect* srcClip;
    public:
        BlasterShot(int startX, int startY);
};

#endif
