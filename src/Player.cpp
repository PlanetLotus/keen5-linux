// Player definition

#include "helpers.h"
#include "Player.h"
#include "globals.h"

Player::Player() {
    ammo = 5;   // Might depend on difficulty level
    xVel = 0;
    yVel = 0;
    yAccel = 0;

    hitbox.x = 40;
    hitbox.y = 16;
    hitbox.w = 64;
    hitbox.h = 80;

    frame = 0;
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

bool Player::is_colliding(SDL_Rect a, SDL_Rect b) {
    int leftA = a.x;
    int rightA = a.x + a.w;
    int topA = a.y;
    int bottomA = a.y + a.h;

    int leftB = b.x;
    int rightB = b.x + b.w;
    int topB = b.y;
    int bottomB = b.y + b.h;

    // This strategy doesn't tell me which side it collided with
    //if (bottomA <= topB)

    return false;
}

bool Player::is_colliding_with_tiles() {
    // Hitbox after update
    SDL_Rect nextHitbox = { hitbox.x + xVel, hitbox.y + yVel, hitbox.w, hitbox.h };

    // Loop through tiles and check collision for each
    for (unsigned int i=0; i<gTiles.size(); i++) {
        if (is_colliding(nextHitbox, gTiles[i]->getBox())) {
            // Stop moving!
        }
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
    } else {
        walk(STOP);
    }

    // Apply gravity
    //fall();

    // Check collision
}

void Player::draw() {
    // Could use an associative array where key=state, value=SDL_Rect clip
    SDL_Rect srcClip = {0, 0, hitbox.w, hitbox.h};

    // Walk right
    hitbox.x += xVel;
    hitbox.y += yVel;
    gKeenTexture->render(hitbox.x, hitbox.y, &srcClip); // Later: Need an offset for the frame. Hitbox and graphic will not match 100%

    xVel = 0;
    //yVel = 0;

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
