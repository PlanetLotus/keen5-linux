#include "Tile.h"
#include "globals.h"

Tile::Tile(int x, int y, int tileType) {
    box.x = x;
    box.y = y;

    box.w = TILE_WIDTH;
    box.h = TILE_HEIGHT;

    type = tileType;
}

void Tile::render(SDL_Rect& camera) {
    gMaskTexture->render(box.x, box.y, &gTiles[type]->box);
}

int Tile::getType() { return type; }
SDL_Rect Tile::getBox() { return box; }
