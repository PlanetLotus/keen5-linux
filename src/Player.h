// Header file for Player class

#ifndef PLAYER_H
#define PLAYER_H

#include "globals.h"

enum directionEnum {UP, DOWN, LEFT, RIGHT, STOP};
enum animStateEnum {STANDL, STANDR, WALKL, WALKR};

// Animation frames
// TODO: Fix name conflict with animStateEnum
SDL_Rect STANDR0 = {0, 0, TILE_WIDTH, TILE_HEIGHT * 2};
SDL_Rect WALKR1 = {TILE_WIDTH, 0, TILE_WIDTH + 1, TILE_HEIGHT * 2};
SDL_Rect WALKR2 = {TILE_WIDTH * 3, 0, TILE_WIDTH + 3, TILE_HEIGHT * 2};
SDL_Rect WALKR3 = {TILE_WIDTH * 5, 0, TILE_WIDTH, TILE_HEIGHT * 2};
SDL_Rect WALKR4 = {TILE_WIDTH * 7, 0, TILE_WIDTH + 4, TILE_HEIGHT * 2};

class Player {
    private:
        int ammo;
        int xVel;     // Movement speed
        int yVel;     // Movement speed
        int yAccel;

        SDL_Rect hitbox;

        int frame;
        animStateEnum state;
    public:
        Player();
        void shoot();
        void walk(directionEnum dir);
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
        void draw();

        // Getters and setters
        void set_ammo(int x);
        void set_xVel(int x);
        void set_yVel(int y);
        void set_state(animStateEnum x);

        int get_ammo();
        int get_xVel();
        int get_yVel();
        int get_state();

        SDL_Rect get_hitbox();
};

#endif
