// Player definition

#include "helpers.h"
#include "Player.h"

// Animation (srcClip) frames
SDL_Rect STANDL0 = {0, TILE_HEIGHT * 2, TILE_WIDTH, TILE_HEIGHT * 2};
SDL_Rect STANDR0 = {0, 0, TILE_WIDTH, TILE_HEIGHT * 2};

SDL_Rect WALKL0 = {TILE_WIDTH, TILE_HEIGHT * 2, TILE_WIDTH * 2, TILE_HEIGHT * 2};
SDL_Rect WALKL1 = {TILE_WIDTH * 3, TILE_HEIGHT * 2, TILE_WIDTH * 2, TILE_HEIGHT * 2};
SDL_Rect WALKL2 = {TILE_WIDTH * 5, TILE_HEIGHT * 2, TILE_WIDTH, TILE_HEIGHT * 2};
SDL_Rect WALKL3 = {TILE_WIDTH * 6, TILE_HEIGHT * 2, TILE_WIDTH * 2, TILE_HEIGHT * 2};

SDL_Rect WALKR0 = {TILE_WIDTH, 0, TILE_WIDTH * 2, TILE_HEIGHT * 2};
SDL_Rect WALKR1 = {TILE_WIDTH * 3, 0, TILE_WIDTH * 2, TILE_HEIGHT * 2};
SDL_Rect WALKR2 = {TILE_WIDTH * 5, 0, TILE_WIDTH, TILE_HEIGHT * 2};
SDL_Rect WALKR3 = {TILE_WIDTH * 6, 0, TILE_WIDTH * 2, TILE_HEIGHT * 2};

SDL_Rect JUMPL0 = {TILE_WIDTH * 8, TILE_HEIGHT * 2, TILE_WIDTH * 2, TILE_HEIGHT * 2};
SDL_Rect FLOATL0 = {TILE_WIDTH * 10, TILE_HEIGHT * 2, TILE_WIDTH * 2, TILE_HEIGHT * 2};
SDL_Rect FALLL0 = {TILE_WIDTH * 12, TILE_HEIGHT * 2, TILE_WIDTH * 2, TILE_HEIGHT * 2};

SDL_Rect JUMPR0 = {TILE_WIDTH * 8, 0, TILE_WIDTH * 2, TILE_HEIGHT * 2};
SDL_Rect FLOATR0 = {TILE_WIDTH * 10, 0, TILE_WIDTH * 2, TILE_HEIGHT * 2};
SDL_Rect FALLR0 = {TILE_WIDTH * 12, 0, TILE_WIDTH * 2, TILE_HEIGHT * 2};

SDL_Rect SHOOTL0 = {0, TILE_HEIGHT * 6, TILE_WIDTH * 3, TILE_HEIGHT * 2};
SDL_Rect SHOOTR0 = {0, TILE_HEIGHT * 4, TILE_WIDTH * 3, TILE_HEIGHT * 2};

// TODO: Could contain idle animation so that this isn't just a wasted array
SDL_Rect STANDL_ARRAY[1] = { STANDL0 };
vector<SDL_Rect> STANDL_ANIM(STANDL_ARRAY, STANDL_ARRAY + sizeof(STANDL_ARRAY) / sizeof(SDL_Rect));

// TODO: Could contain idle animation so that this isn't just a wasted array
SDL_Rect STANDR_ARRAY[1] = { STANDR0 };
vector<SDL_Rect> STANDR_ANIM(STANDR_ARRAY, STANDR_ARRAY + sizeof(STANDR_ARRAY) / sizeof(SDL_Rect));

SDL_Rect WALKL_ARRAY[4] = { WALKL0, WALKL1, WALKL2, WALKL3 };
vector<SDL_Rect> WALKL_ANIM(WALKL_ARRAY, WALKL_ARRAY + sizeof(WALKL_ARRAY) / sizeof(SDL_Rect));

SDL_Rect WALKR_ARRAY[4] = { WALKR0, WALKR1, WALKR2, WALKR3 };
vector<SDL_Rect> WALKR_ANIM(WALKR_ARRAY, WALKR_ARRAY + sizeof(WALKR_ARRAY) / sizeof(SDL_Rect));

SDL_Rect JUMPL_ARRAY[1] = { JUMPL0 };
vector<SDL_Rect> JUMPL_ANIM(JUMPL_ARRAY, JUMPL_ARRAY + sizeof(JUMPL_ARRAY) / sizeof(SDL_Rect));

SDL_Rect FLOATL_ARRAY[1] = { FLOATL0 };
vector<SDL_Rect> FLOATL_ANIM(FLOATL_ARRAY, FLOATL_ARRAY + sizeof(FLOATL_ARRAY) / sizeof(SDL_Rect));

