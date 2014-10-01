#ifndef SPARKY_H
#define SPARKY_H

#include "Player.h"
#include "SDL.h"

class Sparky : public Sprite {
    private:
        enum stateEnum { PATROL, CHASE, SCAN, CHANGE_DIRECTION, STUNNED };
        stateEnum state;
        stateEnum prevState;
        void changeState(stateEnum nextState);
        void patrol();
        void chase();
        void scan();
        void changeDirection();
        void stunned();

        Tile* getTileUnderFeet();

        enum facingEnum { LEFT = -1, RIGHT = 1 };
        facingEnum facing;
        int patrolSpeed;
        int chaseSpeed;

        int scanCount;
        bool canSeeKeen();
        Player* keen;
        bool isCollidingWithPlayer();

        void fall();
        std::vector<SDL_Rect> anims[5];
        void animate(int nextState, int frametime = FRAMETIME);

    public:
        Sparky(Player* player);
        void update();
        void draw(Texture* texture, SDL_Rect cameraBox);
        void takeShotByPlayer();
};

#endif
