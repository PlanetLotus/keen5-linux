#include "Tile.h"
#include "globals.h"

Tile::Tile(int x, int y, int tileType) {
    destBox.x = x;
    destBox.y = y;

    destBox.w = TILE_WIDTH;
    destBox.h = TILE_HEIGHT;

    type = tileType;
}

void Tile::render(int destX, int destY, SDL_Rect& camera) {
    gMaskTexture->render(destBox.x, destBox.y, &gTileClips[type]);
}

int Tile::getType() { return type; }
SDL_Rect Tile::getBox() { return destBox; }