SDL_Rect FALLL_ARRAY[1] = { FALLL0 };
vector<SDL_Rect> FALLL_ANIM(FALLL_ARRAY, FALLL_ARRAY + sizeof(FALLL_ARRAY) / sizeof(SDL_Rect));

SDL_Rect JUMPR_ARRAY[1] = { JUMPR0 };
vector<SDL_Rect> JUMPR_ANIM(JUMPR_ARRAY, JUMPR_ARRAY + sizeof(JUMPR_ARRAY) / sizeof(SDL_Rect));

SDL_Rect FLOATR_ARRAY[1] = { FLOATR0 };
vector<SDL_Rect> FLOATR_ANIM(FLOATR_ARRAY, FLOATR_ARRAY + sizeof(FLOATR_ARRAY) / sizeof(SDL_Rect));

SDL_Rect FALLR_ARRAY[1] = { FALLR0 };
vector<SDL_Rect> FALLR_ANIM(FALLR_ARRAY, FALLR_ARRAY + sizeof(FALLR_ARRAY) / sizeof(SDL_Rect));

SDL_Rect SHOOTL_ARRAY[1] = { SHOOTL0 };
vector<SDL_Rect> SHOOTL_ANIM(SHOOTL_ARRAY, SHOOTL_ARRAY + sizeof(SHOOTL_ARRAY) / sizeof(SDL_Rect));

SDL_Rect SHOOTR_ARRAY[1] = { SHOOTR0 };
vector<SDL_Rect> SHOOTR_ANIM(SHOOTR_ARRAY, SHOOTR_ARRAY + sizeof(SHOOTR_ARRAY) / sizeof(SDL_Rect));

// Array of animations
// Statically set...animStateEnum's value needs to match this array
// e.g. if ANIMS[0] == walk right, then animStateEnum[WALKR] == 0
// Purpose: To be dynamic in animate()
// Alternative: Could use an associative array where key=state, value=array of SDL_Rect clip
std::vector<SDL_Rect> ANIMS[12] = {
    STANDL_ANIM, STANDR_ANIM,
    WALKL_ANIM, WALKR_ANIM,
    JUMPL_ANIM, FLOATL_ANIM, FALLL_ANIM,
    JUMPR_ANIM, FLOATR_ANIM, FALLR_ANIM,
    SHOOTL_ANIM, SHOOTR_ANIM
};

Player::Player() {
    ammo = 5;
    xVel = 0;
    yVel = 0;
    yAccel = 0;

    srcClip = NULL;

    hitbox.x = TILE_WIDTH;
    hitbox.y = TILE_HEIGHT * 3;
    hitbox.w = TILE_WIDTH;
    hitbox.h = TILE_HEIGHT * 2;

    frame = 0;
    state = STANDR;
    facing = LEFT;

    canStartJump = true;
    isJumping = false;

    shootingFrameCount = 0;
    isShooting = false;
}

void Player::shoot() {
    if (shootingFrameCount >= 4) {
        isShooting = false;
        shootingFrameCount = 0;
    } else {
        isShooting = true;
        shootingFrameCount++;
    }

    if (facing == LEFT)
        animate(SHOOTL);
    else if (facing == RIGHT)
        animate(SHOOTR);
}

