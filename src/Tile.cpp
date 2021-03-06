#include "globals.h"
#include "helpers.h"
#include "Texture.h"
#include "Tile.h"

Tile::Tile(int srcX, int srcY, int destX, int destY, int lHeight, int rHeight,
    bool collideT, bool collideR, bool collideB, bool collideL,
    int layer, bool isPole=false, bool isPoleEdge=false, bool isEdge=false, bool isDeadly=false) {
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

    this->layer = layer;

    this->isPole = isPole;
    this->isPoleEdge = isPoleEdge;
    this->isEdge = isEdge;
    this->isDeadly = isDeadly;
}

void Tile::draw(Texture* texture, SDL_Rect cameraBox) {
    if (isTileLocationOnScreen(&destBox, &cameraBox))
        texture->render(destBox.x - cameraBox.x, destBox.y - cameraBox.y, &srcBox);
}

bool Tile::isColliding(TileProperty tileProperty, SDL_Rect hitbox, SDL_Rect nextHitbox) {
    if (tileProperty == TileProperty::ISPOLE) {
        return (isPole || isPoleEdge) && (isRightColliding(hitbox, nextHitbox, destBox) ||
            isLeftColliding(hitbox, nextHitbox, destBox));
    }

    if (isSloped && tileProperty == TileProperty::TOP) {
        return isBottomColliding(hitbox, nextHitbox, destBox, true);
    }

    if (tileProperty == TileProperty::LEFT)
        return collideLeft && isRightColliding(hitbox, nextHitbox, destBox);

    if (tileProperty == TileProperty::RIGHT)
        return collideRight && isLeftColliding(hitbox, nextHitbox, destBox);

    if (tileProperty == TileProperty::TOP)
        return collideTop && isBottomColliding(hitbox, nextHitbox, destBox);

    if (tileProperty == TileProperty::BOTTOM)
        return collideBottom && isTopColliding(hitbox, nextHitbox, destBox);

    return false;
}

bool Tile::isTouching(TileProperty tileProperty, SDL_Rect nextHitbox) {
    if (tileProperty == TileProperty::LEFT)
        return destBox.x == nextHitbox.x + nextHitbox.w;

    if (tileProperty == TileProperty::RIGHT)
        return destBox.x + destBox.w == nextHitbox.x;

    if (tileProperty == TileProperty::TOP)
        return destBox.y == nextHitbox.y + nextHitbox.h;

    if (tileProperty == TileProperty::BOTTOM)
        return destBox.y + destBox.h  == nextHitbox.y;

    return false;
}

SDL_Rect Tile::getBox() { return destBox; }
float Tile::getSlope() { return slope; }
int Tile::getLeftHeight() { return leftHeight; }
int Tile::getRightHeight() { return rightHeight; }
bool Tile::getIsSloped() { return isSloped; }
bool Tile::getIsEdge() { return isEdge; }
bool Tile::getIsDeadly() { return isDeadly; }
bool Tile::getHasCollision() { return hasCollision; }
bool Tile::getCollideTop() { return collideTop; }
bool Tile::getCollideRight() { return collideRight; }
bool Tile::getCollideBottom() { return collideBottom; }
bool Tile::getCollideLeft() { return collideLeft; }
