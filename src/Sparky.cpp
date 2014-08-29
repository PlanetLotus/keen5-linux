#include "globals.h"
#include "Sparky.h"

using namespace std;

Sparky::Sparky(Player* player) {
    hitbox.x = TILE_WIDTH * 14;
    hitbox.y = TILE_HEIGHT * 9;
    hitbox.w = TILE_WIDTH * 2;
    hitbox.h = TILE_HEIGHT * 2;

    facing = LEFT;
    patrolSpeed = 4;
    chaseSpeed = patrolSpeed * 2;
    xVel = patrolSpeed * facing;
    yVel = 0;

    scanCount = 0;
    keen = player;

    isStunned = false;

    state = PATROL;
    prevState = PATROL;

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

    SDL_Rect die0 = { TILE_WIDTH * 22, TILE_HEIGHT * 2, TILE_WIDTH * 2, TILE_HEIGHT * 2 };
    SDL_Rect die1 = { TILE_WIDTH * 24, TILE_HEIGHT * 2, TILE_WIDTH * 2, TILE_HEIGHT * 2 };
    SDL_Rect die2 = { TILE_WIDTH * 26, TILE_HEIGHT * 2, TILE_WIDTH * 2, TILE_HEIGHT * 2 };

    SDL_Rect walkLeftArray[4] = { walkL0, walkL1, walkL2, walkL3 };
    SDL_Rect walkRightArray[4] = { walkR0, walkR1, walkR2, walkR3 };
    SDL_Rect turnLeftArray[4] = { turn2, turn1, turn0, walkL0 };
    SDL_Rect turnRightArray[4] = { turn0, turn1, turn2, walkR0 };
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
    changeState(STUNNED);
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

    // Randomly scan for Keen
    /*
    if (rand() % 100 == 0) {
        changeState(SCAN);
        xVel = 0;
        xVelRem = 0;
    }
    */
}

void Sparky::chase() {
    xVel = chaseSpeed * facing;
    TileCollisionInfo tciLR;

    if (facing == LEFT)
        animate(0, 3);
    else
        animate(1, 3);

    // Check left/right collision
    tciLR = checkTileCollisionLR();

    if (tciLR.isLeftColliding() || tciLR.isRightColliding()) {
        changeState(CHANGE_DIRECTION);
        changeState(PATROL);
        xVel = 0;
        xVelRem = 0;
    }
}

void Sparky::scan() {
    // Scan opposite way, look for Keen, chase if found
    // Else, scan other way, look for Keen, chase if found
    // Else, stop scanning

    if (scanCount == 0) {
        changeState(CHANGE_DIRECTION);
        scanCount++;
    } else if (scanCount == 1) {
        if (canSeeKeen()) {
            changeState(CHASE);
            scanCount = 0;
        } else {
            changeState(CHANGE_DIRECTION);
            scanCount++;
        }
    } else {
        if (canSeeKeen()) {
            changeState(CHASE);
        } else {
            changeState(prevState);
        }
        scanCount = 0;
    }
}

void Sparky::changeDirection() {
    // First, finish animating the direction change before moving
    int frametime = 4;
    if (facing == LEFT)
        animate(3, frametime);
    else
        animate(2, frametime);

    // Then, invert velocity
    if (frame == anims[animState].size() * frametime - 1) {
        facing = facing == LEFT ? RIGHT : LEFT;
        changeState(prevState);
    }
}

void Sparky::stunned() {
    animate(4, 3);
}

bool Sparky::canSeeKeen() {
    SDL_Rect keenHitbox = keen->getBox();

    if (facing == LEFT && keenHitbox.x < hitbox.x) {
        if (keenHitbox.y > hitbox.y - TILE_HEIGHT && keenHitbox.y < hitbox.y + TILE_HEIGHT)
            return true;
    } else if (facing == RIGHT && keenHitbox.x > hitbox.x) {
        if (keenHitbox.y > hitbox.y - TILE_HEIGHT && keenHitbox.y < hitbox.y + TILE_HEIGHT)
            return true;
    }

    return false;
}

void Sparky::changeState(stateEnum nextState) {
    prevState = state;
    state = nextState;
}

Tile* Sparky::getTileUnderFeet() {
    int sparkyBottom = hitbox.y + hitbox.h;

    if (sparkyBottom % TILE_HEIGHT != 0)
        return NULL;

    int sparkyRight = hitbox.x + hitbox.w;
    unsigned int leftCol = hitbox.x / TILE_WIDTH;
    unsigned int rightCol = (sparkyRight + TILE_WIDTH) / TILE_WIDTH;
    int tileRow = sparkyBottom / TILE_HEIGHT;
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

void Sparky::update() {
    fall();

    if (state == PATROL)
        patrol();
    else if (state == CHASE)
        chase();
    else if (state == SCAN)
        scan();
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
