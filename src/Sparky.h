#ifndef SPARKY_H
#define SPARKY_H

#include "SDL.h"

class Sparky : public Sprite {
    private:
        enum stateEnum { PATROL, CHASE, SCAN, CHANGE_DIRECTION, STUNNED };
        stateEnum state;
        void patrol();
        void chase();
        void scan();
        void changeDirection();
        void stunned();

        enum facingEnum { LEFT = -1, RIGHT = 1 };
        facingEnum facing;
        int patrolSpeed;

        void fall();
        std::vector<SDL_Rect> anims[5];
        void animate(int nextState, int frametime = FRAMETIME);

    public:
        Sparky();
        void update();
        void draw(SDL_Rect cameraBox);
        void takeShotByPlayer();
};

#endif
