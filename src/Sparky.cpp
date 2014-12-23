#include "globals.h"
#include "Player.h"
#include "Sparky.h"
#include "Texture.h"
#include "Tile.h"

using namespace std;

Sparky::Sparky(int spawnX, int spawnY) {
    hitbox.x = spawnX;
    hitbox.y = spawnY;
    hitbox.w = TILE_WIDTH * 2;
    hitbox.h = TILE_HEIGHT * 2;

    facing = Facing::LEFT;
    patrolSpeed = 4;
    chaseSpeed = patrolSpeed * 2;
    xVel = patrolSpeed * (int)facing;
    yVel = 0;
    xVelRem = 0;
    yVelRem = 0;

    scanCount = 0;
    keen = NULL;

    isStunned = false;

    state = State::PATROL;
    prevState = State::PATROL;

    frame = 0;
    animState = 0;

    srcClip = NULL;

    vector<SDL_Rect> walkLeftAnim = {
        { TILE_WIDTH * 14, 0, TILE_WIDTH * 2, TILE_HEIGHT * 2 },
        { TILE_WIDTH * 16, 0, TILE_WIDTH * 2, TILE_HEIGHT * 2 },
        { TILE_WIDTH * 18, 0, TILE_WIDTH * 2, TILE_HEIGHT * 2 },
        { TILE_WIDTH * 20, 0, TILE_WIDTH * 2, TILE_HEIGHT * 2 }
    };
    vector<SDL_Rect> walkRightAnim = {
        { TILE_WIDTH * 14, TILE_HEIGHT * 2, TILE_WIDTH * 2, TILE_HEIGHT * 2 },
        { TILE_WIDTH * 16, TILE_HEIGHT * 2, TILE_WIDTH * 2, TILE_HEIGHT * 2 },
        { TILE_WIDTH * 18, TILE_HEIGHT * 2, TILE_WIDTH * 2, TILE_HEIGHT * 2 },
        { TILE_WIDTH * 20, TILE_HEIGHT * 2, TILE_WIDTH * 2, TILE_HEIGHT * 2 }
    };
    vector<SDL_Rect> turnLeftAnim = {
        { TILE_WIDTH * 22, 0, TILE_WIDTH * 2, TILE_HEIGHT * 2 },
        { TILE_WIDTH * 24, 0, TILE_WIDTH * 2, TILE_HEIGHT * 2 },
        { TILE_WIDTH * 26, 0, TILE_WIDTH * 2, TILE_HEIGHT * 2 }
    };
    vector<SDL_Rect> turnRightAnim = {
        { TILE_WIDTH * 26, 0, TILE_WIDTH * 2, TILE_HEIGHT * 2 },
        { TILE_WIDTH * 24, 0, TILE_WIDTH * 2, TILE_HEIGHT * 2 },
        { TILE_WIDTH * 22, 0, TILE_WIDTH * 2, TILE_HEIGHT * 2 }
    };
    vector<SDL_Rect> dieAnim = {
        { TILE_WIDTH * 22, TILE_HEIGHT * 2, TILE_WIDTH * 2, TILE_HEIGHT * 2 },
        { TILE_WIDTH * 24, TILE_HEIGHT * 2, TILE_WIDTH * 2, TILE_HEIGHT * 2 },
        { TILE_WIDTH * 26, TILE_HEIGHT * 2, TILE_WIDTH * 2, TILE_HEIGHT * 2 }
    };

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
    changeState(State::STUNNED);
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
    xVel = patrolSpeed * (int)facing;
    TileCollisionInfo tciLR;

    if (facing == Facing::LEFT)
        animate(0, 3);
    else
        animate(1, 3);

    // Check left/right collision
    tciLR = checkTileCollisionLR();

    if (tciLR.isLeftColliding() || tciLR.isRightColliding()) {
        changeState(State::CHANGE_DIRECTION);
        xVel = 0;
        xVelRem = 0;
    }

    // Randomly scan for Keen
    if (rand() % 100 == 0) {
        changeState(State::SCAN);
        xVel = 0;
        xVelRem = 0;
    }
}

void Sparky::chase() {
    xVel = chaseSpeed * (int)facing;
    TileCollisionInfo tciLR;

    if (facing == Facing::LEFT)
        animate(0, 3);
    else
        animate(1, 3);

    // Check left/right collision
    tciLR = checkTileCollisionLR();

    if (tciLR.isLeftColliding() || tciLR.isRightColliding()) {
        changeState(State::CHANGE_DIRECTION);
        changeState(State::PATROL);
        xVel = 0;
        xVelRem = 0;
    }
}

void Sparky::scan() {
    // Scan opposite way, look for Keen, chase if found
    // Else, scan other way, look for Keen, chase if found
    // Else, stop scanning

    if (scanCount == 0) {
        changeState(State::CHANGE_DIRECTION);
        scanCount++;
    } else if (scanCount == 1) {
        if (canSeeKeen()) {
            changeState(State::CHASE);
            scanCount = 0;
        } else {
            changeState(State::CHANGE_DIRECTION);
            scanCount++;
        }
    } else {
        if (canSeeKeen()) {
            changeState(State::CHASE);
        } else {
            changeState(State::PATROL);
        }
        scanCount = 0;
    }
}

void Sparky::changeDirection() {
    // First, finish animating the direction change before moving
    int frametime = 4;
    if (facing == Facing::LEFT)
        animate(3, frametime);
    else
        animate(2, frametime);

    // Then, invert velocity
    if (frame == anims[animState].size() * frametime - 1) {
        facing = facing == Facing::LEFT ? Facing::RIGHT : Facing::LEFT;
        changeState(prevState);
    }
}

void Sparky::stunned() {
    animate(4, 3);
}

bool Sparky::canSeeKeen() {
    SDL_Rect keenHitbox = keen->getBox();

    if (facing == Facing::LEFT && keenHitbox.x < hitbox.x) {
        if (keenHitbox.y > hitbox.y - TILE_HEIGHT && keenHitbox.y < hitbox.y + TILE_HEIGHT)
            return true;
    } else if (facing == Facing::RIGHT && keenHitbox.x > hitbox.x) {
        if (keenHitbox.y > hitbox.y - TILE_HEIGHT && keenHitbox.y < hitbox.y + TILE_HEIGHT)
            return true;
    }

    return false;
}

void Sparky::changeState(State nextState) {
    prevState = state;
    state = nextState;
}

void Sparky::update() {
    fall();

    if (state == State::PATROL)
        patrol();
    else if (state == State::CHASE)
        chase();
    else if (state == State::SCAN)
        scan();
    else if (state == State::CHANGE_DIRECTION)
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

            if (xVel != 0) {
                Tile* tileUnderFeet = getTileUnderFeet();
                if (tileUnderFeet != NULL && tileUnderFeet->getIsEdge()) {
                    changeState(State::CHANGE_DIRECTION);
                    xVel = 0;
                    xVelRem = 0;
                }
            }
        }
    }

    // Add back remainder
    xVel += xVelRem;
    // This is a bug. This used to be abs(yVelRem) which unintentionally cast yVelRem into an int.
    yVel += (int)yVelRem;

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

    if (state != State::STUNNED && isCollidingWithPlayer())
        keen->die(hitbox.x);
}

void Sparky::draw(Texture* texture, SDL_Rect cameraBox) {
    texture->render(hitbox.x - cameraBox.x, hitbox.y - cameraBox.y, srcClip);
}
