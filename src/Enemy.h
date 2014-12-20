#ifndef ENEMY_H
#define ENEMY_H

#include "Sprite.h"

class Player;

class Enemy : public Sprite {
    protected:
        enum facingEnum { LEFT = -1, RIGHT = 1 };
        facingEnum facing;

        Player* keen;
        bool isCollidingWithPlayer();

        Tile* getTileUnderFeet();

    public:
        virtual void takeShotByPlayer() = 0;
        void setPlayer(Player* player); // Ideally this would be done at instantiation, not later
};

#endif
