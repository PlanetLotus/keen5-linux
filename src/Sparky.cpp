#include "globals.h"
#include "Sparky.h"

using namespace std;

Sparky::Sparky() {
    hitbox.x = TILE_WIDTH * 24;
    hitbox.y = TILE_HEIGHT * 4;
    hitbox.w = TILE_WIDTH * 2;
    hitbox.h = TILE_HEIGHT * 2;

    facing = RIGHT;
    patrolSpeed = 4;
    xVel = patrolSpeed * facing;
    yVel = 0;

    isStunned = false;

    state = PATROL;

    frame = 0;
    animState = 0;

    srcClip = NULL;

    SDL_Rect walkL0 = { TILE_WIDTH * 14, 0, TILE_WIDTH * 2, TILE_HEIGHT * 2 };
    SDL_Rect walkL1 = { TILE_WIDTH * 16, 0, TILE_WIDTH * 2, TILE_HEIGHT * 2 };
    SDL_Rect walkL2 = { TILE_WIDTH * 18, 0, TILE_WIDTH * 2, TILE_HEIGHT * 2 };
    SDL_Rect walkL3 = { TILE_WIDTH * 20, 0, TILE_WIDTH * 2, TILE_HEIGHT * 2 };

    SDL_Rect walkR0 = { TILE_WIDTH * 14, TILE_HEIGHT * 2, TILE_WIDTH * 2, TILE_HEIGHT * 2 };
    SDL_Rect walkR1 = { TILE_WIDTH * 16, TILE_HEIGHT * 2, TILE_WIDTH * 2, TILE_HEIGHT * 2 };
    SDL_Rect walkR2 = { TILE_WIDTH * 18, TILE_HEIGHT * 2, TILE_WIDTH * 2, TILE_HEIGHT * 2 };
    SDL_Rect walkR3 = { TILE_WIDTH * 20, TILE_HEIGHT * 2, TILE_WIDTH * 2, TILE_HEIGHT * 2 };

    SDL_Rect turn0 = { TILE_WIDTH * 22, 0, TILE_WIDTH * 2, TILE_HEIGHT * 2 };
    SDL_Rect turn1 = { TILE_WIDTH * 24, 0, TILE_WIDTH * 2, TILE_HEIGHT * 2 };
    SDL_Rect turn2 = { TILE_WIDTH * 26, 0, TILE_WIDTH * 2, TILE_HEIGHT * 2 };

    SDL_Rect die0 = { TILE_WIDTH * 19, TILE_HEIGHT * 2, TILE_WIDTH * 2, TILE_HEIGHT * 2 };
    SDL_Rect die1 = { TILE_WIDTH * 21, TILE_HEIGHT * 2, TILE_WIDTH * 2, TILE_HEIGHT * 2 };
    SDL_Rect die2 = { TILE_WIDTH * 23, TILE_HEIGHT * 2, TILE_WIDTH * 2, TILE_HEIGHT * 2 };

    SDL_Rect walkLeftArray[4] = { walkL0, walkL1, walkL2, walkL3 };
    SDL_Rect walkRightArray[4] = { walkR0, walkR1, walkR2, walkR3 };
    SDL_Rect turnLeftArray[3] = { turn2, turn1, turn0 };
    SDL_Rect turnRightArray[3] = { turn0, turn1, turn2 };
    SDL_Rect dieArray[3] = { die0, die1, die2 };

    vector<SDL_Rect> walkLeftAnim(walkLeftArray, walkLeftArray + sizeof(walkLeftArray) / sizeof(SDL_Rect));
    vector<SDL_Rect> walkRightAnim(walkRightArray, walkRightArray + sizeof(walkRightArray) / sizeof(SDL_Rect));
    vector<SDL_Rect> turnLeftAnim(turnLeftArray, turnLeftArray + sizeof(turnLeftArray) / sizeof(SDL_Rect));
    vector<SDL_Rect> turnRightAnim(turnRightArray, turnRightArray + sizeof(turnRightArray) / sizeof(SDL_Rect));
    vector<SDL_Rect> dieAnim(dieArray, dieArray + sizeof(dieArray) / sizeof(SDL_Rect));

    anims[0] = walkLeftAnim;
    anims[1] = walkRightAnim;
    anims[2] = turnLeftAnim;
    anims[3] = turnRightAnim;
    anims[4] = dieAnim;
}

void Sparky::animate(int nextState, int frametime) {
    if (animState == nextState) {
        // Get next frame
        frame++;
    } else {
        // Start new animation
        frame = 0;
        animState = nextState;
    }

    if (frame == anims[animState].size() * frametime)
        frame = 0;

    srcClip = &anims[animState][frame / frametime];
}

void Sparky::takeShotByPlayer() {
    isStunned = true;
    state = STUNNED;
    xVel = 0;
    xVelRem = 0;

    // Enemies do a brief "hop" when stunned
    yVel += -12;
}

void Sparky::fall() {
    if (yVel >= 20)
        return;

    yVel += 2.6;
}

void Sparky::patrol() {
    xVel = patrolSpeed * facing;
    TileCollisionInfo tciLR;

    animate(0, 3);

    // Check left/right collision
    tciLR = checkTileCollisionLR();

    if (tciLR.isLeftColliding() || tciLR.isRightColliding()) {
        state = CHANGE_DIRECTION;
        xVel = 0;
        xVelRem = 0;
    }

    // TODO: Look for Keen and switch to chase if detected
}

void Sparky::chase() {
    xVel = patrolSpeed * 2 * facing;
    TileCollisionInfo tciLR;

    animate(0, 3);

    // Check left/right collision
    tciLR = checkTileCollisionLR();

    if (tciLR.isLeftColliding() || tciLR.isRightColliding()) {
        state = CHANGE_DIRECTION;
        xVel = 0;
        xVelRem = 0;
    }
}

void Sparky::changeDirection() {
    // First, finish animating the direction change before moving

    // Then, invert velocity
    facing = facing == LEFT ? RIGHT : LEFT;
    state = PATROL;
}

void Sparky::stunned() {
    animate(4, 3);
}

void Sparky::update() {
    fall();

    if (state == PATROL)
        patrol();
    else if (state == CHASE)
        chase();
    else if (state == CHANGE_DIRECTION)
        changeDirection();
    else
        stunned();

    TileCollisionInfo tciTB;

    // Check top/bottom collision
    if (yVel != 0) {
        tciTB = checkTileCollisionTB();

        if (tciTB.isTopColliding()) {
            yVel = (tciTB.tileCollidingWithTop->getBox().y + tciTB.tileCollidingWithTop->getBox().h) - hitbox.y;
        } else if (tciTB.isBottomColliding()) {
            Tile* tile = tciTB.tileCollidingWithBottom;
            yVel = tile->getBox().y - (hitbox.y + hitbox.h);
        }
    }

    // Add back remainder
    xVel += xVelRem;
    yVel += abs(yVelRem);

    hitbox.x += xVel;
    hitbox.y += yVel;

    // Set fractional part of vel to rem
    double intPart;
    xVelRem = modf(xVel, &intPart);
    yVelRem = modf(yVel, &intPart);

    // Reset velocity if collision or on pole
    if (tciTB.isTopColliding() || tciTB.isBottomColliding()) {
        yVel = 0;
        yVelRem = 0;
    }
}

void Sparky::draw(SDL_Rect cameraBox) {
    gKeenTexture->render(hitbox.x - cameraBox.x, hitbox.y - cameraBox.y, srcClip);
}
