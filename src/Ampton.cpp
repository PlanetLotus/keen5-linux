#include "Ampton.h"
#include "globals.h"
#include "Player.h"
#include "Texture.h"
#include "Tile.h"
#include "Timer.h"

using namespace std;

Ampton::Ampton(int spawnX, int spawnY) {
    hitbox.x = spawnX;
    hitbox.y = spawnY;
    hitbox.w = TILE_WIDTH;
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

    keen = NULL;
    isStunned = false;

    climbCooldownTimer = 0;
    climbCooldown = FRAMES_PER_SECOND / 2;

    SDL_Rect walkL0 = { TILE_WIDTH * 12, TILE_HEIGHT * 4, TILE_WIDTH * 2, TILE_HEIGHT * 2 };
    SDL_Rect walkL1 = { TILE_WIDTH * 14, TILE_HEIGHT * 4, TILE_WIDTH * 2, TILE_HEIGHT * 2 };
    SDL_Rect walkL2 = { TILE_WIDTH * 16, TILE_HEIGHT * 4, TILE_WIDTH * 2, TILE_HEIGHT * 2 };
    SDL_Rect walkL3 = { TILE_WIDTH * 18, TILE_HEIGHT * 4, TILE_WIDTH * 2, TILE_HEIGHT * 2 };

    SDL_Rect walkR0 = { TILE_WIDTH * 12, TILE_HEIGHT * 6, TILE_WIDTH * 2, TILE_HEIGHT * 2 };
    SDL_Rect walkR1 = { TILE_WIDTH * 14, TILE_HEIGHT * 6, TILE_WIDTH * 2, TILE_HEIGHT * 2 };
    SDL_Rect walkR2 = { TILE_WIDTH * 16, TILE_HEIGHT * 6, TILE_WIDTH * 2, TILE_HEIGHT * 2 };
    SDL_Rect walkR3 = { TILE_WIDTH * 18, TILE_HEIGHT * 6, TILE_WIDTH * 2, TILE_HEIGHT * 2 };

    SDL_Rect turn0 = { TILE_WIDTH * 20, TILE_HEIGHT * 4, TILE_WIDTH * 2, TILE_HEIGHT * 2 };

    SDL_Rect climb0 = { TILE_WIDTH * 24, TILE_HEIGHT * 4, TILE_WIDTH * 2, TILE_HEIGHT * 2 };

    SDL_Rect stunned0 = { TILE_WIDTH * 20, TILE_HEIGHT * 6, TILE_WIDTH * 2, TILE_HEIGHT * 2 };
    SDL_Rect stunned1 = { TILE_WIDTH * 22, TILE_HEIGHT * 6, TILE_WIDTH * 2, TILE_HEIGHT * 2 };
    SDL_Rect stunned2 = { TILE_WIDTH * 24, TILE_HEIGHT * 6, TILE_WIDTH * 2, TILE_HEIGHT * 2 };

    SDL_Rect walkLeftArray[4] = { walkL0, walkL1, walkL2, walkL3 };
    SDL_Rect walkRightArray[4] = { walkR0, walkR1, walkR2, walkR3 };
    SDL_Rect turnArray[1] = { turn0 };
    SDL_Rect climbArray[1] = { climb0 };
    SDL_Rect stunnedArray[3] = { stunned0, stunned1, stunned2 };

    vector<SDL_Rect> walkLeftAnim(walkLeftArray, walkLeftArray + sizeof(walkLeftArray) / sizeof(SDL_Rect));
    vector<SDL_Rect> walkRightAnim(walkRightArray, walkRightArray + sizeof(walkRightArray) / sizeof(SDL_Rect));
    vector<SDL_Rect> turnAnim(turnArray, turnArray + sizeof(turnArray) / sizeof(SDL_Rect));
    vector<SDL_Rect> climbAnim(climbArray, climbArray + sizeof(climbArray) / sizeof(SDL_Rect));
    vector<SDL_Rect> stunnedAnim(stunnedArray, stunnedArray + sizeof(stunnedArray) / sizeof(SDL_Rect));

    anims[0] = walkLeftAnim;
    anims[1] = walkRightAnim;
    anims[2] = turnAnim;
    anims[3] = climbAnim;
    anims[4] = stunnedAnim;
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

    // If colliding with pole tile, climb it
    if (climbCooldownTimer > climbCooldown) {
        Tile* pole = getCollidingPoleTile();
        if (pole != NULL) {
            snapToPole(pole);
            changeState(CLIMB_DOWN);
            climbCooldownTimer = 0;
        }
    }

    bool isTBColliding = setYVelIfTBCollision();

    updateVelsWithRemainder();
    updateHitbox();
    updateVelRems();
    resetYVel(isTBColliding);
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

    bool isTBColliding = setYVelIfTBCollision();
    updateVelsWithRemainder();
    updateHitbox();
    updateVelRems();
    resetYVel(isTBColliding);
}

