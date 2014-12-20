#ifndef AMPTON_H
#define AMPTON_H

#include "Enemy.h"
#include "globals.h"
#include "SDL.h"

class Player;

class Ampton : public Enemy {
    private:
        std::vector<SDL_Rect> anims[5];
        void animate(int nextState, int frametime = FRAMETIME);

        int patrolSpeed;

        enum stateEnum { PATROL, CLIMB_UP, CLIMB_DOWN, CHANGE_DIRECTION, FIX_MACHINE, STUNNED };
        stateEnum state;

        int climbCooldownTimer;
        int climbCooldown;

        void fall();
        void changeState(stateEnum nextState);
        void patrol();
        void changeDirection();
        void climbUp();
        void climbDown();
        void stunned();
        void takeShotByPlayer();
        Tile* getCollidingPoleTile();
        void snapToPole(Tile* pole);
        void changeDirectionIfOnEdge();
        bool setYVelIfTBCollision();
        void updateVelsWithRemainder();
        void updateHitbox();
        void updateVelRems();
        void resetYVel(bool isTBColliding);

    public:
        Ampton(Player* player, int spawnX, int spawnY);
        void update();
        void draw(Texture* texture, SDL_Rect cameraBox);

};

#endif
