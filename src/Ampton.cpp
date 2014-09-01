#include "Ampton.h"
#include "globals.h"

using namespace std;

Ampton::Ampton(Player* player) {
    hitbox.x = TILE_WIDTH * 26;
    hitbox.y = TILE_HEIGHT * 3;
    hitbox.w = TILE_WIDTH * 2;
    hitbox.h = TILE_HEIGHT;

    facing = LEFT;
    patrolSpeed = 4;
    xVel = patrolSpeed * facing;
    yVel = 0;
    xVelRem = 0;
    yVelRem = 0;

    state = PATROL;
    frame = 0;
    animState = 0;
    srcClip = NULL;

    keen = player;
    isStunned = false;

    SDL_Rect walkL0 = { TILE_WIDTH * 12, TILE_HEIGHT * 4, TILE_WIDTH * 2, TILE_HEIGHT * 2 };
    SDL_Rect walkL1 = { TILE_WIDTH * 14, TILE_HEIGHT * 4, TILE_WIDTH * 2, TILE_HEIGHT * 2 };
    SDL_Rect walkL2 = { TILE_WIDTH * 16, TILE_HEIGHT * 4, TILE_WIDTH * 2, TILE_HEIGHT * 2 };
    SDL_Rect walkL3 = { TILE_WIDTH * 18, TILE_HEIGHT * 4, TILE_WIDTH * 2, TILE_HEIGHT * 2 };

    SDL_Rect walkR0 = { TILE_WIDTH * 12, TILE_HEIGHT * 6, TILE_WIDTH * 2, TILE_HEIGHT * 2 };
    SDL_Rect walkR1 = { TILE_WIDTH * 14, TILE_HEIGHT * 6, TILE_WIDTH * 2, TILE_HEIGHT * 2 };
    SDL_Rect walkR2 = { TILE_WIDTH * 16, TILE_HEIGHT * 6, TILE_WIDTH * 2, TILE_HEIGHT * 2 };
    SDL_Rect walkR3 = { TILE_WIDTH * 18, TILE_HEIGHT * 6, TILE_WIDTH * 2, TILE_HEIGHT * 2 };

    SDL_Rect turn0 = { TILE_WIDTH * 20, TILE_HEIGHT * 4, TILE_WIDTH * 2, TILE_HEIGHT * 2 };

    SDL_Rect stunned0 = { TILE_WIDTH * 20, TILE_HEIGHT * 6, TILE_WIDTH * 2, TILE_HEIGHT * 2 };
    SDL_Rect stunned1 = { TILE_WIDTH * 22, TILE_HEIGHT * 6, TILE_WIDTH * 2, TILE_HEIGHT * 2 };
    SDL_Rect stunned2 = { TILE_WIDTH * 24, TILE_HEIGHT * 6, TILE_WIDTH * 2, TILE_HEIGHT * 2 };

    SDL_Rect walkLeftArray[4] = { walkL0, walkL1, walkL2, walkL3 };
    SDL_Rect walkRightArray[4] = { walkR0, walkR1, walkR2, walkR3 };
    SDL_Rect turnArray[1] = { turn0 };
    SDL_Rect stunnedArray[3] = { stunned0, stunned1, stunned2 };

    vector<SDL_Rect> walkLeftAnim(walkLeftArray, walkLeftArray + sizeof(walkLeftArray) / sizeof(SDL_Rect));
    vector<SDL_Rect> walkRightAnim(walkRightArray, walkRightArray + sizeof(walkRightArray) / sizeof(SDL_Rect));
    vector<SDL_Rect> turnAnim(turnArray, turnArray + sizeof(turnArray) / sizeof(SDL_Rect));
    vector<SDL_Rect> stunnedAnim(stunnedArray, stunnedArray + sizeof(stunnedArray) / sizeof(SDL_Rect));

    anims[0] = walkLeftAnim;
    anims[1] = walkRightAnim;
    anims[2] = turnAnim;
    anims[3] = stunnedAnim;
}

void Ampton::animate(int nextState, int frametime) {
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

void Ampton::fall() {
    if (yVel >= 20)
        return;

    yVel += 2.6;
}

void Ampton::changeState(stateEnum nextState) {
    state = nextState;
}

void Ampton::patrol() {
    xVel = patrolSpeed * facing;
    TileCollisionInfo tciLR;

    if (facing == LEFT)
        animate(0, 3);
    else
        animate(1, 3);

    // Check left/right collision
    tciLR = checkTileCollisionLR();

    if (tciLR.isLeftColliding() || tciLR.isRightColliding()) {
        changeState(CHANGE_DIRECTION);
        xVel = 0;
        xVelRem = 0;
    }
}

void Ampton::changeDirection() {
    // First, finish animating the direction change before moving
    int frametime = 3;
    animate(2, frametime);

    // Then, invert velocity
    if (frame == anims[animState].size() * frametime - 1) {
        facing = facing == LEFT ? RIGHT : LEFT;
        changeState(PATROL);
    }
}

void Ampton::stunned() {
    animate(3, 3);
}

void Ampton::takeShotByPlayer() {
    isStunned = true;
    changeState(STUNNED);
    xVel = 0;
    xVelRem = 0;

    // Enemies do a brief "hop" when stunned
    yVel += -12;
}

Tile* Ampton::getTileUnderFeet() {
    int amptonBottom = hitbox.y + hitbox.h;

    if (amptonBottom % TILE_HEIGHT != 0)
        return NULL;

    int amptonRight = hitbox.x + hitbox.w;
    unsigned int leftCol = hitbox.x / TILE_WIDTH;
    unsigned int rightCol = (amptonRight + TILE_WIDTH) / TILE_WIDTH;
    int tileRow = amptonBottom / TILE_HEIGHT;
    Tile* tile = NULL;

    // If moving left, return leftmost tile. If moving right, return rightmost
    // If the unit is pushed by something else, will that mess up this logic?
    for (unsigned int i = leftCol; i < rightCol; i++) {
        tile = gTiles[i][tileRow];
        if (tile != NULL) {
            if (facing == LEFT)
                return tile;
        }
    }
    return tile;
}

void Ampton::update() {
    fall();

    if (state == PATROL)
        patrol();
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

            if (xVel != 0) {
                Tile* tileUnderFeet = getTileUnderFeet();
                if (tileUnderFeet != NULL && tileUnderFeet->getIsEdge()) {
                    changeState(CHANGE_DIRECTION);
                    xVel = 0;
                    xVelRem = 0;
                }
            }
        }
    }

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

void Ampton::draw(SDL_Rect cameraBox) {
    // Bottom-align the hitbox for tall frames
    int offsetY = srcClip->h - TILE_HEIGHT;
    int destY = hitbox.y - offsetY;

    gKeenTexture->render(hitbox.x - cameraBox.x, destY - cameraBox.y, srcClip);
}
