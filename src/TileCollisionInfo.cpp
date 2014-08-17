#include "TileCollisionInfo.h"

using namespace std;

TileCollisionInfo::TileCollisionInfo() {
    isTopChecked = false;
    isBottomChecked = false;
    isRightChecked = false;
    isLeftChecked = false;

    tileCollidingWithTop = NULL;
    tileCollidingWithBottom = NULL;
    tileCollidingWithLeft = NULL;
    tileCollidingWithRight = NULL;
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
    return tileCollidingWithTop != NULL;
}

bool TileCollisionInfo::isBottomColliding() {
    return tileCollidingWithBottom != NULL;
}

bool TileCollisionInfo::isRightColliding() {
    return tileCollidingWithRight != NULL;
}

bool TileCollisionInfo::isLeftColliding() {
    return tileCollidingWithLeft != NULL;
}
