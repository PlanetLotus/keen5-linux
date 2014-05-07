#include "BlasterShot.h"
#include "globals.h"
#include "helpers.h"
#include "Player.h"

using namespace std;

Player::Player() {
    ammo = 5;
    xVel = 0;
    yVel = 0;
    xAccel = 0;
    yAccel = 0;

    srcClip = NULL;

    hitbox.x = TILE_WIDTH;
    hitbox.y = TILE_HEIGHT * 3;
    hitbox.w = TILE_WIDTH;
    hitbox.h = TILE_HEIGHT * 2;

    isAnimLocked = false;
    frame = 0;
    animState = 2;
    facing = LEFT;
    idle = true;

    isCollidingThisTurn = false;

    isOnGround = true;

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

    anims[0] = standL_anim; anims[1] = standR_anim;
    anims[2] = walkL_anim; anims[3] = walkR_anim;
    anims[4] = jumpL_anim; anims[5] = jumpR_anim;
    anims[6] = floatL_anim; anims[7] = floatR_anim;
    anims[8] = fallL_anim; anims[9] = fallR_anim;
    anims[10] = shootL_anim; anims[11] = shootR_anim; anims[12] = shootU_anim;
    anims[13] = jumpShootL_anim; anims[14] = jumpShootR_anim;
    anims[15] = jumpShootU_anim; anims[16] = jumpShootD_anim;
}

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

    if (isOnGround) xVel = 0;
}

void Player::walk(directionEnum dir) {
    // TODO: Enforce dir being only left or right
    facing = dir == LEFT ? LEFT : RIGHT;

    if (isOnGround) {
        xAccel = 0;

        xVel = dir == LEFT ? -5 : 5;
        animate(2 + facing);
    } else {
        // Walking in the air
        xAccel = dir == LEFT ? -1 : 1;
        xVel += xAccel;

        // Limit velocity, but let him go a little too fast briefly
        if (xVel > 7) xVel = 6;
        else if (xVel < -7) xVel = -6;
    }
}

void Player::stopwalk() {
    // This is not synonymous with idle. This is specifically NOT holding left or right arrow keys.
    // It's not idling because we could be falling, shooting, etc.

    if (isOnGround) {
        xAccel = 0;
        xVel = 0;
        animate(facing);    // Make sure shooting gets priority over this
    } else {
        // Currently not using this section (drag). Keen should stop moving without drag.
        // Falling
        int drag = facing == LEFT ? 1 : -1;
        xAccel = drag;
        //xVel += xAccel;

        // Limit velocity
        if (facing == LEFT && xVel > 0) xVel = 0;
        else if (facing == RIGHT && xVel < 0) xVel = 0;
    }
}

void Player::pogo() {
    return;
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

void Player::jump() {
    if (gController.IsHoldingCtrl) {
        yAccel = -1;
        yVel += yAccel;
    } else if (isOnGround) {
        yAccel = -20;
        gController.IsHoldingCtrl = true;
        yVel += yAccel;
        isOnGround = false; // This isn't ideal. It's assuming nothing stopped the jump.
    }
}

void Player::fall() {
    yAccel = 2;

    if (yVel >= 20)
        return;

    yVel += yAccel;

    if (!isOnGround && !isShooting) { // Implies that he's either falling or jumping
        if (yVel > 0) {
            animate(8 + facing);
        } else if (yVel == 0) {
            animate(6 + facing);
        } else {
            animate(4 + facing);
        }
    }
}

// Redefined this in Player because we need to know isOnGround
// This will be necessary for some other classes too so maybe this will
// eventually call for another base class (subclass of Sprite)
void Player::CheckBottomCollision(int minCol, int maxCol, int row, SDL_Rect nextHitbox) {
    for (int i = minCol; i <= maxCol; i++) {
        for (unsigned int j = row; j < gTiles[i].size(); j++) {
            Tile* tile = gTiles[i][j];

            if (tile != NULL &&
                tile->CollideTop() &&
                (isOnGround = IsBottomColliding(nextHitbox, tile->getBox()))) {

                // Set yVel to the distance between the player and the
                // tile he's colliding with
                yVel = tile->getBox().y - (hitbox.y + hitbox.h);
                return;
            }
        }
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

    // Read in current keyboard state and update object accordingly
    const Uint8* state = SDL_GetKeyboardState(NULL);

    if (state[SDL_SCANCODE_LCTRL]) {
        jump();
    }

    if (state[SDL_SCANCODE_LEFT]) {
        walk(LEFT);
    } else if (state[SDL_SCANCODE_RIGHT]) {
        walk(RIGHT);
    } else {
        stopwalk();
    }

    if (state[SDL_SCANCODE_SPACE] && !gController.IsHoldingSpace) {
        shoot(state[SDL_SCANCODE_UP], state[SDL_SCANCODE_DOWN]);
        gController.IsHoldingSpace = true;
    } else if (isShooting) {    // Would prefer this logic elsewhere
        shoot(false, false);
    }

    // Apply gravity
    fall();

    CheckCollision();
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
    // Now that all decisions have been made, finally update player location
    hitbox.x += xVel;
    hitbox.y += yVel;

    // Center the hitbox (horizontally) inside the displayed frame
    int offsetX = srcClip->w / 2 - TILE_WIDTH / 2;
    int destX = hitbox.x - offsetX;

    // Bottom-align the hitbox for taller vertical frames
    int offsetY = srcClip->h - TILE_HEIGHT * 2;
    int destY = hitbox.y - offsetY;

    gKeenTexture->render(destX, destY, srcClip);
}

// Getters and setters

void Player::set_ammo(int x) { ammo = x; }
void Player::set_xVel(int x) { xVel = x; }
void Player::set_yVel(int y) { yVel = y; }
void Player::set_state(int x) { animState = x; }

int Player::get_ammo() { return ammo; }
int Player::get_xVel() { return xVel; }
int Player::get_yVel() { return yVel; }
int Player::get_state() { return animState; }

SDL_Rect Player::get_hitbox() { return hitbox; }
