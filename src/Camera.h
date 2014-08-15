#ifndef CAMERA_H
#define CAMERA_H

#include "SDL.h"

const int smoothUpdateFactor = 5;

class Camera {
    private:
        SDL_Rect box;
        int topMargin;
        int bottomMargin;
        int leftBubble;
        int rightBubble;

    public:
        Camera();
        void Update(SDL_Rect keenHitbox, bool isOnGround);
        SDL_Rect GetBox();
        int GetX();
        int GetY();
        int GetTopMargin();
        int GetBottomMargin();
};

#endif
