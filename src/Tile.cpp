#include "globals.h"
#include "helpers.h"
#include "Tile.h"

Tile::Tile(int srcX, int srcY, int destX, int destY, bool collideT, bool collideR, bool collideB, bool collideL,
    int layer, bool isPole=false) {
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

    Layer = layer;

    IsPole = isPole;
}

void Tile::render(SDL_Rect& camera) {
    gMaskTexture->render(destBox.x, destBox.y, &srcBox);
}

bool Tile::IsColliding(TileProperty tileProperty, SDL_Rect hitbox, SDL_Rect nextHitbox) {
    if (tileProperty == ISPOLE) {
        return IsPole && (IsRightColliding(hitbox, nextHitbox, destBox) ||
            IsLeftColliding(hitbox, nextHitbox, destBox));
    }

    if (tileProperty == LEFT)
        return collideLeft && IsRightColliding(hitbox, nextHitbox, destBox);

    if (tileProperty == RIGHT)
        return collideRight && IsLeftColliding(hitbox, nextHitbox, destBox);

    if (tileProperty == TOP)
        return collideTop && IsBottomColliding(hitbox, nextHitbox, destBox);

    if (tileProperty == BOTTOM)
        return collideBottom && IsTopColliding(hitbox, nextHitbox, destBox);

    return false;
}

SDL_Rect Tile::getBox() { return destBox; }
bool Tile::HasCollision() { return hasCollision; }
bool Tile::CollideTop() { return collideTop; }
bool Tile::CollideRight() { return collideRight; }
bool Tile::CollideBottom() { return collideBottom; }
bool Tile::CollideLeft() { return collideLeft; }
