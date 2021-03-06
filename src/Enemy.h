#ifndef ENEMY_H
#define ENEMY_H

#include "MovingSprite.h"

class Player;

class Enemy : public MovingSprite {
    protected:
        enum class Facing { LEFT = -1, RIGHT = 1 };
        Facing facing;

        Player* keen;
        bool isCollidingWithPlayer();

        Tile* getTileUnderFeet();

    public:
        virtual void takeShotByPlayer() = 0;
        void setPlayer(Player* player); // Ideally this would be done at instantiation, not later
};

#endif