void Ampton::climbUp() {
    if (getCollidingPoleTile() == NULL) {
        changeState(CLIMB_DOWN);
        return;
    }

    xVel = 0;
    xVelRem = 0;
    yVel = patrolSpeed * -2;

    animate(3);

    TileCollisionInfo tciTB = checkTileCollisionTB();

    if (tciTB.isTopColliding()) {
        yVel = (tciTB.tileCollidingWithTop->getBox().y + tciTB.tileCollidingWithTop->getBox().h) - hitbox.y;
        changeState(CLIMB_DOWN);
    }

    updateHitbox();
}

void Ampton::climbDown() {
    xVel = 0;
    xVelRem = 0;
    yVel = patrolSpeed * 2;
    yVelRem = 0;

    animate(3);

    TileCollisionInfo tciTB = checkTileCollisionTB();

    if (tciTB.isBottomColliding()) {
        Tile* tile = tciTB.tileCollidingWithBottom;

        // End of pole
        if (tile->getCollideBottom()) {
            if (climbCooldownTimer > climbCooldown) {
                climbCooldownTimer = 0;
                yVel = tile->getBox().y - (hitbox.y + hitbox.h);
                changeState(PATROL);
            } else {
                // If just starting, climb up instead!
                yVel = tile->getBox().y - (hitbox.y + hitbox.h);
                changeState(CLIMB_UP);
            }
        } else if (climbCooldownTimer > climbCooldown) {
            climbCooldownTimer = 0;
            yVel = tile->getBox().y - (hitbox.y + hitbox.h);
            changeState(PATROL);
        }
    }

    updateHitbox();
}

void Ampton::stunned() {
    animate(4, 3);

    bool isTBColliding = setYVelIfTBCollision();
    updateVelsWithRemainder();
    updateHitbox();
    updateVelRems();
    resetYVel(isTBColliding);
}

void Ampton::takeShotByPlayer() {
    isStunned = true;
    changeState(STUNNED);
    xVel = 0;
    xVelRem = 0;

    // Enemies do a brief "hop" when stunned
    yVel += -12;
}

Tile* Ampton::getCollidingPoleTile() {
    SDL_Rect nextHitbox = getNextHitboxX();
    vector<Tile*> leftTiles = getTilesToLeft();

    for (unsigned int i = 0; i < leftTiles.size(); i++) {
        SDL_Rect tileBox = leftTiles[i]->getBox();
        if (tileBox.y + tileBox.h > hitbox.y && hitbox.y >= tileBox.y &&
            leftTiles[i]->isColliding(Tile::ISPOLE, hitbox, nextHitbox)) {

            SDL_Rect poleBox = leftTiles[i]->getBox();
            int poleRight = poleBox.x + poleBox.w;
            int amptonRight = hitbox.x + hitbox.w;

            // Must be in center of pole
            if (hitbox.x < poleRight - TILE_WIDTH / 4 && amptonRight >= TILE_WIDTH / 4 + poleBox.x) {
                return leftTiles[i];
            } else {
                break;
            }
        }
    }

    // Now do same for rightTiles
    vector<Tile*> rightTiles = getTilesToRight();

    for (unsigned int i = 0; i < rightTiles.size(); i++) {
        SDL_Rect tileBox = rightTiles[i]->getBox();
        if (tileBox.y + tileBox.h > hitbox.y && hitbox.y >= tileBox.y &&
            rightTiles[i]->isColliding(Tile::ISPOLE, hitbox, nextHitbox)) {

            int amptonRight = hitbox.x + hitbox.w;
            SDL_Rect poleBox = rightTiles[i]->getBox();
            int poleRight = poleBox.x + poleBox.w;

            // Must be in center of pole
            if (hitbox.x < poleRight - TILE_WIDTH / 4 && amptonRight >= TILE_WIDTH / 4 + poleBox.x) {
                return rightTiles[i];
            } else {
                return NULL;
            }
        }
    }

    return NULL;
}

