#ifndef PLAYER_H
#define PLAYER_H

#include <vector>
#include "SDL.h"
#include "Sprite.h"

enum directionEnum {LEFT, RIGHT, UP, DOWN, STOP};

class Player : public Sprite {
    private:
        float xAccel;
        float yAccel;

        unsigned int frame;
        int animState;
        std::vector<SDL_Rect> anims[21];
        bool isAnimLocked;

        directionEnum facing;
        bool idle;

        bool isOnGround;
        bool isOnPogo;
        bool isOnPole;

        int shootingFrameCount;
        bool isShooting;

        void processKeyboard(); // Call player actions based on keyboard input
    public:
        Player();
        void shoot(bool isPressingUp, bool isPressingDown);
        void walk(directionEnum dir);
        void stopwalk();
        void jump();
        void togglePogo();
        void pogo();
        void look(directionEnum dir);
        void processUpArrow();
        void climb(directionEnum dir);
        void enter_door();
        void fall();
        void update();
        void animate(int nextState);
        void draw();
};

#endif
