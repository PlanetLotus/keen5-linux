// Header file for Player class

//#include "Unit.h"

#ifndef PLAYER_H
#define PLAYER_H

enum directionEnum {UP, DOWN, LEFT, RIGHT};
enum animStateEnum {STANDL, STANDR, WALKL, WALKR};

class Player {
    private:
        int ammo;
        int xVel;     // Movement speed
        int yVel;     // Movement speed

        int frameWidth;
        int frameHeight;
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

        int get_frameWidth();
        int get_frameHeight();
};

#endif
