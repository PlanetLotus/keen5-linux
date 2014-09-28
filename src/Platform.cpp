#include "globals.h"
#include "Platform.h"

Platform::Platform() {
    hitbox.x = TILE_WIDTH;
    hitbox.x = TILE_HEIGHT;
    hitbox.w = TILE_WIDTH * 2;
    hitbox.h = TILE_HEIGHT;

    SDL_Rect frame = { TILE_WIDTH * 14, TILE_HEIGHT * 8, TILE_WIDTH * 2, TILE_HEIGHT };
    srcClip = &frame;

    xVel = 0;
    yVel = 0;
}

void Platform::update() {
}

void Platform::draw(SDL_Rect cameraBox) {
}
