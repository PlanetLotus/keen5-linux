#include "globals.h"
#include "helpers.h"
#include "Platform.h"

Platform::Platform(Player* player) {
    hitbox.x = TILE_WIDTH * 23;
    hitbox.y = TILE_HEIGHT * 9;
    hitbox.w = TILE_WIDTH * 2;
    hitbox.h = TILE_HEIGHT;

    srcRect.x = TILE_WIDTH * 14;
    srcRect.y = TILE_HEIGHT * 8;
    srcRect.w = TILE_WIDTH * 2;
    srcRect.h = TILE_HEIGHT * 2;

    keen = player;

    xVel = 0;
    yVel = 0;
}

void Platform::update() {
    hitbox.x += xVel;
    hitbox.y += yVel;

    Platform* collidingPlatform = keen->getCollidingPlatform();
    if (collidingPlatform != NULL && collidingPlatform == this) {
        keen->pushX(xVel);
        keen->pushY(yVel);
    }
}

void Platform::draw(Texture* texture, SDL_Rect cameraBox) {
    // Center the hitbox vertically
    int offsetY = srcRect.h - TILE_HEIGHT;
    int destY = hitbox.y - offsetY;

    texture->render(hitbox.x - cameraBox.x, destY - cameraBox.y, &srcRect);
}

SDL_Rect Platform::getBox() { return hitbox; }
