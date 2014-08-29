#ifndef PLAYER_H
#define PLAYER_H

#include <vector>
#include "SDL.h"
#include "Sprite.h"

class Player : public Sprite {
    private:
        enum directionEnum { LEFT, RIGHT, UP, DOWN, STOP };

        float xAccel;
        float yAccel;

        std::vector<SDL_Rect> anims[32];
        bool isAnimLocked;

        int lookTimer;

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
        void stopWalk();
        void stopClimb();
        void snapToPole(Tile* pole, directionEnum facing);
        Tile* getCollidingPoleTile();
        void animate(int nextState, int frametime = FRAMETIME);
        void die();
    public:
        Player();
        void shoot(bool isPressingUp, bool isPressingDown);
        void walk(directionEnum dir);
        void jump();
        void togglePogo();
        void pogo();
        void look(directionEnum dir);
        void climb(directionEnum dir);
        void fall();
        void update();
        void draw(SDL_Rect cameraBox);
        bool getIsOnGround();
};

#endif
