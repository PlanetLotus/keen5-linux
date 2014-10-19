#ifndef PLAYER_H
#define PLAYER_H

#include <vector>
#include "Camera.h"
#include "Controller.h"
#include "Platform.h"
#include "SDL.h"
#include "Sprite.h"

// This can't be the right way to do this, but it does work as intended...
extern const int FRAMETIME;
class Platform;

class Player : public Sprite {
    private:
        enum directionEnum { LEFT, RIGHT, UP, DOWN, STOP };

        int xPush;
        int yPush;
        float xAccel;
        float yAccel;

        std::vector<SDL_Rect> anims[34];
        bool isAnimLocked;

        int lookTimer;

        directionEnum facing;
        bool idle;

        bool isOnGround;
        bool isOnPogo;
        bool isOnPole;

        int shootingFrameCount;
        bool isShooting;

        void processKeyboard(); // Call player actions based on keyboard input
        void processUpArrow();
        void processDownArrow();
        void stopWalk();
        void stopClimb();
        void snapToPole(Tile* pole, directionEnum facing);
        Tile* getCollidingPoleTile();
        bool isCollidingWithPlatform(SDL_Rect platformBox);
        bool handlePlatformCollision();
        void animate(int nextState, int frametime = FRAMETIME);
        static Camera& cameraRef;
        static Controller& controllerRef;
        static const std::vector<Platform*>& platformBatchRef;
    public:
        Player();
        void shoot(bool isPressingUp, bool isPressingDown);
        void walk(directionEnum dir);
        void jump();
        void togglePogo();
        void pogo();
        void look(directionEnum dir);
        void climb(directionEnum dir);
        void fall();
        void die(int collidingEnemyX);
        void pushX(int x);
        void pushY(int y);
        void update();
        void draw(Texture* texture, SDL_Rect cameraBox);
        bool getIsOnGround();
        Platform* platformStandingOn;
};

#endif