void Ampton::snapToPole(Tile* pole) {
    if (pole == NULL)
        return;

    // "Snap" to the pole horizontally, locking movement in x-direction
    if (facing == LEFT)
        xVel = pole->getBox().x - hitbox.x;
    else
        xVel = pole->getBox().x - hitbox.x;
}

void Ampton::changeDirectionIfOnEdge() {
    if (xVel != 0) {
        Tile* tileUnderFeet = getTileUnderFeet();
        if (tileUnderFeet != NULL && tileUnderFeet->getIsEdge()) {
            changeState(CHANGE_DIRECTION);
            xVel = 0;
            xVelRem = 0;
        }
    }
}

bool Ampton::setYVelIfTBCollision() {
    if (yVel != 0) {
        TileCollisionInfo tciTB = checkTileCollisionTB();

        if (tciTB.isTopColliding()) {
            yVel = (tciTB.tileCollidingWithTop->getBox().y + tciTB.tileCollidingWithTop->getBox().h) - hitbox.y;
            return true;
        } else if (tciTB.isBottomColliding()) {
            Tile* tile = tciTB.tileCollidingWithBottom;
            yVel = tile->getBox().y - (hitbox.y + hitbox.h);
            changeDirectionIfOnEdge();
            return true;
        }

        return false;
    }
    return false;
}

void Ampton::updateVelsWithRemainder() {
    xVel += xVelRem;
    // This is a bug. This used to be abs(yVelRem) which unintentionally cast yVelRem into an int.
    yVel += (int)yVelRem;
}

void Ampton::updateHitbox() {
    hitbox.x += xVel;
    hitbox.y += yVel;
}

void Ampton::updateVelRems() {
    // Set fractional part of vel to rem
    double intPart;
    xVelRem = modf(xVel, &intPart);
    yVelRem = modf(yVel, &intPart);
}

void Ampton::resetYVel(bool isTBColliding) {
    // Reset velocity if collision
    if (isTBColliding) {
        yVel = 0;
        yVelRem = 0;
    }
}

void Ampton::update() {
    fall();

    if (state == PATROL)
        patrol();
    else if (state == CHANGE_DIRECTION)
        changeDirection();
    else if (state == CLIMB_UP)
        climbUp();
    else if (state == CLIMB_DOWN)
        climbDown();
    else
        stunned();

    climbCooldownTimer++;

    if (state != STUNNED && isCollidingWithPlayer()) {
        SDL_Rect keenBox = keen->getBox();
        if (state == CLIMB_UP || state == CLIMB_DOWN)
            keen->die(hitbox.x);
        else if (facing == LEFT && hitbox.x > keenBox.x)
            keen->pushX(hitbox.x - (keenBox.x + keenBox.w));
        else if (hitbox.x + hitbox.w < keenBox.x + keenBox.w)
            keen->pushX((hitbox.x + hitbox.w) - keenBox.x);
    }
}

void Ampton::draw(Texture* texture, SDL_Rect cameraBox) {
    // Center the hitbox (horizontally) inside the displayed frame
    int offsetX = srcClip->w / 2 - TILE_WIDTH / 2;
    int destX = hitbox.x - offsetX;

    // Bottom-align the hitbox for tall frames
    int offsetY = srcClip->h - TILE_HEIGHT;
    int destY = hitbox.y - offsetY;

    texture->render(destX - cameraBox.x, destY - cameraBox.y, srcClip);
}