void Player::walk(directionEnum dir) {
    switch (dir) {
        case RIGHT:
            xVel = 5;
            facing = RIGHT;
            animate(WALKR);
            break;
        case LEFT:
            xVel = -5;
            facing = LEFT;
            animate(WALKL);
            break;
        case UP:
            yVel = -5;
            break;
        case DOWN:
            yVel = 5;
            break;
        case STOP:
            xVel = 0;
            // TODO: Make this more dynamic
            if (facing == LEFT)
                animate(STANDL);
            else if (facing == RIGHT)
                animate(STANDR);
            break;
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
    if (!isJumping && !canStartJump) return;

    if (isJumping) {
        yAccel = -1;
    } else if (canStartJump) {
        yAccel = -20;

        isJumping = true;
        canStartJump = false;
    }

    yVel += yAccel;
}

void Player::fall() {
    yAccel = 2;

    if (yVel >= 20)
        return;

    yVel += yAccel;

    if (!canStartJump) { // Implies that he's either falling or jumping
        if (yVel > 0) {
            if (facing == LEFT) animate(FALLL);
            else animate(FALLR);
        } else if (yVel == 0) {
            if (facing == LEFT) animate(FLOATL);
            else animate(FLOATR);
        } else {
            if (facing == LEFT) animate(JUMPL);
            else animate(JUMPR);
        }
    }
}

bool Player::IsTopColliding(SDL_Rect a, SDL_Rect b) {
    int topA = a.y;
    int bottomB = b.y + b.h;

    if (topA >= bottomB) return false;

    return true;
}

bool Player::IsBottomColliding(SDL_Rect a, SDL_Rect b) {
    int bottomA = a.y + a.h;
    int topB = b.y;

    if (bottomA <= topB) {
        canStartJump = false;
        return false;
    }

    canStartJump = true;
    return true;
}

bool Player::IsLeftColliding(SDL_Rect a, SDL_Rect b) {
    int leftA = a.x;
    int rightB = b.x + b.w;

    if (leftA >= rightB) return false;

    return true;
}

bool Player::IsRightColliding(SDL_Rect a, SDL_Rect b) {
    int rightA = a.x + a.w;
    int leftB = b.x;

    if (rightA <= leftB) return false;

    return true;
}

void Player::CheckTopCollision() {
    // Hitbox after update in y-direction
    SDL_Rect nextHitbox = { hitbox.x + xVel, hitbox.y + yVel, hitbox.w, hitbox.h };

    int minCol = nextHitbox.x / TILE_WIDTH;
    int maxCol = (nextHitbox.x + nextHitbox.w) / TILE_WIDTH;

    // TODO: Fix this logic. Currently this addresses the case where, if the rightmost point
    // of player and leftmost point of tile are equal, it should NOT check this column.
    if ((nextHitbox.x + nextHitbox.w) % TILE_WIDTH == 0 && maxCol > 0)
        maxCol--;

    // Don't let maxCol go out of bounds
    if (maxCol > TILES_WIDE-1) maxCol = TILES_WIDE-1;

    int row = nextHitbox.y / TILE_HEIGHT;

    for (int i = minCol; i <= maxCol; i++) {
        for (int j = row; j >= 0; j--) {
            Tile* tile = gTiles[i][j];

            if (tile != NULL &&
                tile->CollideBottom() &&
                IsTopColliding(nextHitbox, tile->getBox())) {

                // Set yVel to the distance between the player and the
                // tile he's colliding with
                yVel = (tile->getBox().y + tile->getBox().h) - hitbox.y;
                return;
            }
        }
    }
}

void Player::CheckBottomCollision() {
    // Hitbox after update in y-direction
    SDL_Rect nextHitbox = { hitbox.x + xVel, hitbox.y + yVel, hitbox.w, hitbox.h };

    int minCol = nextHitbox.x / TILE_WIDTH;
    int maxCol = (nextHitbox.x + nextHitbox.w) / TILE_WIDTH;

    // TODO: Fix this logic. Currently this addresses the case where, if the rightmost point
    // of player and leftmost point of tile are equal, it should NOT check this column.
    if ((nextHitbox.x + nextHitbox.w) % TILE_WIDTH == 0 && maxCol > 0)
        maxCol--;

    // Don't let maxCol go out of bounds
    if (maxCol > TILES_WIDE-1) maxCol = TILES_WIDE-1;

    int row = (nextHitbox.y + nextHitbox.h) / TILE_HEIGHT;

    for (int i = minCol; i <= maxCol; i++) {
        for (unsigned int j = row; j < gTiles[i].size(); j++) {
            Tile* tile = gTiles[i][j];

            if (tile != NULL &&
                tile->CollideTop() &&
                IsBottomColliding(nextHitbox, tile->getBox())) {

                // Set yVel to the distance between the player and the
                // tile he's colliding with
                yVel = tile->getBox().y - (hitbox.y + hitbox.h);
                return;
            }
        }
    }
}

void Player::CheckLeftCollision() {
    // Hitbox after update in x-direction
    // Does not include new yVel
    SDL_Rect nextHitbox = { hitbox.x + xVel, hitbox.y, hitbox.w, hitbox.h };

    int minRow = nextHitbox.y / TILE_HEIGHT;
    int maxRow = (nextHitbox.y + nextHitbox.h) / TILE_HEIGHT;

    // TODO: Fix this logic. Currently this addresses the case where, if the bottommost point
    // of player and topmost point of tile are equal, it should NOT check this row.
    if ((nextHitbox.y + nextHitbox.h) % TILE_WIDTH == 0 && maxRow > 0)
        maxRow--;

    // Don't let maxRow go out of bounds
    if (maxRow > TILES_TALL-1) maxRow = TILES_TALL-1;

    int col = nextHitbox.x / TILE_WIDTH;

    for (int i = col; i >= 0; i--) {
        for (int j = minRow; j <= maxRow; j++) {
            Tile* tile = gTiles[i][j];

            if (tile != NULL &&
                tile->CollideRight() &&
                IsLeftColliding(nextHitbox, tile->getBox())) {

                // Set xVel to the distance between the player and the
                // tile he's colliding with
                xVel = (tile->getBox().x + tile->getBox().w) - hitbox.x;
                return;
            }
        }
    }
}

void Player::CheckRightCollision() {
    // Hitbox after update in x-direction
    // Does not include new yVel
    SDL_Rect nextHitbox = { hitbox.x + xVel, hitbox.y, hitbox.w, hitbox.h };

    int minRow = nextHitbox.y / TILE_HEIGHT;
    int maxRow = (nextHitbox.y + nextHitbox.h) / TILE_HEIGHT;

    // TODO: Fix this logic. Currently this addresses the case where, if the bottommost point
    // of player and topmost point of tile are equal, it should NOT check this row.
    if ((nextHitbox.y + nextHitbox.h) % TILE_WIDTH == 0 && maxRow > 0)
        maxRow--;

    // Don't let maxRow go out of bounds
    if (maxRow > TILES_TALL-1) maxRow = TILES_TALL-1;

    // This could be wrong...should it round up instead?
    int col = nextHitbox.x / TILE_WIDTH;

    for (unsigned int i = col; i < gTiles.size(); i++) {
        for (int j = minRow; j <= maxRow; j++) {
            Tile* tile = gTiles[i][j];

            if (tile != NULL &&
                tile->CollideLeft() &&
                IsRightColliding(nextHitbox, tile->getBox())) {

                // Set xVel to the distance between the player and the
                // tile he's colliding with
                xVel = tile->getBox().x - (hitbox.x + hitbox.w);
                return;
            }
        }
    }
}

bool Player::IsCollidingWithTiles() {
    bool xChange = xVel != 0;
    bool yChange = yVel != 0;

    // For static objects (tiles), check per-axis collision only if we're moving on that axis
    if (xChange) {
        if (xVel > 0)
            CheckRightCollision();
        else
            CheckLeftCollision();
    }

    if (yChange) {
        if (yVel > 0)
            CheckBottomCollision();
        else
            CheckTopCollision();
    }

    return false;
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

    bool idle = true;

    // TODO: Make this suck a lot less
    if (!isShooting) {
        if (state[SDL_SCANCODE_SPACE] && !gController.IsHoldingSpace) {
            // TODO: Rate-limit firing and check holding spacebar
            shoot();
            gController.IsHoldingSpace = true;
            idle = false;
        }

        if (state[SDL_SCANCODE_LCTRL]) {
            if (!(isJumping && canStartJump)) { // Player landed jump but is still holding ctrl
                jump();
                idle = false;
            }
        } else {
            isJumping = false;
        }

        if (state[SDL_SCANCODE_RIGHT]) {
            walk(RIGHT);
            idle = false;
        } else if (state[SDL_SCANCODE_LEFT]) {
            walk(LEFT);
            idle = false;
        } else if (state[SDL_SCANCODE_UP]) {
            walk(UP);
            idle = false;
        } else if (state[SDL_SCANCODE_DOWN]) {
            walk(DOWN);
            idle = false;
        }

        if (idle)
            walk(STOP);
    } else {
        shoot();
    }

    // Apply gravity
    fall();

    // Check collision
    IsCollidingWithTiles();
}

void Player::animate(animStateEnum nextState) {
    if (state == nextState) {
        // Get next frame
        frame++;
    } else {
        // Start new animation
        frame = 0;
        state = nextState;
    }

    if (frame == ANIMS[state].size() * FRAMETIME)
        frame = 0;

    srcClip = &ANIMS[state][frame / FRAMETIME];
}

void Player::draw() {
    // Now that all decisions have been made, finally update player location
    hitbox.x += xVel;
    hitbox.y += yVel;

    // Center the hitbox (horizontally) inside the displayed frame
    int offsetX = srcClip->w / 2 - TILE_WIDTH / 2;
    int destX = hitbox.x - offsetX;

    gKeenTexture->render(destX, hitbox.y, srcClip); // Later: Need an offset for the frame. Hitbox and graphic will not match 100%

    xVel = 0;
    //yVel = 0;
}

// Getters and setters

void Player::set_ammo(int x) { ammo = x; }
void Player::set_xVel(int x) { xVel = x; }
void Player::set_yVel(int y) { yVel = y; }
void Player::set_state(animStateEnum x) { state = x; }

int Player::get_ammo() { return ammo; }
int Player::get_xVel() { return xVel; }
int Player::get_yVel() { return yVel; }
int Player::get_state() { return state; }

SDL_Rect Player::get_hitbox() { return hitbox; }
