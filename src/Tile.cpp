#include "globals.h"
#include "helpers.h"
#include "Tile.h"

Tile::Tile(int srcX, int srcY, int destX, int destY, int lHeight, int rHeight,
    bool collideT, bool collideR, bool collideB, bool collideL, int layer, bool isPole=false) {
    srcBox.x = srcX;
    srcBox.y = srcY;

    srcBox.w = TILE_WIDTH;
    srcBox.h = TILE_HEIGHT;

    destBox.x = destX;
    destBox.y = destY;

    destBox.w = TILE_WIDTH;
    destBox.h = TILE_HEIGHT;

    leftHeight = lHeight;
    rightHeight = rHeight;

    collideTop = collideT;
    collideRight = collideR;
    collideBottom = collideB;
    collideLeft = collideL;

    isSloped = leftHeight != 0 || rightHeight != 0;
    hasCollision = collideT || collideR || collideB || collideL ? true : false;

    if (isSloped)
        slope = (float)(rightHeight - leftHeight) / (float)TILE_HEIGHT;
    else
        slope = 0;

    Layer = layer;

    IsPole = isPole;
}

void Tile::render(SDL_Rect* camera) {
    gMaskTexture->render(destBox.x - camera->x, destBox.y - camera->y, &srcBox);
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

bool Tile::IsTouching(TileProperty tileProperty, SDL_Rect nextHitbox) {
    if (tileProperty == LEFT)
        return destBox.x == nextHitbox.x + nextHitbox.w;

    if (tileProperty == RIGHT)
        return destBox.x + destBox.w == nextHitbox.x;

    if (tileProperty == TOP)
        return destBox.y == nextHitbox.y + nextHitbox.h;

    if (tileProperty == BOTTOM)
        return destBox.y + destBox.h  == nextHitbox.y;

    return false;
}

SDL_Rect Tile::getBox() { return destBox; }
float Tile::GetSlope() { return slope; }
int Tile::GetLeftHeight() { return leftHeight; }
int Tile::GetRightHeight() { return rightHeight; }
bool Tile::IsSloped() { return isSloped; }
bool Tile::HasCollision() { return hasCollision; }
bool Tile::CollideTop() { return collideTop; }
bool Tile::CollideRight() { return collideRight; }
bool Tile::CollideBottom() { return collideBottom; }
bool Tile::CollideLeft() { return collideLeft; }
