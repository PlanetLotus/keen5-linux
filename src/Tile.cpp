#include "Tile.h"
#include "globals.h"

Tile::Tile(int x, int y, int tileType) {
    srcBox.x = x;
    srcBox.y = y;

    srcBox.w = TILE_WIDTH;
    srcBox.h = TILE_HEIGHT;

    type = tileType;
}

void Tile::render(int destX, int destY, SDL_Rect& camera) {
    // Box is the source (x,y) location
    gMaskTexture->render(destX, destY, &srcBox);
}

int Tile::getType() { return type; }
SDL_Rect Tile::getBox() { return srcBox; }
