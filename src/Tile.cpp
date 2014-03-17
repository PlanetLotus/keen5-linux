#include "Tile.h"
#include "globals.h"

Tile::Tile(int srcX, int srcY, int destX, int destY) {
    srcBox.x = srcX;
    srcBox.y = srcY;

    srcBox.w = TILE_WIDTH;
    srcBox.h = TILE_HEIGHT;

    destBox.x = destX;
    destBox.y = destY;

    destBox.w = TILE_WIDTH;
    destBox.h = TILE_HEIGHT;
}

void Tile::render(int destX, int destY, SDL_Rect& camera) {
    gMaskTexture->render(destBox.x, destBox.y, &srcBox);
}

SDL_Rect Tile::getBox() { return destBox; }
