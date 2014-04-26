#include "BlasterShot.h"

//std::vector<SDL_Rect> ANIMS[2] = { MOVE_ANIM, COLLIDE_ANIM };

BlasterShot::BlasterShot(int startX, int startY) {
    this->startX = startX;
    this->startY = startY;

    speed = 10;
    srcClip = NULL;
}
