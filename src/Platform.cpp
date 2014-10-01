#include "globals.h"
#include "Platform.h"

Platform::Platform(Player* player) {
    hitbox.x = TILE_WIDTH * 23;
    hitbox.y = TILE_HEIGHT * 9;
    hitbox.w = TILE_WIDTH * 2;
    hitbox.h = TILE_HEIGHT;

    srcRect.x = TILE_WIDTH * 14;
    srcRect.y = TILE_HEIGHT * 8;
    srcRect.w = TILE_WIDTH * 2;
    srcRect.h = TILE_HEIGHT;

    keen = player;

    xVel = 0;
    yVel = 0;
}

void Platform::update() {
}

void Platform::draw(Texture* texture, SDL_Rect cameraBox) {
    texture->render(hitbox.x - cameraBox.x, hitbox.y - cameraBox.y, &srcRect);
}
