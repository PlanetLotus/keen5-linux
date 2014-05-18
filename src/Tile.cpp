#include "globals.h"
#include "Tile.h"

Tile::Tile(int srcX, int srcY, int destX, int destY, bool collideT, bool collideR, bool collideB, bool collideL,
        bool isPole=false, bool hidesKeen=false) {
    srcBox.x = srcX;
    srcBox.y = srcY;

    srcBox.w = TILE_WIDTH;
    srcBox.h = TILE_HEIGHT;

    destBox.x = destX;
    destBox.y = destY;

    destBox.w = TILE_WIDTH;
    destBox.h = TILE_HEIGHT;

    collideTop = collideT;
    collideRight = collideR;
    collideBottom = collideB;
    collideLeft = collideL;

    hasCollision = collideT || collideR || collideB || collideL ? true : false;

    IsPole = isPole;
    HidesKeen = hidesKeen;
}

void Tile::render(int destX, int destY, SDL_Rect& camera) {
    gMaskTexture->render(destBox.x, destBox.y, &srcBox);
}

SDL_Rect Tile::getBox() { return destBox; }
bool Tile::HasCollision() { return hasCollision; }
bool Tile::CollideTop() { return collideTop; }
bool Tile::CollideRight() { return collideRight; }
bool Tile::CollideBottom() { return collideBottom; }
bool Tile::CollideLeft() { return collideLeft; }
