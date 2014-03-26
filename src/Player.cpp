// Player definition

#include "helpers.h"
#include "Player.h"

Player::Player() {
    ammo = 5;   // Might depend on difficulty level
    xVel = 0;
    yVel = 0;
    yAccel = 0;

    hitbox.x = 0;
    hitbox.y = 0;
    //hitbox.w = 64;
    //hitbox.h = 80;
    hitbox.w = TILE_WIDTH;
    hitbox.h = TILE_HEIGHT * 2;

    frame = 1;
    state = STANDR;
}

void Player::shoot() {
    return;
}

void Player::walk(directionEnum dir) {
    switch (dir) {
        case RIGHT:
            state = WALKR;
            xVel = 5;
            break;
        case LEFT:
            state = WALKL;
            xVel = -5;
            break;
        case UP:
            yVel = -5;
            break;
        case DOWN:
            yVel = 5;
            break;
        case STOP:
            state = STANDR;
            xVel = 0;
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
    // Some idea for how to handle update... (NOT IMPLEMENTED)
    // Use "velocity" as a calculation of how far to move per loop
    // Assign or increment position by velocity AFTER all updates have been calculated (walking, falling, collision, etc.)
    // Reset velocity to 0 afterward

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

void Player::draw() {
    // Could use an associative array where key=state, value=SDL_Rect clip
    SDL_Rect srcClip;
    printf("%d\n", frame);

    if (frame == 1)
        srcClip = WALKR1;
    else if (frame == 2)
        srcClip = WALKR2;
    else if (frame == 3)
        srcClip = WALKR3;
    else if (frame == 4) {
        srcClip = WALKR4;
        frame = 0;
    }
    frame++;

    // Walk right
    hitbox.x += xVel;
    hitbox.y += yVel;
    gKeenTexture->render(hitbox.x, hitbox.y, &srcClip); // Later: Need an offset for the frame. Hitbox and graphic will not match 100%

    xVel = 0;
    yVel = 0;

    /*
    if (state == STANDR) {
        // Stand still
        frame = 0;
        SDL_Rect tempRect;
        tempRect.x = 0;
        tempRect.y = 0;
        tempRect.w = frameWidth;
        tempRect.h = frameHeight;
        //apply_surface(50, 50, keen, screen, &tempRect);
        return;
    } else if (state == WALKR) {
    */

    // Go to next frame
    /*
    frame++;
    if (frame >= 5)
        frame = 1;

    // Walk right frames
    SDL_Rect tempRect;
    tempRect.x = frame * frameWidth;
    tempRect.y = 0;
    tempRect.w = frameWidth;
    tempRect.h = frameHeight;
    */

    // Draw the character
    //apply_surface(50, 50, keen, screen, &tempRect);
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
