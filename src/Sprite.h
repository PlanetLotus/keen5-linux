#ifndef SPRITE_H
#define SPRITE_H

#include "SDL.h"

class Sprite {
    protected:
        SDL_Rect hitbox;
        SDL_Rect* srcClip;
        unsigned int frame;
        int animState;
        //std::vector<SDL_Rect> anims[0];

        int xVel;
        int yVel;
    public:
        virtual void update();
        void animate(int nextState);
        virtual void draw();
        void CheckCollision();
        void CheckTBCollision();
        void CheckLRCollision();
        void CheckTopCollision(int minCol, int maxCol, int row, SDL_Rect nextHitbox);
        virtual void CheckBottomCollision(int minCol, int maxCol, int row, SDL_Rect nextHitbox);
        void CheckLeftCollision(int col, int minRow, int maxRow, SDL_Rect nextHitbox);
        void CheckRightCollision(int col, int minRow, int maxRow, SDL_Rect nextHitbox);
};

#endif
