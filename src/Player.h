#ifndef PLAYER_H
#define PLAYER_H

#include <vector>
#include "SDL.h"
#include "Sprite.h"

class Player : public Sprite {
    private:
        enum directionEnum {LEFT, RIGHT, UP, DOWN, STOP};

        float xAccel;
        float yAccel;

        unsigned int frame;
        int animState;
        std::vector<SDL_Rect> anims[30];
        bool isAnimLocked;

        directionEnum facing;
        bool idle;

        bool isOnGround;
        bool isOnPogo;
        bool isOnPole;

        int shootingFrameCount;
        bool isShooting;

        void processKeyboard(); // Call player actions based on keyboard input
        void processUpArrow();
        void processDownArrow();
        void stopwalk();
        void stopClimb();
        void snapToPole(Tile* pole, directionEnum facing);
        Tile* GetCollidingPoleTile();
    public:
        Player();
        void shoot(bool isPressingUp, bool isPressingDown);
        void walk(directionEnum dir);
        void jump();
        void togglePogo();
        void pogo();
        void look(directionEnum dir);
        void climb(directionEnum dir);
        void enter_door();
        void fall();
        void update();
        void animate(int nextState, int frametime = FRAMETIME);
        void draw();
};

#endif
