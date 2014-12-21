#include <algorithm>
#include "BlasterShot.h"
#include "Camera.h"
#include "Controller.h"
#include "globals.h"
#include "helpers.h"
#include "Item.h"
#include "Platform.h"
#include "Player.h"
#include "Texture.h"
#include "Tile.h"
#include "Timer.h"

using namespace std;

void Player::shoot(bool isPressingUp, bool isPressingDown) {
    int animVal = 10 + facing;

    if (!isShooting) {
        // Create shot
        int xStart = 0;
        int yStart = 0;
        int xShotVel = 0;
        int yShotVel = 0;

        if (isOnPole && isPressingDown) {
            xStart = hitbox.x;
            yStart = hitbox.y + TILE_HEIGHT;
            yShotVel = 1;
            animVal = 24 + facing;
        } else if (isPressingUp) {
            xStart = hitbox.x;
            yStart = hitbox.y - TILE_HEIGHT;
            yShotVel = -1;
            animVal = isOnGround ? 12 : 15;
            if (isOnPole) animVal = 28 + facing;
        } else if (isPressingDown && !isOnGround) {
            xStart = hitbox.x;
            yStart = hitbox.y + TILE_HEIGHT;
            yShotVel = 1;
            animVal = 16;
        } else {
            xStart = facing == 0 ? hitbox.x - TILE_WIDTH : hitbox.x + TILE_WIDTH;
            yStart = hitbox.y + TILE_HEIGHT / 3;
            xShotVel = facing == 0 ? -1 : 1;

            if (!isOnGround) animVal = 13 + facing;
            if (isOnPole) animVal = 26 + facing;
        }

        new BlasterShot(xStart, yStart, xShotVel, yShotVel);

        animate(animVal);

        isShooting = true;
        isAnimLocked = true;
        shootingFrameCount = 1;
    } else if (shootingFrameCount >= 4) {
        isShooting = false;
        isAnimLocked = false;
        shootingFrameCount = 0;
    } else {
        shootingFrameCount++;
    }

    if (isOnGround) {
        xVel = 0;
        xVelRem = 0;
    }

    isOnPogo = false;
}

void Player::walk(directionEnum dir) {
    // TODO: Enforce dir being only left or right
    bool isFacingChanging = dir != facing;
    facing = dir == LEFT ? LEFT : RIGHT;

    // Does this special case warrant a change of method name to something more general?
    if (isOnPole) {
        if (isFacingChanging) {
            Tile* pole = getCollidingPoleTile();
            snapToPole(pole, facing);
            animate(21 + facing);
        }
    } else if (isOnGround) {
        xAccel = 0;

        xVel = dir == LEFT ? -5 : 5;
        animate(2 + facing);
    } else {
        // Walking in the air
        // Stray thought: Maybe it's just low acceleration and not "drag"
        xAccel = dir == LEFT ? -1.7 : 1.7;
        xVel += xAccel;

        // Limit velocity
        if (xVel > 7) {
            xVel = 6;
            xVelRem = 0;
        } else if (xVel < -7) {
            xVel = -6;
            xVelRem = 0;
        }
    }
}

void Player::stopWalk() {
    // This is not synonymous with idle. This is specifically NOT holding left or right arrow keys.
    // It's not idling because we could be falling, shooting, etc.

    if (isOnGround) {
        xAccel = 0;
        xVel = 0;
        xVelRem = 0;

        if (lookTimer == 0)
            animate(facing);
    } else {
        // Falling with drag
        if (!isOnPogo) {
            float drag = facing == LEFT ? 0.8 : -0.8;
            xVel += drag;
        }

        // Make sure remainder is applied in direction opposite movement
        //if ((facing == LEFT && xVelRem < 0) || (facing == RIGHT && xVelRem > 0))
        //    xVelRem *= -1;
        xVelRem = 0;

        // Limit velocity
        if ((facing == LEFT && xVel > 0) || (facing == RIGHT && xVel < 0)) {
            xVel = 0;
            xVelRem = 0;
        }
    }
}

void Player::stopClimb() {
    if (animState != 21 + facing) {
        Tile* pole = getCollidingPoleTile();
        snapToPole(pole, facing);
        animate(21 + facing);
    } else if (frame / FRAMETIME != 0) {
        // Hack: Force a frame reset
        // Could add an optional param to animate to reset frame
        frame = anims[animState].size() * FRAMETIME - 1;
        animate(21 + facing);
    }
}

void Player::processUpArrow() {
    if (isOnPole) {
        climb(UP);
        return;
    }

    Tile* pole = getCollidingPoleTile();
    if (pole != NULL) {
        snapToPole(pole, facing);
        animate(21 + facing);
        return;
    }

    look(UP);
}

void Player::processDownArrow() {
    if (isOnPole) {
        climb(DOWN);
        return;
    }

    Tile* pole = getCollidingPoleTile();
    if (pole != NULL) {
        Tile* tileCollidingWithBottom = getTileCollidingWithBottom(true);
        if (tileCollidingWithBottom != NULL && !tileCollidingWithBottom->getCollideBottom()) {
            snapToPole(pole, facing);
            animate(21 + facing);
            return;
        }
    }

    look(DOWN);
}

void Player::snapToPole(Tile* pole, directionEnum facing) {
    if (pole == NULL)
        return;

    // "Snap" to the pole horizontally, locking movement in x-direction
    if (facing == LEFT)
        xVel = pole->getBox().x + TILE_WIDTH / 4 - hitbox.x;
    else
        xVel = pole->getBox().x - TILE_WIDTH / 4 - hitbox.x;

    isOnPole = true;

    if (isOnGround) {
        yVel = -2;
        isOnGround = false;
    }
}

