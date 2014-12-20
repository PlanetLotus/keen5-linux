#ifndef CAMERA_H
#define CAMERA_H

#include "SDL.h"

const int smoothUpdateFactor = 5;

class Level;
class Camera {
    private:
        SDL_Rect box;
        int topMargin;
        int bottomMargin;
        int leftBubble;
        int rightBubble;
        int yOffset;
        static Level*& currentLevelRef;

    public:
        Camera();
        void update(SDL_Rect keenHitbox, bool isOnGround);
        void lookUp();
        void lookDown();
        SDL_Rect getBox();
        int getX();
        int getY();
        int getRight();
        int getBottom();
        int getTopMargin();
        int getBottomMargin();
        bool isDisabled;
};

#endif
