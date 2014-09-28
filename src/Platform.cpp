#include "globals.h"
#include "Platform.h"

Platform::Platform() {
    hitbox.x = TILE_WIDTH * 23;
    hitbox.y = TILE_HEIGHT * 9;
    hitbox.w = TILE_WIDTH * 2;
    hitbox.h = TILE_HEIGHT;

    srcRect.x = TILE_WIDTH * 14;
    srcRect.y = TILE_HEIGHT * 8;
    srcRect.w = TILE_WIDTH * 2;
    srcRect.h = TILE_HEIGHT;

    xVel = 0;
    yVel = 0;
}

void Platform::update() {
}

void Platform::draw(SDL_Rect cameraBox) {
    gKeenTexture->render(hitbox.x - cameraBox.x, hitbox.y - cameraBox.y, &srcRect);
}