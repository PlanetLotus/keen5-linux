// Player definition

#include "helpers.h"
#include "Player.h"

// Animation (srcClip) frames
SDL_Rect STANDL0 = {0, 0, TILE_WIDTH, TILE_HEIGHT * 2}; // Correct frames not set yet
SDL_Rect STANDR0 = {0, 0, TILE_WIDTH, TILE_HEIGHT * 2};

SDL_Rect WALKL0 = {TILE_WIDTH, 0, TILE_WIDTH + 1, TILE_HEIGHT * 2};     // Correct frames not set yet
SDL_Rect WALKL1 = {TILE_WIDTH * 3, 0, TILE_WIDTH + 3, TILE_HEIGHT * 2};
SDL_Rect WALKL2 = {TILE_WIDTH * 5, 0, TILE_WIDTH, TILE_HEIGHT * 2};
SDL_Rect WALKL3 = {TILE_WIDTH * 7, 0, TILE_WIDTH + 4, TILE_HEIGHT * 2};

SDL_Rect WALKR0 = {TILE_WIDTH, 0, TILE_WIDTH * 2, TILE_HEIGHT * 2};
SDL_Rect WALKR1 = {TILE_WIDTH * 3, 0, TILE_WIDTH * 2, TILE_HEIGHT * 2};
SDL_Rect WALKR2 = {TILE_WIDTH * 5, 0, TILE_WIDTH, TILE_HEIGHT * 2};
SDL_Rect WALKR3 = {TILE_WIDTH * 6, 0, TILE_WIDTH * 2, TILE_HEIGHT * 2};

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

// Array of animations
// Statically set...animStateEnum's value needs to match this array
// e.g. if ANIMS[0] == walk right, then animStateEnum[WALKR] == 0
// Purpose: To be dynamic in animate()
// Alternative: Could use an associative array where key=state, value=array of SDL_Rect clip
std::vector<SDL_Rect> ANIMS[4] = { STANDL_ANIM, STANDR_ANIM, WALKL_ANIM, WALKR_ANIM };

Player::Player() {
    ammo = 5;   // Might depend on difficulty level
    xVel = 0;
    yVel = 0;
    yAccel = 0;

    srcClip = NULL;

    hitbox.x = TILE_WIDTH;
    hitbox.y = TILE_HEIGHT;
    //hitbox.w = 64;
    //hitbox.h = 80;
    hitbox.w = TILE_WIDTH;
    hitbox.h = TILE_HEIGHT * 2;

    frame = 0;
    state = STANDR;
}

void Player::shoot() {
    return;
}

void Player::walk(directionEnum dir) {
    switch (dir) {
        case RIGHT:
            xVel = 5;
            animate(WALKR);
            break;
        case LEFT:
            //state = WALKL;
            xVel = -5;
            break;
        case UP:
            yVel = -5;
            break;
        case DOWN:
            yVel = 5;
            break;
        case STOP:
            xVel = 0;
            animate(STANDR);    // TODO: Won't always be R
            break;
    }
}

void Player::jump() {
    return;
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

void Player::fall() {
    yAccel = 2;

    if (yVel >= 10)
        return;

    yVel += yAccel;
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

    if (bottomA <= topB) return false;

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
    int maxCol = (int) ceil( (float) (nextHitbox.x + nextHitbox.w) / TILE_WIDTH);

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
    int maxCol = (int) ceil( (float) (nextHitbox.x + nextHitbox.w) / TILE_WIDTH);

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
    int maxRow = (int) ceil( (float) (nextHitbox.y + nextHitbox.h) / TILE_HEIGHT );

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
    int maxRow = (int) ceil( (float) (nextHitbox.y + nextHitbox.h) / TILE_HEIGHT );

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

    if (state[SDL_SCANCODE_RIGHT]) {
        walk(RIGHT);
    } else if (state[SDL_SCANCODE_LEFT]) {
        walk(LEFT);
    } else if (state[SDL_SCANCODE_UP]) {
        walk(UP);
    } else if (state[SDL_SCANCODE_DOWN]) {
        walk(DOWN);
    } else {
        walk(STOP);
    }

    // Apply gravity
    //fall();

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

    //printf("%d\n", state);
    srcClip = &ANIMS[state][frame / FRAMETIME];
}

void Player::draw() {
    // Now that all decisions have been made, finally update player location
    hitbox.x += xVel;
    hitbox.y += yVel;

    // Center the hitbox (horizontally) inside the displayed frame
    int offsetX = srcClip->w / 2 - TILE_WIDTH / 2;
    printf("%d\n", offsetX);
    int destX = hitbox.x - offsetX;

    gKeenTexture->render(destX, hitbox.y, srcClip); // Later: Need an offset for the frame. Hitbox and graphic will not match 100%

    xVel = 0;
    yVel = 0;
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
