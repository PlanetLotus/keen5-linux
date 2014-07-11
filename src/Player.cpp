#include <math.h>
#include "BlasterShot.h"
#include "globals.h"
#include "helpers.h"
#include "Player.h"

using namespace std;

void Player::shoot(bool isPressingUp, bool isPressingDown) {
    int animVal = 10 + facing;

    if (!isShooting) {
        // Create shot
        int xStart = 0;
        int yStart = 0;
        int xShotVel = 0;
        int yShotVel = 0;

        if (isPressingUp) {
            xStart = hitbox.x;
            yStart = hitbox.y - TILE_HEIGHT;
            yShotVel = -1;
            animVal = isOnGround ? 12 : 15;
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
}

void Player::walk(directionEnum dir) {
    // TODO: Enforce dir being only left or right
    bool isFacingChanging = dir != facing;
    facing = dir == LEFT ? LEFT : RIGHT;

    // Does this special case warrant a change of method name to something more general?
    if (isOnPole) {
        if (isFacingChanging) {
            Tile* pole = GetCollidingPoleTile();
            snapToPole(pole, facing);
        }
    } else if (isOnGround) {
        xAccel = 0;

        xVel = dir == LEFT ? -5 : 5;
        animate(2 + facing);
    } else {
        // Walking in the air
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

void Player::stopwalk() {
    // This is not synonymous with idle. This is specifically NOT holding left or right arrow keys.
    // It's not idling because we could be falling, shooting, etc.

    if (isOnGround) {
        xAccel = 0;
        xVel = 0;
        xVelRem = 0;
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

void Player::processUpArrow() {
    if (isOnPole) {
        climb(UP);
        return;
    }

    Tile* pole = GetCollidingPoleTile();
    if (pole != NULL) {
        snapToPole(pole, facing);
        return;
    }

    look(UP);
}

void Player::snapToPole(Tile* pole, directionEnum facing) {
    if (pole == NULL) {
        printf("NULL POLE!\n");
        return;
    }

    // "Snap" to the pole horizontally, locking movement in x-direction
    if (facing == LEFT)
        xVel = pole->getBox().x + TILE_WIDTH / 4 - hitbox.x;
    else
        xVel = pole->getBox().x - TILE_WIDTH / 4 - hitbox.x;

    isOnPole = true;

    // Set animation
    animate(21 + facing);
}

Tile* Player::GetCollidingPoleTile() {
    SDL_Rect nextHitbox = { hitbox.x + (int)xVel, hitbox.y, hitbox.w, hitbox.h };
    vector<Tile*> leftTiles = GetTilesToLeft();

    for (unsigned int i = 0; i < leftTiles.size(); i++) {
        if (leftTiles[i]->IsColliding(Tile::ISPOLE, hitbox, nextHitbox)) {
            SDL_Rect poleBox = leftTiles[i]->getBox();
            int poleRight = poleBox.x + poleBox.w;
            int playerRight = hitbox.x + hitbox.w;

            // Must be in center of pole
            if (hitbox.x < poleRight - TILE_WIDTH / 4 && playerRight >= TILE_WIDTH / 4 + poleBox.x) {
                printf("returning leftTile\n");
                return leftTiles[i];
            } else {
                printf("not in pole center\n");
                break;
            }
        }
    }

    // Now do same for rightTiles
    vector<Tile*> rightTiles = GetTilesToRight();

    for (unsigned int i = 0; i < rightTiles.size(); i++) {
        if (rightTiles[i]->IsColliding(Tile::ISPOLE, hitbox, nextHitbox)) {
            int playerRight = hitbox.x + hitbox.w;
            SDL_Rect poleBox = rightTiles[i]->getBox();
            int poleRight = poleBox.x + poleBox.w;

            // Must be in center of pole
            if (hitbox.x < poleRight - TILE_WIDTH / 4 && playerRight >= TILE_WIDTH / 4 + poleBox.x) {
                printf("returning rightTile\n");
                return rightTiles[i];
            } else {
                printf("checking rightTiles - not in pole center\n");
                return NULL;
            }
        }
    }

    return NULL;
}

void Player::look(directionEnum dir) {
    return;
}

void Player::climb(directionEnum dir) {
    return;
}

void Player::enter_door() {
    return;
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
    }

    if (yVel >= 0)
        animate(17 + facing);
    else
        animate(19 + facing);
}

void Player::jump() {
    if (gController.IsHoldingCtrl && yVel < 0) {
        if (isOnPogo)
            yAccel = -1.5;
        else
            yAccel = -1.3;
        yVel += yAccel;
    } else if (isOnGround && !gController.IsHoldingCtrl) {
        yAccel = -21;
        yVel += yAccel;
        isOnGround = false; // This isn't ideal. It's assuming nothing stopped the jump.
    }
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

    if (state[SDL_SCANCODE_LALT] && !gController.IsHoldingAlt && !isOnPole) {
        togglePogo();
        gController.IsHoldingAlt = true;
    } else if (isOnPogo) {
        pogo();
    }

    if (state[SDL_SCANCODE_LCTRL]) {
        jump();
        gController.IsHoldingCtrl = true;
    }

    if (state[SDL_SCANCODE_LEFT]) {
        walk(LEFT);
    } else if (state[SDL_SCANCODE_RIGHT]) {
        walk(RIGHT);
    } else if (!isOnPole) {
        stopwalk();
    }

    if (state[SDL_SCANCODE_UP] & !isOnPogo) {
        processUpArrow();
    }

    if (state[SDL_SCANCODE_SPACE] && !gController.IsHoldingSpace) {
        shoot(state[SDL_SCANCODE_UP], state[SDL_SCANCODE_DOWN]);
        gController.IsHoldingSpace = true;
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
    if (isShooting)
        shoot(false, false);
    else
        processKeyboard();

    // Apply gravity and relevant animations
    if (!isOnPole)
        fall();

    // Check left/right collision
    TileCollisionInfo tciLR;
    if (xVel != 0) {
        tciLR = CheckTileCollisionLR();

        // Set properties based on x-collision
        if (tciLR.IsLeftColliding()) {
            xVel = (tciLR.TileCollidingWithLeft->getBox().x + tciLR.TileCollidingWithLeft->getBox().w) - hitbox.x;
        } else if (tciLR.IsRightColliding()) {
            xVel = tciLR.TileCollidingWithRight->getBox().x - (hitbox.x + hitbox.w);
        }
    }

    // Check top/bottom collision
    TileCollisionInfo tciTB;
    if (yVel != 0) {
        tciTB = CheckTileCollisionTB();

        // Set properties based on y-collision
        if (tciTB.IsBottomChecked)
            isOnGround = tciTB.IsBottomColliding();

        if (tciTB.IsTopColliding()) {
            yVel = (tciTB.TileCollidingWithTop->getBox().y + tciTB.TileCollidingWithTop->getBox().h) - hitbox.y;
        } else if (tciTB.IsBottomColliding()) {
            yVel = tciTB.TileCollidingWithBottom->getBox().y - (hitbox.y + hitbox.h);
        }
    }

    // Update hitbox //
    // Add back remainder
    xVel += xVelRem;
    yVel += abs(yVelRem);

    /*
    if (xVel != 0)
        printf("%f, %f, %f, %f\n", xVel, xVelRem, yVel, yVelRem);
    */

    // Add int part of vel to pos
    hitbox.x += xVel;
    hitbox.y += yVel;

    // Set fractional part of vel to rem
    double intPart;
    xVelRem = modf(xVel, &intPart);
    yVelRem = modf(yVel, &intPart);

    // Reset velocity if collision or on pole
    if (tciTB.IsTopColliding() || tciTB.IsBottomColliding()) {
        yVel = 0;
        yVelRem = 0;
    }
    if (tciLR.IsLeftColliding() || tciLR.IsRightColliding()) {
        xVel = 0;
        xVelRem = 0;
    }
    if (isOnPole) {
        xVel = 0;
        xVelRem = 0;
        yVel = 0;
        yVelRem = 0;
    }
}

void Player::animate(int nextState) {
    if (isAnimLocked) return;

    if (animState == nextState) {
        // Get next frame
        frame++;
    } else {
        // Start new animation
        frame = 0;
        animState = nextState;
    }

    if (frame == anims[animState].size() * FRAMETIME)
        frame = 0;

    srcClip = &anims[animState][frame / FRAMETIME];
}

void Player::draw() {
    // Center the hitbox (horizontally) inside the displayed frame
    int offsetX = srcClip->w / 2 - TILE_WIDTH / 2;
    int destX = hitbox.x - offsetX;

    // Bottom-align the hitbox for taller vertical frames
    int offsetY = srcClip->h - TILE_HEIGHT * 2;
    int destY = hitbox.y - offsetY;

    gKeenTexture->render(destX, destY, srcClip);
}

Player::Player() {
    xVel = 0;
    yVel = 0;
    xVelRem = 0;
    yVelRem = 0;
    xAccel = 0;
    yAccel = 0;

    srcClip = NULL;

    hitbox.x = TILE_WIDTH * 14;
    hitbox.y = TILE_HEIGHT * 3;
    hitbox.w = TILE_WIDTH;
    hitbox.h = TILE_HEIGHT * 2;

    isAnimLocked = false;
    frame = 0;
    animState = 2;
    facing = LEFT;
    idle = true;

    isOnGround = true;
    isOnPogo = false;
    isOnPole = false;

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

    SDL_Rect climbL0 = { TILE_WIDTH * 10, TILE_HEIGHT * 10, TILE_WIDTH, TILE_HEIGHT * 2 };
    SDL_Rect climbL1 = { TILE_WIDTH * 11, TILE_HEIGHT * 10, TILE_WIDTH, TILE_HEIGHT * 2 };
    SDL_Rect climbL2 = { TILE_WIDTH * 12, TILE_HEIGHT * 10, TILE_WIDTH, TILE_HEIGHT * 2 };
    SDL_Rect climbR0 = { 0, TILE_HEIGHT * 10, TILE_WIDTH, TILE_HEIGHT * 2 };
    SDL_Rect climbR1 = { TILE_WIDTH, TILE_HEIGHT * 10, TILE_WIDTH, TILE_HEIGHT * 2 };
    SDL_Rect climbR2 = { TILE_WIDTH * 2, TILE_HEIGHT * 10, TILE_WIDTH, TILE_HEIGHT * 2 };
    SDL_Rect climbDown0 = { TILE_WIDTH * 3, TILE_HEIGHT * 10, TILE_WIDTH * 2, TILE_HEIGHT * 2 };
    SDL_Rect climbDown1 = { TILE_WIDTH * 5, TILE_HEIGHT * 10, TILE_WIDTH * 2, TILE_HEIGHT * 2 };
    SDL_Rect climbDown2 = { TILE_WIDTH * 7, TILE_HEIGHT * 10, TILE_WIDTH * 2, TILE_HEIGHT * 2 };
    SDL_Rect climbDown3 = { TILE_WIDTH * 9, TILE_HEIGHT * 10, TILE_WIDTH * 2, TILE_HEIGHT * 2 };

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
}
