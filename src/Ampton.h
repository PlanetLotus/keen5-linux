#ifndef AMPTON_H
#define AMPTON_H

#include "Enemy.h"
#include "globals.h"
#include "SDL.h"

class Ampton : public Enemy {
    private:
        std::vector<SDL_Rect> anims[5];
        void animate(int nextState, int frametime = FRAMETIME);

        enum class State { PATROL, CLIMB_UP, CLIMB_DOWN, CHANGE_DIRECTION, FIX_MACHINE, STUNNED };
        State state;

        int climbCooldownTimer;
        int climbCooldown;

        // Values are in pixels per second
        const int patrolSpeed = 96;
        const int fallAccel = 62;
        const int fallVelLimit = 480;
        const int climbSpeedFactor = 2;
        const int stunHopVel = -288;

        void fall();
        void changeState(State nextState);
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
        Ampton(int spawnX, int spawnY);
        void update();
        void draw(Texture* texture, SDL_Rect cameraBox);

};

#endif
