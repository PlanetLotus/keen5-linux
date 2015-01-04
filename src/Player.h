#ifndef PLAYER_H
#define PLAYER_H

#include <vector>
#include "SDL.h"
#include "MovingSprite.h"

// This can't be the right way to do this, but it does work as intended...
extern const int FRAMETIME;

class Camera;
class Controller;
class Item;
class Platform;
class Tile;

class Player : public MovingSprite {
    private:
        enum class Direction { LEFT, RIGHT, UP, DOWN, STOP };

        int xPush;
        int yPush;
        float xAccel;
        float yAccel;

        // Values are in pixels per second
        const int walkSpeed = 120;
        const int walkSpeedInAir = 40;
        const int dragSpeed = 19;
        const int xVelLimit = 168;
        const int fallAccel = 62;
        const int fallVelLimit = 480;
        const int startJumpVel = -504;
        const int holdJumpVel = -31;
        const int startPogoJumpVel = -48;
        const int hitGroundPogoJumpVel = -576;
        const int holdPogoJumpVel = -36;
        const int poleJumpVel = -240;
        const int poleClimbUpVel = -72;
        const int poleClimbDownVel = 168;
        const int poleCollideGroundFixVel = -48;

        std::vector<SDL_Rect> anims[38];
        bool isAnimLocked;
        std::vector<SDL_Rect*> topAlignedFrames;

        int lookTimer;
        int hangTimer;
        int hangCooldown;

        Direction facing;
        bool idle;

        bool isOnGround;
        bool isOnPogo;
        bool isOnPole;
        bool isJumpingDown;
        bool isHangingLeft;
        bool isHangingRight;

        int shootingFrameCount;
        bool isShooting;

        int rollingFrameCount;
        bool isRolling;

        void shoot(bool isPressingUp, bool isPressingDown);
        void walk(Direction dir);
        void jump();
        void jumpDown();
        void togglePogo();
        void pogo();
        void look(Direction dir);
        void climb(Direction dir);
        void fall();
        void processKeyboard(); // Call player actions based on keyboard input
        void processUpArrow();
        void processDownArrow();
        void stopWalk();
        void stopClimb();
        void snapToPole(Tile* pole, Direction facing);
        Tile* getCollidingPoleTile();
        Tile* getTileUnderFeet();
        bool isCollidingWithPlatform(SDL_Rect platformBox);
        bool handlePlatformCollision();
        void handleLeftLedgeCollision();
        void handleRightLedgeCollision();
        void handleLedgeHanging();
        void checkItemCollision();
        void handleItemCollision(Item* item);
        void rollLeft();
        void rollRight();
        void animate(int nextState, int frametime = FRAMETIME);
        static Camera& cameraRef;
        static Controller& controllerRef;
        static const std::vector<Platform*>& platformBatchRef;
        static std::vector<Item*>& itemBatchRef;
    public:
        Player(int spawnX, int spawnY);
        void die(int collidingEnemyX);
        void pushX(int x);
        void pushY(int y);
        void update();
        void draw(Texture* texture, SDL_Rect cameraBox);
        bool getIsOnGround();
        Platform* platformStandingOn;
};

#endif
