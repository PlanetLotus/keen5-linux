#ifndef SPARKY_H
#define SPARKY_H

#include "Enemy.h"
#include "SDL.h"

class Sparky : public Enemy {
    private:
        enum class State { PATROL, CHASE, SCAN, CHANGE_DIRECTION, STUNNED };
        State state;
        State prevState;
        void changeState(State nextState);
        void patrol();
        void chase();
        void scan();
        void changeDirection();
        void stunned();

        // Values are in pixels per second
        const int patrolSpeed = 96;
        const int chaseSpeedFactor = 2;
        const int fallAccel = 62;
        const int fallVelLimit = 480;
        const int stunHopVel = -288;

        int scanCount;
        bool canSeeKeen();

        void fall();
        std::vector<SDL_Rect> anims[5];
        void animate(int nextState, int frametime = FRAMETIME);

    public:
        Sparky(int spawnX, int spawnY);
        void update();
        void draw(Texture* texture, SDL_Rect cameraBox);
        void takeShotByPlayer();
};

#endif
