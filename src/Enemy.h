#ifndef ENEMY_H
#define ENEMY_H

#include "Player.h"
#include "Sprite.h"

class Enemy : public Sprite {
    protected:
        enum facingEnum { LEFT = -1, RIGHT = 1 };
        facingEnum facing;

        Player* keen;
        bool isCollidingWithPlayer();

        Tile* getTileUnderFeet();
};

#endif
