#include "Tile.h"
#include "TileCollisionInfo.h"

using namespace std;

TileCollisionInfo::TileCollisionInfo() {
    isTopChecked = false;
    isBottomChecked = false;
    isRightChecked = false;
    isLeftChecked = false;

    tileCollidingWithTop = nullptr;
    tileCollidingWithBottom = nullptr;
    tileCollidingWithLeft = nullptr;
    tileCollidingWithRight = nullptr;
}

TileCollisionInfo::TileCollisionInfo(TileCollisionInfo tciLR, TileCollisionInfo tciTB) {
    isLeftChecked = tciLR.isLeftChecked;
    isRightChecked = tciLR.isRightChecked;
    isTopChecked = tciTB.isTopChecked;
    isBottomChecked = tciTB.isBottomChecked;

    tileCollidingWithLeft = tciLR.tileCollidingWithLeft;
    tileCollidingWithRight = tciLR.tileCollidingWithRight;
    tileCollidingWithTop = tciLR.tileCollidingWithTop;
    tileCollidingWithBottom = tciLR.tileCollidingWithBottom;
}

bool TileCollisionInfo::isTopColliding() {
    return tileCollidingWithTop != nullptr;
}

bool TileCollisionInfo::isBottomColliding() {
    return tileCollidingWithBottom != nullptr;
}

bool TileCollisionInfo::isRightColliding() {
    return tileCollidingWithRight != nullptr;
}

bool TileCollisionInfo::isLeftColliding() {
    return tileCollidingWithLeft != nullptr;
}
