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

        std::vector<SDL_Rect> anims[38];
        bool isAnimLocked;
        std::vector<SDL_Rect*> topAlignedFrames;

        int lookTimer;

        directionEnum facing;
        bool idle;

        bool isOnGround;
        bool isOnPogo;
        bool isOnPole;
        bool isJumpingDown;
        bool isHangingLeft;
        bool isHangingRight;

        int shootingFrameCount;
        bool isShooting;

        void shoot(bool isPressingUp, bool isPressingDown);
        void walk(directionEnum dir);
        void jump();
        void jumpDown();
        void togglePogo();
        void pogo();
        void look(directionEnum dir);
        void climb(directionEnum dir);
        void fall();
        void processKeyboard(); // Call player actions based on keyboard input
        void processUpArrow();
        void processDownArrow();
        void stopWalk();
        void stopClimb();
        void snapToPole(Tile* pole, directionEnum facing);
        Tile* getCollidingPoleTile();
        Tile* getTileUnderFeet();
        bool isCollidingWithPlatform(SDL_Rect platformBox);
        bool handlePlatformCollision();
        void handleLeftLedgeCollision();
        void handleRightLedgeCollision();
        void animate(int nextState, int frametime = FRAMETIME);
        static Camera& cameraRef;
        static Controller& controllerRef;
        static const std::vector<Platform*>& platformBatchRef;
    public:
        Player();
        void die(int collidingEnemyX);
        void pushX(int x);
        void pushY(int y);
        void update();
        void draw(Texture* texture, SDL_Rect cameraBox);
        bool getIsOnGround();
        Platform* platformStandingOn;
};

#endif
