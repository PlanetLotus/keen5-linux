#ifndef PLAYER_H
#define PLAYER_H

#include <vector>
#include "SDL.h"
#include "Sprite.h"

enum directionEnum {LEFT, RIGHT, UP, DOWN, STOP};

class Player : public Sprite {
    private:
        int ammo;
        int xAccel;
        int yAccel;

        unsigned int frame;
        int animState;
        std::vector<SDL_Rect> anims[21];
        bool isAnimLocked;

        directionEnum facing;
        bool idle;

        bool isOnGround;

        int shootingFrameCount;
        bool isShooting;
    public:
        Player();
        void shoot(bool isPressingUp, bool isPressingDown);
        void walk(directionEnum dir);
        void stopwalk();
        void jump();
        void pogo();
        void look(directionEnum dir);
        void climb(directionEnum dir);
        void enter_door();
        void fall();
        bool CheckBottomCollision(int minCol, int maxCol, SDL_Rect nextHitbox);
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
