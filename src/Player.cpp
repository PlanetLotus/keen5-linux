// Player definition

#include "helpers.h"
#include "Player.h"

Player::Player() {
    ammo = 5;   // Might depend on difficulty level
    xVel = 0;
    yVel = 0;

    frameWidth = 64;
    frameHeight = 80;
    frame = 0;
    state = STANDR;
}

void Player::shoot() {
    return;
}

void Player::walk(directionEnum dir) {
    return;
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

void Player::draw() {
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
    }

    // Go to next frame
    frame++;
    if (frame >= 5)
        frame = 1;

    // Walk right frames
    SDL_Rect tempRect;
    tempRect.x = frame * frameWidth;
    tempRect.y = 0;
    tempRect.w = frameWidth;
    tempRect.h = frameHeight;

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

int Player::get_frameWidth() { return frameWidth; }
int Player::get_frameHeight() { return frameHeight; }
