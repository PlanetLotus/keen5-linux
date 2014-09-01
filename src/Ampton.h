#ifndef AMPTON_H
#define AMPTON_H

#include "globals.h"
#include "Player.h"
#include "SDL.h"

class Ampton : public Sprite {
    private:
        std::vector<SDL_Rect> anims[3];
        void animate(int nextState, int frametime = FRAMETIME);

        enum facingEnum { LEFT = -1, RIGHT = 1 };
        facingEnum facing;
        int patrolSpeed;

        enum stateEnum { PATROL, CLIMB_UP, CLIMB_DOWN, CHANGE_DIRECTION, FIX_MACHINE, STUNNED };
        stateEnum state;

        Player* keen;

        void fall();
        void changeState(stateEnum nextState);
        void patrol();
        void changeDirection();
        Tile* getTileUnderFeet();

    public:
        Ampton(Player* player);
        void update();
        void draw(SDL_Rect cameraBox);

};

#endif
