#ifndef PLAYER_H
#define PLAYER_H

#include "SDL.h"
#include "Sprite.h"
#include <vector>

enum directionEnum {LEFT, RIGHT, UP, DOWN, STOP};

class Player : public Sprite {
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
        std::vector<SDL_Rect> anims[14];

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
