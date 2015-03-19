#include <algorithm>
#include "BlasterShot.h"
#include "Camera.h"
#include "Controller.h"
#include "globals.h"
#include "helpers.h"
#include "Item.h"
#include "Platform.h"
#include "Player.h"
#include "StatsManager.h"
#include "Texture.h"
#include "Tile.h"
#include "Timer.h"

using namespace std;

void Player::shoot(bool isPressingUp, bool isPressingDown) {
    int animVal = 10 + (int)facing;

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
            animVal = 24 + (int)facing;
        } else if (isPressingUp) {
            xStart = hitbox.x;
            yStart = hitbox.y - TILE_HEIGHT;
            yShotVel = -1;
            animVal = isOnGround ? 12 : 15;
            if (isOnPole) animVal = 28 + (int)facing;
        } else if (isPressingDown && !isOnGround) {
            xStart = hitbox.x;
            yStart = hitbox.y + TILE_HEIGHT;
            yShotVel = 1;
            animVal = 16;
        } else {
            xStart = facing == Direction::LEFT ? hitbox.x - TILE_WIDTH : hitbox.x + TILE_WIDTH;
            yStart = hitbox.y + TILE_HEIGHT / 3;
            xShotVel = facing == Direction::LEFT ? -1 : 1;

            if (!isOnGround) animVal = 13 + (int)facing;
            if (isOnPole) animVal = 26 + (int)facing;
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

void Player::walk(Direction dir) {
    // TODO: Enforce dir being only left or right
    facing = dir == Direction::LEFT ? Direction::LEFT : Direction::RIGHT;

    if (isOnGround) {
        xVel = dir == Direction::LEFT ? -walkSpeed : walkSpeed;
        animate(2 + (int)facing);
    } else {
        // Walking in the air
        // Stray thought: Maybe it's just low acceleration and not "drag"
        xVel += dir == Direction::LEFT ? -walkSpeedInAir : walkSpeedInAir;

        // Limit velocity
        if (xVel > walkSpeed) {
            xVel = walkSpeed;
            xVelRem = 0;
        } else if (xVel < -walkSpeed) {
            xVel = -walkSpeed;
            xVelRem = 0;
        }
    }
}

void Player::changeDirectionOnPole(Direction dir) {
    bool isFacingChanging = dir != facing;
    if (!isFacingChanging)
        return;

    Tile* pole = getCollidingPoleTile();
    if (pole == nullptr)
        return;

    facing = dir == Direction::LEFT ? Direction::LEFT : Direction::RIGHT;
    snapToPole(pole, facing);
    animate(21 + (int)facing);
}

void Player::stopWalk() {
    // This is not synonymous with idle. This is specifically NOT holding left or right arrow keys.
    // It's not idling because we could be falling, shooting, etc.

    if (isOnGround) {
        xAccel = 0;
        xVel = 0;
        xVelRem = 0;

        if (lookTimer == 0)
            animate((int)facing);
    } else {
        // Falling with drag
        if (!isOnPogo) {
            xVel += facing == Direction::LEFT ? dragSpeed : -dragSpeed;
        }

        // Make sure remainder is applied in direction opposite movement
        //if ((facing == Direction::LEFT && xVelRem < 0) || (facing == Direction::RIGHT && xVelRem > 0))
        //    xVelRem *= -1;
        xVelRem = 0;

        // Limit velocity
        if ((facing == Direction::LEFT && xVel > 0) || (facing == Direction::RIGHT && xVel < 0)) {
            xVel = 0;
            xVelRem = 0;
        }
    }
}

void Player::stopClimb() {
    if (animState != 21 + (int)facing) {
        Tile* pole = getCollidingPoleTile();
        snapToPole(pole, facing);
        animate(21 + (int)facing);
    } else if (frame / FRAMETIME != 0) {
        // Hack: Force a frame reset
        // Could add an optional param to animate to reset frame
        frame = anims[animState].size() * FRAMETIME - 1;
        animate(21 + (int)facing);
    }
}

void Player::processUpArrow() {
    if (isOnPole) {
        climb(Direction::UP);
        return;
    }

    Tile* pole = getCollidingPoleTile();
    if (pole != nullptr) {
        snapToPole(pole, facing);
        animate(21 + (int)facing);
        return;
    }

    look(Direction::UP);
}

void Player::processDownArrow() {
    if (isOnPole) {
        climb(Direction::DOWN);
        return;
    }

    Tile* pole = getCollidingPoleTile();
    if (pole != nullptr) {
        Tile* tileCollidingWithBottom = getTileCollidingWithBottom(true);
        if (tileCollidingWithBottom != nullptr && !tileCollidingWithBottom->getCollideBottom()) {
            snapToPole(pole, facing);
            animate(21 + (int)facing);
            return;
        }
    }

    look(Direction::DOWN);
}

void Player::snapToPole(Tile* pole, Direction facing) {
    if (pole == nullptr)
        return;

    // "Snap" to the pole horizontally, locking movement in x-direction
    if (facing == Direction::LEFT)
        xVel = (pole->getBox().x + TILE_WIDTH / 4 - hitbox.x) / timeDelta;
    else
        xVel = (pole->getBox().x - TILE_WIDTH / 4 - hitbox.x) / timeDelta;

    isOnPole = true;
    isOnGround = false;
}

Tile* Player::getCollidingPoleTile() {
    SDL_Rect nextHitbox = getNextHitboxX();
    vector<Tile*> leftTiles = getTilesToLeft();

    for (unsigned int i = 0; i < leftTiles.size(); i++) {
        SDL_Rect tileBox = leftTiles[i]->getBox();
        if (tileBox.y + tileBox.h > hitbox.y && hitbox.y >= tileBox.y &&
            leftTiles[i]->isColliding(Tile::TileProperty::ISPOLE, hitbox, nextHitbox)) {

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
            rightTiles[i]->isColliding(Tile::TileProperty::ISPOLE, hitbox, nextHitbox)) {

            int playerRight = hitbox.x + hitbox.w;
            SDL_Rect poleBox = rightTiles[i]->getBox();
            int poleRight = poleBox.x + poleBox.w;

            // Must be in center of pole
            if (hitbox.x < poleRight - TILE_WIDTH / 4 && playerRight >= TILE_WIDTH / 4 + poleBox.x) {
                return rightTiles[i];
            } else {
                return nullptr;
            }
        }
    }

    return nullptr;
}

void Player::look(Direction dir) {
    if (!isOnGround) return;

    lookTimer++;

    if (dir == Direction::UP) {
        animate(30);

        if (lookTimer >= FRAMES_PER_SECOND / 2 && hitbox.y + hitbox.h < cameraRef.getBottomMargin())
            cameraRef.lookUp();
    } else if (dir == Direction::DOWN) {
        if (frame < 2)
            animate(31);

        if (lookTimer >= FRAMES_PER_SECOND / 2 && hitbox.y > cameraRef.getTopMargin())
            cameraRef.lookDown();
    }
}

void Player::climb(Direction dir) {
    if (dir == Direction::UP && getCollidingPoleTile() != nullptr) {
        yVel = poleClimbUpVel;
        animate(21 + (int)facing, 3);
    } else if (dir == Direction::DOWN) {
        Tile* pole = getCollidingPoleTile();
        if (pole == nullptr || pole->isPoleEdge) {
            isOnPole = false;
            return;
        }

        yVel = poleClimbDownVel;
        int frametime = 3;
        animate(23, frametime);

        if (frame / frametime == 0) {
            Tile* pole = getCollidingPoleTile();
            snapToPole(pole, Direction::RIGHT);
        } else if (frame / frametime == 2) {
            Tile* pole = getCollidingPoleTile();
            snapToPole(pole, Direction::LEFT);
        }
    }
}

void Player::togglePogo() {
    if (!isOnPogo) {
        isOnPogo = true;
        pogo();
        yVel += startPogoJumpVel;  // Provide a slightly stronger initial jump
    } else {
        isOnPogo = false;
    }
}

void Player::pogo() {
    // Continue idly jumping while on pogo
    //jump();
    if (isOnGround) {
        yVel = hitGroundPogoJumpVel;
        isOnGround = false; // This isn't ideal. It's assuming nothing stopped the jump.
        platformStandingOn = nullptr;  // Not my favorite hack.
    }

    if (yVel >= 0)
        animate(17 + (int)facing);
    else
        animate(19 + (int)facing);
}

void Player::jump() {
    if (controllerRef.isHoldingCtrl && yVel < 0) {
        if (isOnPogo)
            yVel += holdPogoJumpVel;
        else
            yVel += holdJumpVel;
    } else if (isOnGround && !controllerRef.isHoldingCtrl) {
        yVel += startJumpVel;
        isOnGround = false; // This isn't ideal. It's assuming nothing stopped the jump.
        platformStandingOn = nullptr;  // Not my favorite hack.
    } else if (isOnPole) {
        yVel += poleJumpVel;
        isOnPole = false;
    }
}

void Player::jumpDown() {
    if (!isOnGround) return;

    if (platformStandingOn == nullptr) {
        Tile* tile = getTileUnderFeet();
        if (tile == nullptr || tile->getCollideBottom() || !tile->getCollideTop())
            return;
    }

    // Verified that Keen is on a platform or valid tile to jump down from
    // Turn off collision for one update loop
    isOnGround = false;
    platformStandingOn = nullptr;
    isJumpingDown = true;
    lookTimer = 0;
}

// This is redundant with the one in Enemy class but currently Player and Enemy
// don't agree on values for facing
Tile* Player::getTileUnderFeet() {
    int keenBottom = hitbox.y + hitbox.h;

    if (keenBottom % TILE_HEIGHT != 0)
        return nullptr;

    int keenRight = hitbox.x + hitbox.w;
    unsigned int leftCol = hitbox.x / TILE_WIDTH;
    unsigned int rightCol = (keenRight + TILE_WIDTH) / TILE_WIDTH;
    int tileRow = keenBottom / TILE_HEIGHT;
    Tile* tile = nullptr;

    // If moving left, return leftmost tile. If moving right, return rightmost
    // If the unit is pushed by something else, will that mess up this logic?
    for (unsigned int i = leftCol; i < rightCol; i++) {
        tile = tilesRef[i][tileRow];
        if (tile != nullptr) {
            if (facing == Direction::LEFT)
                return tile;
        }
    }
    return tile;
}

void Player::fall() {
    if (yVel >= fallVelLimit)
        return;

    yVel += fallAccel;

    if (!isOnGround && !isShooting && !isOnPogo) { // Implies that he's either falling or jumping
        if (yVel > 0) {
            animate(8 + (int)facing);
        } else if (yVel == 0) {
            animate(6 + (int)facing);
        } else {
            animate(4 + (int)facing);
        }
    }
}

void Player::processKeyboard() {
    // Read in current keyboard state and update object accordingly
    const Uint8* state = SDL_GetKeyboardState(nullptr);

    if (state[SDL_SCANCODE_LALT] && !controllerRef.isHoldingAlt && !isOnPole && !isHangingLeft && !isHangingRight) {
        togglePogo();
        controllerRef.isHoldingAlt = true;
    } else if (isOnPogo) {
        pogo();
    }

    if (state[SDL_SCANCODE_LEFT] && !isRolling) {
        controllerRef.isHoldingLeft = true;
        if (isOnPole)
            changeDirectionOnPole(Direction::LEFT);
        else
            walk(Direction::LEFT);
    } else if (state[SDL_SCANCODE_RIGHT] && !isRolling) {
        controllerRef.isHoldingRight = true;
        if (isOnPole)
            changeDirectionOnPole(Direction::RIGHT);
        else
            walk(Direction::RIGHT);
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

            yVel = (platformTop - keenBottom) / timeDelta;
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
    SDL_Rect nextHitbox = getNextHitboxXY();
    int nextKeenLeft = nextHitbox.x;
    int keenTop = hitbox.y;
    int nextKeenTop = nextHitbox.y;

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
    if (tile == nullptr || !tile->getIsEdge())
        return;

    yVel = (yCollide - keenTop) / timeDelta;
    animate(34);
    isHangingLeft = true;
}

void Player::handleRightLedgeCollision() {
    // "Right" being right edge of Keen
    SDL_Rect nextHitbox = getNextHitboxXY();
    int nextKeenRight = nextHitbox.x + nextHitbox.w;
    int keenTop = hitbox.y;
    int nextKeenTop = nextHitbox.y;

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
    if (tile == nullptr || !tile->getIsEdge())
        return;

    yVel = (yCollide - keenTop) / timeDelta;
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
        yVel = -TILE_HEIGHT / timeDelta;
    }

    // Shift left and up on 2nd frame
    if (rollingFrameCount % rollingFrametime == 0 && rollingFrameCount / rollingFrametime == 1) {
        xVel = -TILE_WIDTH / timeDelta;
        yVel = -TILE_HEIGHT / timeDelta;
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
        yVel = -TILE_HEIGHT / timeDelta;
    }

    // Shift left and up on 2nd frame
    if (rollingFrameCount % rollingFrametime == 0 && rollingFrameCount / rollingFrametime == 1) {
        xVel = TILE_WIDTH / timeDelta;
        yVel = -TILE_HEIGHT / timeDelta;
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

    ItemType itemType = item->getType();

    // TODO: Add PlayerStats class
    if (itemType == ItemType::AMMO) {
        // Add item value to ammo
    } else if (itemType == ItemType::VITALIN) {
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
        hitbox.x += xVel * timeDelta;
        hitbox.y += yVel * timeDelta;
        return;
    } else if (isShooting) {
        shoot(false, false);
    } else {
        processKeyboard();
    }

    // Apply gravity and relevant animations
    if (!isOnPole && !isHangingLeft && !isHangingRight)
        fall();

    if (platformStandingOn != nullptr) {
        // If Platform handle exists, adjust player position by Platform velocity
        int platformXVel = platformStandingOn->getXVel() / timeDelta;
        xVel += platformXVel;
        xVelRem = 0;
        yVel = platformStandingOn->getYVel() / timeDelta;
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
        xVel += xPush / timeDelta;
        xPush = 0;
    }

    // Check left/right collision
    TileCollisionInfo tciLR;
    if (xVel != 0) {
        tciLR = checkTileCollisionLR();
        Tile* collidingTile = nullptr;

        // Set properties based on x-collision
        if (tciLR.isLeftColliding()) {
            collidingTile = tciLR.tileCollidingWithLeft;
            xVel = ((collidingTile->getBox().x + collidingTile->getBox().w) - hitbox.x) / timeDelta;
        } else if (tciLR.isRightColliding()) {
            collidingTile = tciLR.tileCollidingWithRight;
            xVel = (collidingTile->getBox().x - (hitbox.x + hitbox.w)) / timeDelta;
        }

        if (collidingTile != nullptr && collidingTile->getIsDeadly())
            die(collidingTile->getBox().x);
    }

    // Must go after left/right collision check
    if (!isOnPogo)
        handleLedgeHanging();

    // Check top/bottom collision
    TileCollisionInfo tciTB;
    if (yVel != 0 && !isJumpingDown && !isRolling) {
        tciTB = checkTileCollisionTB();
        Tile* collidingTile = nullptr;

        // Set properties based on y-collision
        if (tciTB.isBottomChecked)
            isOnGround = tciTB.isBottomColliding() || platformStandingOn != nullptr;

        if (tciTB.isTopColliding()) {
            collidingTile = tciTB.tileCollidingWithTop;
            yVel = ((collidingTile->getBox().y + collidingTile->getBox().h) - hitbox.y) / timeDelta;
        } else if (tciTB.isBottomColliding() && (!isOnPole || tciTB.tileCollidingWithBottom->getCollideBottom())) {
            collidingTile = tciTB.tileCollidingWithBottom;
            yVel = (collidingTile->getBox().y - (hitbox.y + hitbox.h)) / timeDelta;

            // Updates yVel
            checkAndHandleSlope(collidingTile);
        }

        if (collidingTile != nullptr && collidingTile->getIsDeadly())
            die(collidingTile->getBox().x);
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
    //printf("%f,%f,%f\n", xVel * timeDelta, yVel, yVel * timeDelta);
    hitbox.x += xVel * timeDelta;
    hitbox.y += yVel * timeDelta;

    // Set fractional part of vel to rem
    double intPart;
    xVelRem = modf(xVel, &intPart);
    yVelRem = modf(yVel, &intPart);

    // Reset velocity if collision or on pole
    if (tciTB.isTopColliding() || tciTB.isBottomColliding() || platformStandingOn != nullptr || isHangingLeft || isHangingRight) {
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
    xVel = hitbox.x < collidingEnemyX ? -dieXSpeed : dieXSpeed;
    yVel += dieYVel;
    animate(32);

    hitbox.x += xVel * timeDelta;
    hitbox.y += yVel * timeDelta;

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

Player::Player(int spawnX, int spawnY, StatsManager* statsManager) {
    xVel = 0;
    yVel = 0;
    xVelRem = 0;
    yVelRem = 0;
    xPush = 0;
    yPush = 0;
    xAccel = 0;
    yAccel = 0;

    this->statsManager = statsManager;

    isStunned = false;

    srcClip = nullptr;

    hitbox.x = 230;
    hitbox.y = 500;
    hitbox.w = TILE_WIDTH;
    hitbox.h = TILE_HEIGHT * 2;

    isAnimLocked = false;
    frame = 0;
    animState = 2;
    facing = Direction::LEFT;
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
    isRolling = false;

    platformStandingOn = nullptr;

    shootingFrameCount = 0;
    isShooting = false;

    // Animation data
    vector<SDL_Rect> standLeftAnim = { { 0, TILE_HEIGHT * 2, TILE_WIDTH, TILE_HEIGHT * 2 } };
    vector<SDL_Rect> standRightAnim = { { 0, 0, TILE_WIDTH, TILE_HEIGHT * 2 } };

    vector<SDL_Rect> walkLeftAnim = {
        { TILE_WIDTH, TILE_HEIGHT * 2, TILE_WIDTH * 2, TILE_HEIGHT * 2 },
        { TILE_WIDTH * 3, TILE_HEIGHT * 2, TILE_WIDTH * 2, TILE_HEIGHT * 2 },
        { TILE_WIDTH * 5, TILE_HEIGHT * 2, TILE_WIDTH, TILE_HEIGHT * 2 },
        { TILE_WIDTH * 6, TILE_HEIGHT * 2, TILE_WIDTH * 2, TILE_HEIGHT * 2 }
    };
    vector<SDL_Rect> walkRightAnim = {
        { TILE_WIDTH, 0, TILE_WIDTH * 2, TILE_HEIGHT * 2 },
        { TILE_WIDTH * 3, 0, TILE_WIDTH * 2, TILE_HEIGHT * 2 },
        { TILE_WIDTH * 5, 0, TILE_WIDTH, TILE_HEIGHT * 2 },
        { TILE_WIDTH * 6, 0, TILE_WIDTH * 2, TILE_HEIGHT * 2 }
    };

    vector<SDL_Rect> jumpLeftAnim = { { TILE_WIDTH * 8, TILE_HEIGHT * 2, TILE_WIDTH * 2, TILE_HEIGHT * 2 } };
    vector<SDL_Rect> floatLeftAnim = { { TILE_WIDTH * 10, TILE_HEIGHT * 2, TILE_WIDTH * 2, TILE_HEIGHT * 2 } };
    vector<SDL_Rect> fallLeftAnim = { { TILE_WIDTH * 12, TILE_HEIGHT * 2, TILE_WIDTH * 2, TILE_HEIGHT * 2 } };

    vector<SDL_Rect> jumpRightAnim = { { TILE_WIDTH * 8, 0, TILE_WIDTH * 2, TILE_HEIGHT * 2 } };
    vector<SDL_Rect> floatRightAnim = { { TILE_WIDTH * 10, 0, TILE_WIDTH * 2, TILE_HEIGHT * 2 } };
    vector<SDL_Rect> fallRightAnim = { { TILE_WIDTH * 12, 0, TILE_WIDTH * 2, TILE_HEIGHT * 2 } };

    vector<SDL_Rect> shootLeftAnim = { { 0, TILE_HEIGHT * 6, TILE_WIDTH * 3, TILE_HEIGHT * 2 } };
    vector<SDL_Rect> shootRightAnim = { { 0, TILE_HEIGHT * 4, TILE_WIDTH * 3, TILE_HEIGHT * 2 } };
    vector<SDL_Rect> shootUpAnim = { { TILE_WIDTH * 10, TILE_HEIGHT * 4, TILE_WIDTH * 2, TILE_HEIGHT * 3 } };

    vector<SDL_Rect> jumpShootLeftAnim = { { TILE_WIDTH * 3, TILE_HEIGHT * 6, TILE_WIDTH * 3, TILE_HEIGHT * 2 } };
    vector<SDL_Rect> jumpShootRightAnim = { { TILE_WIDTH * 3, TILE_HEIGHT * 4, TILE_WIDTH * 3, TILE_HEIGHT * 2 } };
    vector<SDL_Rect> jumpShootUpAnim = { { TILE_WIDTH * 8, TILE_HEIGHT * 4, TILE_WIDTH * 2, TILE_HEIGHT * 3 } };
    vector<SDL_Rect> jumpShootDownAnim = { { TILE_WIDTH * 6, TILE_HEIGHT * 4, TILE_WIDTH * 2, TILE_HEIGHT * 2 } };

    vector<SDL_Rect> pogoStraightLeftAnim = { { TILE_WIDTH * 3, TILE_HEIGHT * 8, TILE_WIDTH, TILE_HEIGHT * 2 } };
    vector<SDL_Rect> pogoStraightRightAnim = { { 0, TILE_HEIGHT * 8, TILE_WIDTH, TILE_HEIGHT * 2 } };
    vector<SDL_Rect> pogoBentLeftAnim = { { TILE_WIDTH * 4, TILE_HEIGHT * 8, TILE_WIDTH * 2, TILE_HEIGHT * 2 } };
    vector<SDL_Rect> pogoBentRightAnim = { { TILE_WIDTH, TILE_HEIGHT * 8, TILE_WIDTH * 2, TILE_HEIGHT * 2 } };

    vector<SDL_Rect> climbLeftAnim = {
        { TILE_WIDTH * 11, TILE_HEIGHT * 10, TILE_WIDTH, TILE_HEIGHT * 2 },
        { TILE_WIDTH * 12, TILE_HEIGHT * 10, TILE_WIDTH, TILE_HEIGHT * 2 },
        { TILE_WIDTH * 13, TILE_HEIGHT * 10, TILE_WIDTH, TILE_HEIGHT * 2 }
    };
    vector<SDL_Rect> climbRightAnim = {
        { 0, TILE_HEIGHT * 10, TILE_WIDTH, TILE_HEIGHT * 2 },
        { TILE_WIDTH, TILE_HEIGHT * 10, TILE_WIDTH, TILE_HEIGHT * 2 },
        { TILE_WIDTH * 2, TILE_HEIGHT * 10, TILE_WIDTH, TILE_HEIGHT * 2 }
    };
    vector<SDL_Rect> climbDownAnim = {
        { TILE_WIDTH * 3, TILE_HEIGHT * 10, TILE_WIDTH * 2, TILE_HEIGHT * 2 },
        { TILE_WIDTH * 5, TILE_HEIGHT * 10, TILE_WIDTH * 2, TILE_HEIGHT * 2 },
        { TILE_WIDTH * 7, TILE_HEIGHT * 10, TILE_WIDTH * 2, TILE_HEIGHT * 2 },
        { TILE_WIDTH * 9, TILE_HEIGHT * 10, TILE_WIDTH * 2, TILE_HEIGHT * 2 }
    };

    vector<SDL_Rect> poleShootDownLeftAnim = { { TILE_WIDTH * 12, TILE_HEIGHT * 12, TILE_WIDTH * 2, TILE_HEIGHT * 2 } };
    vector<SDL_Rect> poleShootDownRightAnim = { { 0, TILE_HEIGHT * 12, TILE_WIDTH * 2, TILE_HEIGHT * 2 } };
    vector<SDL_Rect> poleShootLeftAnim = { { TILE_WIDTH * 9, TILE_HEIGHT * 12, TILE_WIDTH * 3, TILE_HEIGHT * 2 } };
    vector<SDL_Rect> poleShootRightAnim = { { TILE_WIDTH * 2, TILE_HEIGHT * 12, TILE_WIDTH * 3, TILE_HEIGHT * 2 } };
    vector<SDL_Rect> poleShootUpLeftAnim = { { TILE_WIDTH * 7, TILE_HEIGHT * 12, TILE_WIDTH * 2, TILE_HEIGHT * 3 } };
    vector<SDL_Rect> poleShootUpRightAnim = { { TILE_WIDTH * 5, TILE_HEIGHT * 12, TILE_WIDTH * 2, TILE_HEIGHT * 3 } };

    vector<SDL_Rect> lookUpAnim = { { TILE_WIDTH * 6, TILE_HEIGHT * 8, TILE_WIDTH, TILE_HEIGHT * 2 } };
    vector<SDL_Rect> lookDownAnim = {
        { TILE_WIDTH * 7, TILE_HEIGHT * 8, TILE_WIDTH * 2, TILE_HEIGHT * 2 },
        { TILE_WIDTH * 9, TILE_HEIGHT * 8, TILE_WIDTH * 2, TILE_HEIGHT * 2 }
    };

    vector<SDL_Rect> dieAnim = {
        { TILE_WIDTH * 11, TILE_HEIGHT * 8, TILE_WIDTH * 2, TILE_HEIGHT * 2 },
        { TILE_WIDTH * 13, TILE_HEIGHT * 8, TILE_WIDTH, TILE_HEIGHT * 2 }
    };

    vector<SDL_Rect> standPlatformAnim = { { 0, TILE_HEIGHT * 14, TILE_WIDTH * 2, TILE_HEIGHT * 2 } };
    vector<SDL_Rect> hangLeftAnim = { { 0, TILE_HEIGHT * 16, TILE_WIDTH * 2, TILE_HEIGHT * 3 } };
    vector<SDL_Rect> hangRightAnim = { { 0, TILE_HEIGHT * 19, TILE_WIDTH, TILE_HEIGHT * 3 } };

    vector<SDL_Rect> rollLeftAnim = {
        { TILE_WIDTH * 2, TILE_HEIGHT * 16, TILE_WIDTH * 2, TILE_HEIGHT * 2 },
        { TILE_WIDTH * 4, TILE_HEIGHT * 16, TILE_WIDTH * 2, TILE_HEIGHT },
        { TILE_WIDTH * 6, TILE_HEIGHT * 16, TILE_WIDTH * 2, TILE_HEIGHT * 2 },
        { TILE_WIDTH * 8, TILE_HEIGHT * 16, TILE_WIDTH, TILE_HEIGHT * 2 }
    };
    vector<SDL_Rect> rollRightAnim = {
        { TILE_WIDTH * 2, TILE_HEIGHT * 18, TILE_WIDTH * 2, TILE_HEIGHT * 2 },
        { TILE_WIDTH * 4, TILE_HEIGHT * 18, TILE_WIDTH * 2, TILE_HEIGHT },
        { TILE_WIDTH * 6, TILE_HEIGHT * 18, TILE_WIDTH * 2, TILE_HEIGHT * 2 },
        { TILE_WIDTH * 8, TILE_HEIGHT * 18, TILE_WIDTH, TILE_HEIGHT * 2 }
    };

    anims[0] = standLeftAnim; anims[1] = standRightAnim;
    anims[2] = walkLeftAnim; anims[3] = walkRightAnim;
    anims[4] = jumpLeftAnim; anims[5] = jumpRightAnim;
    anims[6] = floatLeftAnim; anims[7] = floatRightAnim;
    anims[8] = fallLeftAnim; anims[9] = fallRightAnim;
    anims[10] = shootLeftAnim; anims[11] = shootRightAnim; anims[12] = shootUpAnim;
    anims[13] = jumpShootLeftAnim; anims[14] = jumpShootRightAnim;
    anims[15] = jumpShootUpAnim; anims[16] = jumpShootDownAnim;
    anims[17] = pogoStraightLeftAnim; anims[18] = pogoStraightRightAnim;
    anims[19] = pogoBentLeftAnim; anims[20] = pogoBentRightAnim;
    anims[21] = climbLeftAnim; anims[22] = climbRightAnim; anims[23] = climbDownAnim;
    anims[24] = poleShootDownLeftAnim; anims[25] = poleShootDownRightAnim;
    anims[26] = poleShootLeftAnim; anims[27] = poleShootRightAnim;
    anims[28] = poleShootUpLeftAnim; anims[29] = poleShootUpRightAnim;
    anims[30] = lookUpAnim; anims[31] = lookDownAnim;
    anims[32] = dieAnim; anims[33] = standPlatformAnim;
    anims[34] = hangLeftAnim; anims[35] = hangRightAnim;
    anims[36] = rollLeftAnim; anims[37] = rollRightAnim;

    topAlignedFrames.push_back(&anims[34][0]);
    topAlignedFrames.push_back(&anims[35][0]);
}