Tile* Player::getCollidingPoleTile() {
    SDL_Rect nextHitbox = getNextHitboxX();
    vector<Tile*> leftTiles = getTilesToLeft();

    for (unsigned int i = 0; i < leftTiles.size(); i++) {
        SDL_Rect tileBox = leftTiles[i]->getBox();
        if (tileBox.y + tileBox.h > hitbox.y && hitbox.y >= tileBox.y &&
            leftTiles[i]->isColliding(Tile::ISPOLE, hitbox, nextHitbox)) {

            SDL_Rect poleBox = leftTiles[i]->getBox();
            int poleRight = poleBox.x + poleBox.w;
            int playerRight = hitbox.x + hitbox.w;

            // Must be in center of pole
            if (hitbox.x < poleRight - TILE_WIDTH / 4 && playerRight >= TILE_WIDTH / 4 + poleBox.x) {
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

            int playerRight = hitbox.x + hitbox.w;
            SDL_Rect poleBox = rightTiles[i]->getBox();
            int poleRight = poleBox.x + poleBox.w;

            // Must be in center of pole
            if (hitbox.x < poleRight - TILE_WIDTH / 4 && playerRight >= TILE_WIDTH / 4 + poleBox.x) {
                return rightTiles[i];
            } else {
                return NULL;
            }
        }
    }

    return NULL;
}

void Player::look(directionEnum dir) {
    if (!isOnGround) return;

    lookTimer++;

    if (dir == UP) {
        animate(30);

        if (lookTimer >= FRAMES_PER_SECOND / 2 && hitbox.y + hitbox.h < cameraRef.getBottomMargin())
            cameraRef.lookUp();
    } else if (dir == DOWN) {
        if (frame < 2)
            animate(31);

        if (lookTimer >= FRAMES_PER_SECOND / 2 && hitbox.y > cameraRef.getTopMargin())
            cameraRef.lookDown();
    }
}

void Player::climb(directionEnum dir) {
    if (dir == UP && getCollidingPoleTile() != NULL) {
        yVel = -3;
        animate(21 + facing, 3);
    } else if (dir == DOWN) {
        yVel = 7;
        int frametime = 3;
        animate(23, frametime);

        if (frame / frametime == 0) {
            Tile* pole = getCollidingPoleTile();
            snapToPole(pole, RIGHT);
        } else if (frame / frametime == 2) {
            Tile* pole = getCollidingPoleTile();
            snapToPole(pole, LEFT);
        }
    }
}

void Player::togglePogo() {
    if (!isOnPogo) {
        isOnPogo = true;
        pogo();
        yVel -= 2;  // Provide a slightly stronger initial jump
    } else {
        isOnPogo = false;
    }
}

void Player::pogo() {
    // Continue idly jumping while on pogo
    //jump();
    if (isOnGround) {
        yVel = -24;
        isOnGround = false; // This isn't ideal. It's assuming nothing stopped the jump.
        platformStandingOn = NULL;  // Not my favorite hack.
    }

    if (yVel >= 0)
        animate(17 + facing);
    else
        animate(19 + facing);
}

void Player::jump() {
    if (controllerRef.isHoldingCtrl && yVel < 0) {
        if (isOnPogo)
            yAccel = -1.5;
        else
            yAccel = -1.3;
        yVel += yAccel;
    } else if (isOnGround && !controllerRef.isHoldingCtrl) {
        yAccel = -21;
        yVel += yAccel;
        isOnGround = false; // This isn't ideal. It's assuming nothing stopped the jump.
        platformStandingOn = NULL;  // Not my favorite hack.
    } else if (isOnPole) {
        yAccel = -12;
        yVel += yAccel;
        isOnPole = false;
    }
}

void Player::jumpDown() {
    if (!isOnGround) return;

    if (platformStandingOn == NULL) {
        Tile* tile = getTileUnderFeet();
        if (tile == NULL || tile->getCollideBottom() || !tile->getCollideTop())
            return;
    }

    // Verified that Keen is on a platform or valid tile to jump down from
    // Turn off collision for one update loop
    isOnGround = false;
    platformStandingOn = NULL;
    isJumpingDown = true;
    lookTimer = 0;
}

// This is redundant with the one in Enemy class but currently Player and Enemy
// don't agree on values for facing
Tile* Player::getTileUnderFeet() {
    int keenBottom = hitbox.y + hitbox.h;

    if (keenBottom % TILE_HEIGHT != 0)
        return NULL;

    int keenRight = hitbox.x + hitbox.w;
    unsigned int leftCol = hitbox.x / TILE_WIDTH;
    unsigned int rightCol = (keenRight + TILE_WIDTH) / TILE_WIDTH;
    int tileRow = keenBottom / TILE_HEIGHT;
    Tile* tile = NULL;

    // If moving left, return leftmost tile. If moving right, return rightmost
    // If the unit is pushed by something else, will that mess up this logic?
    for (unsigned int i = leftCol; i < rightCol; i++) {
        tile = tilesRef[i][tileRow];
        if (tile != NULL) {
            if (facing == LEFT)
                return tile;
        }
    }
    return tile;
}

void Player::fall() {
    yAccel = 2.6;

    if (yVel >= 20)
        return;

    yVel += yAccel;

    if (!isOnGround && !isShooting && !isOnPogo) { // Implies that he's either falling or jumping
        if (yVel > 0) {
            animate(8 + facing);
        } else if (yVel == 0) {
            animate(6 + facing);
        } else {
            animate(4 + facing);
        }
    }
}

void Player::processKeyboard() {
    // Read in current keyboard state and update object accordingly
    const Uint8* state = SDL_GetKeyboardState(NULL);

    if (state[SDL_SCANCODE_LALT] && !controllerRef.isHoldingAlt && !isOnPole && !isHangingLeft && !isHangingRight) {
        togglePogo();
        controllerRef.isHoldingAlt = true;
    } else if (isOnPogo) {
        pogo();
    }

    if (state[SDL_SCANCODE_LEFT] && !isRolling) {
        controllerRef.isHoldingLeft = true;
        walk(LEFT);
    } else if (state[SDL_SCANCODE_RIGHT] && !isRolling) {
        controllerRef.isHoldingRight = true;
        walk(RIGHT);
    } else if (!isOnPole) {
        stopWalk();
    }

    if (state[SDL_SCANCODE_LCTRL]) {
        if (controllerRef.isHoldingDown && !controllerRef.isHoldingCtrl) {
            jumpDown();
        } else if (!controllerRef.isHoldingDown) {
            jump();
            lookTimer = 0;
        }
        controllerRef.isHoldingCtrl = true;
    } else if (state[SDL_SCANCODE_UP] && !isOnPogo) {
        controllerRef.isHoldingUp = true;
        processUpArrow();
    } else if (state[SDL_SCANCODE_DOWN] && !isOnPogo) {
        processDownArrow();
        controllerRef.isHoldingDown = true;
    } else if (isOnPole) {
        lookTimer = 0;
        stopClimb();
    } else {
        lookTimer = 0;
    }

    if (state[SDL_SCANCODE_SPACE] && !controllerRef.isHoldingSpace && !isHangingLeft && !isHangingRight) {
        shoot(state[SDL_SCANCODE_UP], state[SDL_SCANCODE_DOWN]);
        controllerRef.isHoldingSpace = true;
    }
}

bool Player::isCollidingWithPlatform(SDL_Rect platformBox) {
    int keenLeft = hitbox.x;
    int keenRight = hitbox.x + hitbox.w;
    int platformLeft = platformBox.x;
    int platformRight = platformBox.x + platformBox.w;

    if (keenLeft >= platformRight) return false;
    if (keenRight <= platformLeft) return false;

    SDL_Rect nextHitbox = getNextHitboxXY();
    return isBottomColliding(hitbox, nextHitbox, platformBox);
}

bool Player::handlePlatformCollision() {
    for (unsigned int i = 0; i < platformBatchRef.size(); i++) {
        SDL_Rect platformBox = platformBatchRef[i]->getBox();

        if (isCollidingWithPlatform(platformBox)) {
            int keenBottom = hitbox.y + hitbox.h;
            int platformTop = platformBox.y;

            yVel = platformTop - keenBottom;
            yVelRem = 0;

            int platformXVel = platformBatchRef[i]->getXVel();

            if (!isOnPogo && (xVel == 0 || (xVel == platformXVel && platformXVel != 0)))
                animate(33);

            return true;
        }
    }

    return false;
}

void Player::handleLeftLedgeCollision() {
    // "Left" being left edge of Keen
    int nextKeenLeft = hitbox.x + xVel + (int)xVelRem;
    int keenTop = hitbox.y;
    int nextKeenTop = hitbox.y + yVel + (int)yVelRem;

    if (nextKeenLeft % TILE_WIDTH != 0) return;

    int yCollide = -1;

    for (int i = keenTop; i <= nextKeenTop; i++) {
        if (i % TILE_HEIGHT == 0) {
            yCollide = i;
            break;
        }
    }

    if (yCollide == -1) return;

    int tileRow = nextKeenTop / TILE_HEIGHT;
    int tileCol = nextKeenLeft / TILE_WIDTH - 1;

    Tile* tile = tilesRef[tileCol][tileRow];
    if (tile == NULL || !tile->getIsEdge())
        return;

    yVel = yCollide - keenTop;
    animate(34);
    isHangingLeft = true;
}

void Player::handleRightLedgeCollision() {
    // "Right" being right edge of Keen
    int nextKeenRight = hitbox.x + hitbox.w + xVel + (int)xVelRem;
    int keenTop = hitbox.y;
    int nextKeenTop = hitbox.y + yVel + (int)yVelRem;

    if (nextKeenRight % TILE_WIDTH != 0) return;

    int yCollide = -1;

    for (int i = keenTop; i <= nextKeenTop; i++) {
        if (i % TILE_HEIGHT == 0) {
            yCollide = i;
            break;
        }
    }

    if (yCollide == -1) return;

    int tileRow = nextKeenTop / TILE_HEIGHT;
    int tileCol = nextKeenRight / TILE_WIDTH;

    Tile* tile = tilesRef[tileCol][tileRow];
    if (tile == NULL || !tile->getIsEdge())
        return;

    yVel = yCollide - keenTop;
    animate(35);
    isHangingRight = true;
}

void Player::rollLeft() {
    if (hangTimer < hangCooldown) return;

    xVel = 0;
    xVelRem = 0;
    yVel = 0;
    yVelRem = 0;
    const int rollingFrametime = 4;

    if (!isRolling) {
        isRolling = true;
        yVel = TILE_HEIGHT * -1;
    }

    // Shift left and up on 2nd frame
    if (rollingFrameCount % rollingFrametime == 0 && rollingFrameCount / rollingFrametime == 1) {
        xVel = TILE_WIDTH * -1;
        yVel = TILE_HEIGHT * -1;
    }

    animate(36, rollingFrametime);
    rollingFrameCount++;

    // Stop rolling at end of last frame
    if (rollingFrameCount % rollingFrametime == 0 && rollingFrameCount / rollingFrametime == 4) {
        isRolling = false;
        isHangingLeft = false;
        rollingFrameCount = 0;
        hangTimer = 0;
    }
}

void Player::rollRight() {
    if (hangTimer < hangCooldown) return;

    xVel = 0;
    xVelRem = 0;
    yVel = 0;
    yVelRem = 0;
    const int rollingFrametime = 4;

    if (!isRolling) {
        isRolling = true;
        yVel = TILE_HEIGHT * -1;
    }

    // Shift left and up on 2nd frame
    if (rollingFrameCount % rollingFrametime == 0 && rollingFrameCount / rollingFrametime == 1) {
        xVel = TILE_WIDTH * 1;
        yVel = TILE_HEIGHT * -1;
    }

    animate(37, rollingFrametime);
    rollingFrameCount++;

    // Stop rolling at end of last frame
    if (rollingFrameCount % rollingFrametime == 0 && rollingFrameCount / rollingFrametime == 4) {
        isRolling = false;
        isHangingRight = false;
        rollingFrameCount = 0;
        hangTimer = 0;
    }
}

void Player::handleLedgeHanging() {
    // Increment hang timer if hanging
    if ((isHangingLeft || isHangingRight) && hangTimer < hangCooldown)
        hangTimer++;

    // Check for actions while hanging on ledge
    if (isHangingLeft && (controllerRef.isHoldingRight || controllerRef.isHoldingDown))
        isHangingLeft = false;
    else if (isHangingRight && (controllerRef.isHoldingLeft || controllerRef.isHoldingDown))
        isHangingRight = false;
    else if (isHangingLeft && !isRolling && (controllerRef.isHoldingUp || controllerRef.isHoldingLeft))
        rollLeft();
    else if (isHangingRight && !isRolling && (controllerRef.isHoldingUp || controllerRef.isHoldingRight))
        rollRight();
    else if (isRolling && isHangingLeft)
        rollLeft();
    else if (isRolling && isHangingRight)
        rollRight();

    // Check for ledge - Should be done AFTER xVel collision
    //  this allows player to hold arrow against wall
    if (!isOnGround && yVel > 0) {
        if (controllerRef.isHoldingLeft)
            handleLeftLedgeCollision();
        if (controllerRef.isHoldingRight)
            handleRightLedgeCollision();
    }
}

void Player::checkItemCollision() {
    for (unsigned int i = 0; i < itemBatchRef.size(); i++) {
        if (isUnitColliding(itemBatchRef[i]->getBox()))
            handleItemCollision(itemBatchRef[i]);
    }
}

void Player::handleItemCollision(Item* item) {
    item->beginExpire();

    ItemTypeEnum itemType = item->getType();

    // TODO: Add PlayerStats class
    if (itemType == AMMO) {
        // Add item value to ammo
    } else if (itemType == VITALIN) {
        // Add item value to vitalin
    } else {
        // Add item value to points
    }
}

void Player::update() {
    // Process in this order
    // 1) User actions
    // 2) AI actions
    // 3) Environment consequences (gravity, other static things)
    // 4) Collision
    // 3 & 4 might need mixed or reversed in some situations
    // Handle animations at any step - Some steps can overwrite other steps' animation
    // e.g. Collision while walking
    // Other animations are only done via keypress e.g. shooting but also are preventable by collision, like dying

    // Check any "blocking" actions before processing more input
    if (isStunned) {
        fall();
        hitbox.x += xVel;
        hitbox.y += yVel;
        return;
    } else if (isShooting) {
        shoot(false, false);
    } else {
        processKeyboard();
    }

    // Apply gravity and relevant animations
    if (!isOnPole && !isHangingLeft && !isHangingRight)
        fall();

    if (platformStandingOn != NULL) {
        // If Platform handle exists, adjust player position by Platform velocity
        int platformXVel = platformStandingOn->getXVel();
        xVel += platformXVel;
        xVelRem = 0;
        yVel = platformStandingOn->getYVel();
        yVelRem = 0;
        isOnGround = true;

        if (!isOnPogo && (xVel == 0 || (xVel == platformXVel && platformXVel != 0)))
            animate(33);
    } else if (!isJumpingDown) {
        if (handlePlatformCollision())
            isOnGround = true;
    }

    // Apply push from other units
    if (xPush != 0) {
        xVel += xPush;
        xPush = 0;
    }

    // Check left/right collision
    TileCollisionInfo tciLR;
    if (xVel != 0) {
        tciLR = checkTileCollisionLR();

        // Set properties based on x-collision
        if (tciLR.isLeftColliding()) {
            xVel = (tciLR.tileCollidingWithLeft->getBox().x + tciLR.tileCollidingWithLeft->getBox().w) - hitbox.x;
        } else if (tciLR.isRightColliding()) {
            xVel = tciLR.tileCollidingWithRight->getBox().x - (hitbox.x + hitbox.w);
        }
    }

    // Must go after left/right collision check
    handleLedgeHanging();

    // Check top/bottom collision
    TileCollisionInfo tciTB;
    if (yVel != 0 && !isJumpingDown && !isRolling) {
        tciTB = checkTileCollisionTB();

        // Set properties based on y-collision
        if (tciTB.isBottomChecked)
            isOnGround = tciTB.isBottomColliding() || platformStandingOn != NULL;

        if (tciTB.isTopColliding()) {
            yVel = (tciTB.tileCollidingWithTop->getBox().y + tciTB.tileCollidingWithTop->getBox().h) - hitbox.y;
        } else if (tciTB.isBottomColliding() && (!isOnPole || tciTB.tileCollidingWithBottom->getCollideBottom())) {
            Tile* tile = tciTB.tileCollidingWithBottom;
            yVel = tile->getBox().y - (hitbox.y + hitbox.h);

            if (tile->getIsSloped()) {
                int xPosInTile = (hitbox.x + (int)xVel) - tile->getBox().x;
                //printf("%d = %d - %d\n", xPosInTile, hitbox.x + (int)xVel, tile->getBox().x);

                // y = mx + b
                float yDesiredPosInTile = tile->getSlope() * xPosInTile + tile->getLeftHeight();
                yVel += (TILE_HEIGHT - yDesiredPosInTile);
                printf("%f: %f = %f * %d + %d\n", yVel, yDesiredPosInTile, tile->getSlope(), xPosInTile, tile->getLeftHeight());
            }
        }
    }

    // Check item collision
    checkItemCollision();

    // Update hitbox //
    // Add back remainder
    xVel += xVelRem;
    // This is a bug. This used to be abs(yVelRem) which unintentionally cast yVelRem into an int.
    // For some reason, unless I cast to int here, I fall through the floor when hitbox.y = TILE_HEIGHT * 8
    yVel += (int)yVelRem;

    // Add int part of vel to pos
    hitbox.x += xVel;
    hitbox.y += yVel;

    // Set fractional part of vel to rem
    double intPart;
    xVelRem = modf(xVel, &intPart);
    yVelRem = modf(yVel, &intPart);

    // Reset velocity if collision or on pole
    if (tciTB.isTopColliding() || tciTB.isBottomColliding() || platformStandingOn != NULL || isHangingLeft || isHangingRight) {
        yVel = 0;
        yVelRem = 0;
    }
    if (tciLR.isLeftColliding() || tciLR.isRightColliding() || isRolling) {
        xVel = 0;
        xVelRem = 0;
    }
    if (isOnPole) {
        xVel = 0;
        xVelRem = 0;
        yVel = 0;
        yVelRem = 0;

        if (tciTB.isBottomColliding() && (!isOnPole || tciTB.tileCollidingWithBottom->getCollideBottom()))
            isOnPole = false;
    }

    isJumpingDown = false;
}

void Player::animate(int nextState, int frametime) {
    if (isAnimLocked) return;

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

void Player::draw(Texture* texture, SDL_Rect cameraBox) {
    // Center the hitbox (horizontally) inside the displayed frame
    int offsetX = srcClip->w / 2 - TILE_WIDTH / 2;
    int destX = hitbox.x - offsetX;

    // Align the hitbox for taller vertical frames
    int offsetY;

    if (find(topAlignedFrames.begin(), topAlignedFrames.end(), srcClip) != topAlignedFrames.end()) {
        // Align bottom
        offsetY = srcClip->h - TILE_HEIGHT * 3;
    } else {
        // Align top
        offsetY = srcClip->h - TILE_HEIGHT * 2;
    }

    int destY = hitbox.y - offsetY;

    texture->render(destX - cameraBox.x, destY - cameraBox.y, srcClip);
}

void Player::die(int collidingEnemyX) {
    if (!isStunned) {
        isOnGround = true;  // Animation hack
        xVel = 0;
        xVelRem = 0;
        yVel = 0;
        yVelRem = 0;
        cameraRef.isDisabled = true;
    }

    isStunned = true;

    fall();

    // Play death animation for every enemy collision
    xVel = hitbox.x < collidingEnemyX ? -5 : 5;
    yVel += -8;
    animate(32);

    hitbox.x += xVel;
    hitbox.y += yVel;

    // Pause game loop after falling off screen
    // Show menu
}

void Player::pushX(int x) {
    if (!isOnPole)
        xPush += x;
}

void Player::pushY(int y) {
    if (!isOnPole)
        yPush += y;
}

bool Player::getIsOnGround() { return isOnGround; }

Player::Player(int spawnX, int spawnY) {
    xVel = 0;
    yVel = 0;
    xVelRem = 0;
    yVelRem = 0;
    xPush = 0;
    yPush = 0;
    xAccel = 0;
    yAccel = 0;

    isStunned = false;

    srcClip = NULL;

    hitbox.x = spawnX;
    hitbox.y = spawnY;
    hitbox.w = TILE_WIDTH;
    hitbox.h = TILE_HEIGHT * 2;

    isAnimLocked = false;
    frame = 0;
    animState = 2;
    facing = LEFT;
    idle = true;

    lookTimer = 0;
    hangTimer = 0;
    hangCooldown = FRAMES_PER_SECOND / 4;
    rollingFrameCount = 0;

    isOnGround = true;
    isOnPogo = false;
    isOnPole = false;
    isJumpingDown = false;
    isHangingLeft = false;
    isHangingRight = false;

    platformStandingOn = NULL;

    shootingFrameCount = 0;
    isShooting = false;

    // Animation data
    SDL_Rect standL0 = {0, TILE_HEIGHT * 2, TILE_WIDTH, TILE_HEIGHT * 2};
    SDL_Rect standR0 = {0, 0, TILE_WIDTH, TILE_HEIGHT * 2};

    SDL_Rect walkL0 = {TILE_WIDTH, TILE_HEIGHT * 2, TILE_WIDTH * 2, TILE_HEIGHT * 2};
    SDL_Rect walkL1 = {TILE_WIDTH * 3, TILE_HEIGHT * 2, TILE_WIDTH * 2, TILE_HEIGHT * 2};
    SDL_Rect walkL2 = {TILE_WIDTH * 5, TILE_HEIGHT * 2, TILE_WIDTH, TILE_HEIGHT * 2};
    SDL_Rect walkL3 = {TILE_WIDTH * 6, TILE_HEIGHT * 2, TILE_WIDTH * 2, TILE_HEIGHT * 2};

    SDL_Rect walkR0 = {TILE_WIDTH, 0, TILE_WIDTH * 2, TILE_HEIGHT * 2};
    SDL_Rect walkR1 = {TILE_WIDTH * 3, 0, TILE_WIDTH * 2, TILE_HEIGHT * 2};
    SDL_Rect walkR2 = {TILE_WIDTH * 5, 0, TILE_WIDTH, TILE_HEIGHT * 2};
    SDL_Rect walkR3 = {TILE_WIDTH * 6, 0, TILE_WIDTH * 2, TILE_HEIGHT * 2};

    SDL_Rect jumpL0 = {TILE_WIDTH * 8, TILE_HEIGHT * 2, TILE_WIDTH * 2, TILE_HEIGHT * 2};
    SDL_Rect floatL0 = {TILE_WIDTH * 10, TILE_HEIGHT * 2, TILE_WIDTH * 2, TILE_HEIGHT * 2};
    SDL_Rect fallL0 = {TILE_WIDTH * 12, TILE_HEIGHT * 2, TILE_WIDTH * 2, TILE_HEIGHT * 2};

    SDL_Rect jumpR0 = {TILE_WIDTH * 8, 0, TILE_WIDTH * 2, TILE_HEIGHT * 2};
    SDL_Rect floatR0 = {TILE_WIDTH * 10, 0, TILE_WIDTH * 2, TILE_HEIGHT * 2};
    SDL_Rect fallR0 = {TILE_WIDTH * 12, 0, TILE_WIDTH * 2, TILE_HEIGHT * 2};

    SDL_Rect shootL0 = {0, TILE_HEIGHT * 6, TILE_WIDTH * 3, TILE_HEIGHT * 2};
    SDL_Rect shootR0 = {0, TILE_HEIGHT * 4, TILE_WIDTH * 3, TILE_HEIGHT * 2};
    SDL_Rect shootU0 = {TILE_WIDTH * 10, TILE_HEIGHT * 4, TILE_WIDTH * 2, TILE_HEIGHT * 3};

    SDL_Rect jumpShootL0 = {TILE_WIDTH * 3, TILE_HEIGHT * 6, TILE_WIDTH * 3, TILE_HEIGHT * 2};
    SDL_Rect jumpShootR0 = {TILE_WIDTH * 3, TILE_HEIGHT * 4, TILE_WIDTH * 3, TILE_HEIGHT * 2};
    SDL_Rect jumpShootU0 = {TILE_WIDTH * 8, TILE_HEIGHT * 4, TILE_WIDTH * 2, TILE_HEIGHT * 3};
    SDL_Rect jumpShootD0 = {TILE_WIDTH * 6, TILE_HEIGHT * 4, TILE_WIDTH * 2, TILE_HEIGHT * 2};

    SDL_Rect pogoStraightL = {TILE_WIDTH * 3, TILE_HEIGHT * 8, TILE_WIDTH, TILE_HEIGHT * 2};
    SDL_Rect pogoStraightR = {0, TILE_HEIGHT * 8, TILE_WIDTH, TILE_HEIGHT * 2};
    SDL_Rect pogoBentL = {TILE_WIDTH * 4, TILE_HEIGHT * 8, TILE_WIDTH * 2, TILE_HEIGHT * 2};
    SDL_Rect pogoBentR = {TILE_WIDTH, TILE_HEIGHT * 8, TILE_WIDTH * 2, TILE_HEIGHT * 2};

    SDL_Rect climbL0 = { TILE_WIDTH * 11, TILE_HEIGHT * 10, TILE_WIDTH, TILE_HEIGHT * 2 };
    SDL_Rect climbL1 = { TILE_WIDTH * 12, TILE_HEIGHT * 10, TILE_WIDTH, TILE_HEIGHT * 2 };
    SDL_Rect climbL2 = { TILE_WIDTH * 13, TILE_HEIGHT * 10, TILE_WIDTH, TILE_HEIGHT * 2 };
    SDL_Rect climbR0 = { 0, TILE_HEIGHT * 10, TILE_WIDTH, TILE_HEIGHT * 2 };
    SDL_Rect climbR1 = { TILE_WIDTH, TILE_HEIGHT * 10, TILE_WIDTH, TILE_HEIGHT * 2 };
    SDL_Rect climbR2 = { TILE_WIDTH * 2, TILE_HEIGHT * 10, TILE_WIDTH, TILE_HEIGHT * 2 };
    SDL_Rect climbDown0 = { TILE_WIDTH * 3, TILE_HEIGHT * 10, TILE_WIDTH * 2, TILE_HEIGHT * 2 };
    SDL_Rect climbDown1 = { TILE_WIDTH * 5, TILE_HEIGHT * 10, TILE_WIDTH * 2, TILE_HEIGHT * 2 };
    SDL_Rect climbDown2 = { TILE_WIDTH * 7, TILE_HEIGHT * 10, TILE_WIDTH * 2, TILE_HEIGHT * 2 };
    SDL_Rect climbDown3 = { TILE_WIDTH * 9, TILE_HEIGHT * 10, TILE_WIDTH * 2, TILE_HEIGHT * 2 };

    SDL_Rect poleShootDownL = { TILE_WIDTH * 12, TILE_HEIGHT * 12, TILE_WIDTH * 2, TILE_HEIGHT * 2 };
    SDL_Rect poleShootDownR = { 0, TILE_HEIGHT * 12, TILE_WIDTH * 2, TILE_HEIGHT * 2 };
    SDL_Rect poleShootL = { TILE_WIDTH * 9, TILE_HEIGHT * 12, TILE_WIDTH * 3, TILE_HEIGHT * 2 };
    SDL_Rect poleShootR = { TILE_WIDTH * 2, TILE_HEIGHT * 12, TILE_WIDTH * 3, TILE_HEIGHT * 2 };
    SDL_Rect poleShootUpL = { TILE_WIDTH * 7, TILE_HEIGHT * 12, TILE_WIDTH * 2, TILE_HEIGHT * 3 };
    SDL_Rect poleShootUpR = { TILE_WIDTH * 5, TILE_HEIGHT * 12, TILE_WIDTH * 2, TILE_HEIGHT * 3 };
    SDL_Rect lookUp = { TILE_WIDTH * 6, TILE_HEIGHT * 8, TILE_WIDTH, TILE_HEIGHT * 2 };
    SDL_Rect lookDown0 = { TILE_WIDTH * 7, TILE_HEIGHT * 8, TILE_WIDTH * 2, TILE_HEIGHT * 2 };
    SDL_Rect lookDown1 = { TILE_WIDTH * 9, TILE_HEIGHT * 8, TILE_WIDTH * 2, TILE_HEIGHT * 2 };

    SDL_Rect die0 = { TILE_WIDTH * 11, TILE_HEIGHT * 8, TILE_WIDTH * 2, TILE_HEIGHT * 2 };
    SDL_Rect die1 = { TILE_WIDTH * 13, TILE_HEIGHT * 8, TILE_WIDTH, TILE_HEIGHT * 2 };

    SDL_Rect standPlatform0 = { 0, TILE_HEIGHT * 14, TILE_WIDTH * 2, TILE_HEIGHT * 2 };

    SDL_Rect hangL0 = { 0, TILE_HEIGHT * 16, TILE_WIDTH * 2, TILE_HEIGHT * 3 };
    SDL_Rect hangR0 = { 0, TILE_HEIGHT * 19, TILE_WIDTH, TILE_HEIGHT * 3 };

    SDL_Rect rollL0 = { TILE_WIDTH * 2, TILE_HEIGHT * 16, TILE_WIDTH * 2, TILE_HEIGHT * 2 };
    SDL_Rect rollL1 = { TILE_WIDTH * 4, TILE_HEIGHT * 16, TILE_WIDTH * 2, TILE_HEIGHT };
    SDL_Rect rollL2 = { TILE_WIDTH * 6, TILE_HEIGHT * 16, TILE_WIDTH * 2, TILE_HEIGHT * 2 };
    SDL_Rect rollL3 = { TILE_WIDTH * 8, TILE_HEIGHT * 16, TILE_WIDTH, TILE_HEIGHT * 2 };

    SDL_Rect rollR0 = { TILE_WIDTH * 2, TILE_HEIGHT * 18, TILE_WIDTH * 2, TILE_HEIGHT * 2 };
    SDL_Rect rollR1 = { TILE_WIDTH * 4, TILE_HEIGHT * 18, TILE_WIDTH * 2, TILE_HEIGHT };
    SDL_Rect rollR2 = { TILE_WIDTH * 6, TILE_HEIGHT * 18, TILE_WIDTH * 2, TILE_HEIGHT * 2 };
    SDL_Rect rollR3 = { TILE_WIDTH * 8, TILE_HEIGHT * 18, TILE_WIDTH, TILE_HEIGHT * 2 };

    SDL_Rect standL_array[1] = { standL0 };
    SDL_Rect standR_array[1] = { standR0 };
    SDL_Rect walkL_array[4] = { walkL0, walkL1, walkL2, walkL3 };
    SDL_Rect walkR_array[4] = { walkR0, walkR1, walkR2, walkR3 };
    SDL_Rect jumpL_array[1] = { jumpL0 };
    SDL_Rect floatL_array[1] = { floatL0 };
    SDL_Rect fallL_array[1] = { fallL0 };
    SDL_Rect jumpR_array[1] = { jumpR0 };
    SDL_Rect floatR_array[1] = { floatR0 };
    SDL_Rect fallR_array[1] = { fallR0 };
    SDL_Rect shootL_array[1] = { shootL0 };
    SDL_Rect shootR_array[1] = { shootR0 };
    SDL_Rect shootU_array[1] = { shootU0 };
    SDL_Rect jumpShootL_array[1] = { jumpShootL0 };
    SDL_Rect jumpShootR_array[1] = { jumpShootR0 };
    SDL_Rect jumpShootU_array[1] = { jumpShootU0 };
    SDL_Rect jumpShootD_array[1] = { jumpShootD0 };
    SDL_Rect pogoStraightL_array[1] = { pogoStraightL };
    SDL_Rect pogoStraightR_array[1] = { pogoStraightR };
    SDL_Rect pogoBentL_array[1] = { pogoBentL };
    SDL_Rect pogoBentR_array[1] = { pogoBentR };
    SDL_Rect climbL_array[3] = { climbL0, climbL1, climbL2 };
    SDL_Rect climbR_array[3] = { climbR0, climbR1, climbR2 };
    SDL_Rect climbDown_array[4] = { climbDown0, climbDown1, climbDown2, climbDown3 };
    SDL_Rect poleShootDownL_array[1] = { poleShootDownL };
    SDL_Rect poleShootDownR_array[1] = { poleShootDownR };
    SDL_Rect poleShootL_array[1] = { poleShootL };
    SDL_Rect poleShootR_array[1] = { poleShootR };
    SDL_Rect poleShootUpL_array[1] = { poleShootUpL };
    SDL_Rect poleShootUpR_array[1] = { poleShootUpR };
    SDL_Rect lookUp_array[1] = { lookUp };
    SDL_Rect lookDown_array[2] = { lookDown0, lookDown1 };
    SDL_Rect die_array[2] = { die0, die1 };
    SDL_Rect standPlatform_array[1] = { standPlatform0 };
    SDL_Rect hangL_array[1] = { hangL0 };
    SDL_Rect hangR_array[2] = { hangR0 };
    SDL_Rect rollL_array[4] = { rollL0, rollL1, rollL2, rollL3 };
    SDL_Rect rollR_array[4] = { rollR0, rollR1, rollR2, rollR3 };

    vector<SDL_Rect> standL_anim(standL_array, standL_array + sizeof(standL_array) / sizeof(SDL_Rect));
    vector<SDL_Rect> standR_anim(standR_array, standR_array + sizeof(standR_array) / sizeof(SDL_Rect));
    vector<SDL_Rect> walkL_anim(walkL_array, walkL_array + sizeof(walkL_array) / sizeof(SDL_Rect));
    vector<SDL_Rect> walkR_anim(walkR_array, walkR_array + sizeof(walkR_array) / sizeof(SDL_Rect));
    vector<SDL_Rect> jumpL_anim(jumpL_array, jumpL_array + sizeof(jumpL_array) / sizeof(SDL_Rect));
    vector<SDL_Rect> floatL_anim(floatL_array, floatL_array + sizeof(floatL_array) / sizeof(SDL_Rect));
    vector<SDL_Rect> fallL_anim(fallL_array, fallL_array + sizeof(fallL_array) / sizeof(SDL_Rect));
    vector<SDL_Rect> jumpR_anim(jumpR_array, jumpR_array + sizeof(jumpR_array) / sizeof(SDL_Rect));
    vector<SDL_Rect> floatR_anim(floatR_array, floatR_array + sizeof(floatR_array) / sizeof(SDL_Rect));
    vector<SDL_Rect> fallR_anim(fallR_array, fallR_array + sizeof(fallR_array) / sizeof(SDL_Rect));
    vector<SDL_Rect> shootL_anim(shootL_array, shootL_array + sizeof(shootL_array) / sizeof(SDL_Rect));
    vector<SDL_Rect> shootR_anim(shootR_array, shootR_array + sizeof(shootR_array) / sizeof(SDL_Rect));
    vector<SDL_Rect> shootU_anim(shootU_array, shootU_array + sizeof(shootU_array) / sizeof(SDL_Rect));
    vector<SDL_Rect> jumpShootL_anim(jumpShootL_array, jumpShootL_array + sizeof(jumpShootL_array) / sizeof(SDL_Rect));
    vector<SDL_Rect> jumpShootR_anim(jumpShootR_array, jumpShootR_array + sizeof(jumpShootR_array) / sizeof(SDL_Rect));
    vector<SDL_Rect> jumpShootU_anim(jumpShootU_array, jumpShootU_array + sizeof(jumpShootU_array) / sizeof(SDL_Rect));
    vector<SDL_Rect> jumpShootD_anim(jumpShootD_array, jumpShootD_array + sizeof(jumpShootD_array) / sizeof(SDL_Rect));
    vector<SDL_Rect> pogoStraightL_anim(pogoStraightL_array, pogoStraightL_array + sizeof(pogoStraightL_array) / sizeof(SDL_Rect));
    vector<SDL_Rect> pogoStraightR_anim(pogoStraightR_array, pogoStraightR_array + sizeof(pogoStraightR_array) / sizeof(SDL_Rect));
    vector<SDL_Rect> pogoBentL_anim(pogoBentL_array, pogoBentL_array + sizeof(pogoBentL_array) / sizeof(SDL_Rect));
    vector<SDL_Rect> pogoBentR_anim(pogoBentR_array, pogoBentR_array + sizeof(pogoBentR_array) / sizeof(SDL_Rect));
    vector<SDL_Rect> climbL_anim(climbL_array, climbL_array + sizeof(climbL_array) / sizeof(SDL_Rect));
    vector<SDL_Rect> climbR_anim(climbR_array, climbR_array + sizeof(climbR_array) / sizeof(SDL_Rect));
    vector<SDL_Rect> climbDown_anim(climbDown_array, climbDown_array + sizeof(climbDown_array) / sizeof(SDL_Rect));
    vector<SDL_Rect> poleShootDownL_anim(poleShootDownL_array, poleShootDownL_array + sizeof(poleShootDownL_array) / sizeof(SDL_Rect));
    vector<SDL_Rect> poleShootDownR_anim(poleShootDownR_array, poleShootDownR_array + sizeof(poleShootDownR_array) / sizeof(SDL_Rect));
    vector<SDL_Rect> poleShootL_anim(poleShootL_array, poleShootL_array + sizeof(poleShootL_array) / sizeof(SDL_Rect));
    vector<SDL_Rect> poleShootR_anim(poleShootR_array, poleShootR_array + sizeof(poleShootR_array) / sizeof(SDL_Rect));
    vector<SDL_Rect> poleShootUpL_anim(poleShootUpL_array, poleShootUpL_array + sizeof(poleShootUpL_array) / sizeof(SDL_Rect));
    vector<SDL_Rect> poleShootUpR_anim(poleShootUpR_array, poleShootUpR_array + sizeof(poleShootUpR_array) / sizeof(SDL_Rect));
    vector<SDL_Rect> lookUp_anim(lookUp_array, lookUp_array + sizeof(lookUp_array) / sizeof(SDL_Rect));
    vector<SDL_Rect> lookDown_anim(lookDown_array, lookDown_array + sizeof(lookDown_array) / sizeof(SDL_Rect));
    vector<SDL_Rect> die_anim(die_array, die_array + sizeof(die_array) / sizeof(SDL_Rect));
    vector<SDL_Rect> standPlatform_anim(standPlatform_array, standPlatform_array + sizeof(standPlatform_array) / sizeof(SDL_Rect));
    vector<SDL_Rect> hangL_anim(hangL_array, hangL_array + sizeof(hangL_array) / sizeof(SDL_Rect));
    vector<SDL_Rect> hangR_anim(hangR_array, hangR_array + sizeof(hangR_array) / sizeof(SDL_Rect));
    vector<SDL_Rect> rollL_anim(rollL_array, rollL_array + sizeof(rollL_array) / sizeof(SDL_Rect));
    vector<SDL_Rect> rollR_anim(rollR_array, rollR_array + sizeof(rollR_array) / sizeof(SDL_Rect));

    anims[0] = standL_anim; anims[1] = standR_anim;
    anims[2] = walkL_anim; anims[3] = walkR_anim;
    anims[4] = jumpL_anim; anims[5] = jumpR_anim;
    anims[6] = floatL_anim; anims[7] = floatR_anim;
    anims[8] = fallL_anim; anims[9] = fallR_anim;
    anims[10] = shootL_anim; anims[11] = shootR_anim; anims[12] = shootU_anim;
    anims[13] = jumpShootL_anim; anims[14] = jumpShootR_anim;
    anims[15] = jumpShootU_anim; anims[16] = jumpShootD_anim;
    anims[17] = pogoStraightL_anim; anims[18] = pogoStraightR_anim;
    anims[19] = pogoBentL_anim; anims[20] = pogoBentR_anim;
    anims[21] = climbL_anim; anims[22] = climbR_anim; anims[23] = climbDown_anim;
    anims[24] = poleShootDownL_anim; anims[25] = poleShootDownR_anim;
    anims[26] = poleShootL_anim; anims[27] = poleShootR_anim;
    anims[28] = poleShootUpL_anim; anims[29] = poleShootUpR_anim;
    anims[30] = lookUp_anim; anims[31] = lookDown_anim;
    anims[32] = die_anim; anims[33] = standPlatform_anim;
    anims[34] = hangL_anim; anims[35] = hangR_anim;
    anims[36] = rollL_anim; anims[37] = rollR_anim;

    topAlignedFrames.push_back(&anims[34][0]);
    topAlignedFrames.push_back(&anims[35][0]);
}
