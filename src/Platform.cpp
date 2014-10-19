#include "globals.h"
#include "helpers.h"
#include "Platform.h"

Platform::Platform(Player* player) {
    hitbox.x = TILE_WIDTH * 24;
    hitbox.y = TILE_HEIGHT * 27;
    hitbox.w = TILE_WIDTH * 2;
    hitbox.h = TILE_HEIGHT;

    srcRect.x = TILE_WIDTH * 14;
    srcRect.y = TILE_HEIGHT * 8;
    srcRect.w = TILE_WIDTH * 2;
    srcRect.h = TILE_HEIGHT * 2;

    keen = player;

    xVel = 0;
    yVel = -3;
}

bool Platform::playerIsStandingOnThis(SDL_Rect keenBox) {
    int platformLeft = hitbox.x;
    int platformRight = hitbox.x + hitbox.w;
    int platformTop = hitbox.y;
    int keenRight = keenBox.x + keenBox.w;
    int keenLeft = keenBox.x;
    int keenBottom = keenBox.y + keenBox.h;

    if (keenRight <= platformLeft) return false;
    if (keenLeft >= platformRight) return false;

    return keenBottom == platformTop;
}

void Platform::update() {
    // 0) Reset platformStandingOn if it points to this platform
    if (keen->platformStandingOn == this)
        keen->platformStandingOn = NULL;

    // 1) Check if player is standing on this platform. If so, store handle to it in Player.
    SDL_Rect keenBox = keen->getBox();
    if (playerIsStandingOnThis(keenBox))
        keen->platformStandingOn = this;

    // 2) Update Platform
    hitbox.x += xVel;
    hitbox.y += yVel;
}

void Platform::draw(Texture* texture, SDL_Rect cameraBox) {
    // Center the hitbox vertically
    int offsetY = srcRect.h - TILE_HEIGHT;
    int destY = hitbox.y - offsetY;

    texture->render(hitbox.x - cameraBox.x, destY - cameraBox.y, &srcRect);
}

SDL_Rect Platform::getBox() { return hitbox; }
float Platform::getXVel() { return xVel; }
float Platform::getYVel() { return yVel; }
