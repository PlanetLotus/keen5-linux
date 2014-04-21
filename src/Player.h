#ifndef PLAYER_H
#define PLAYER_H

#include "globals.h"

enum directionEnum {LEFT, RIGHT, UP, DOWN, STOP};

using namespace std;

class Player {
    private:
        int ammo;
        int xVel;
        int yVel;
        int xAccel;
        int yAccel;

        SDL_Rect* srcClip;
        SDL_Rect hitbox;

        unsigned int frame;
        int animState;
        directionEnum facing;
        bool idle;

        bool isOnGround;

        int shootingFrameCount;
        bool isShooting;
    public:
        Player();
        void shoot();
        void walk(directionEnum dir);
        void stopwalk();
        void jump();
        void pogo();
        void look(directionEnum dir);
        void climb(directionEnum dir);
        void enter_door();
        void fall();
        bool IsTopColliding(SDL_Rect a, SDL_Rect b);
        bool IsBottomColliding(SDL_Rect a, SDL_Rect b);
        bool IsLeftColliding(SDL_Rect a, SDL_Rect b);
        bool IsRightColliding(SDL_Rect a, SDL_Rect b);
        void CheckTopCollision();
        void CheckBottomCollision();
        void CheckLeftCollision();
        void CheckRightCollision();
        bool IsCollidingWithTiles();
        void update();
        void animate(int nextState);
        void draw();

        // Getters and setters
        void set_ammo(int x);
        void set_xVel(int x);
        void set_yVel(int y);
        void set_state(int x);

        int get_ammo();
        int get_xVel();
        int get_yVel();
        int get_state();

        SDL_Rect get_hitbox();
};

#endif
