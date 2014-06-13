#include "TileCollisionInfo.h"

using namespace std;

TileCollisionInfo::TileCollisionInfo() {
    IsTopChecked = false;
    IsBottomChecked = false;
    IsRightChecked = false;
    IsLeftChecked = false;

    TileCollidingWithTop = NULL;
    TileCollidingWithBottom = NULL;
    TileCollidingWithLeft = NULL;
    TileCollidingWithRight = NULL;
}

bool TileCollisionInfo::IsTopColliding() {
    return TileCollidingWithTop != NULL;
}

bool TileCollisionInfo::IsBottomColliding() {
    return TileCollidingWithBottom != NULL;
}

bool TileCollisionInfo::IsRightColliding() {
    return TileCollidingWithRight != NULL;
}

bool TileCollisionInfo::IsLeftColliding() {
    return TileCollidingWithLeft != NULL;
}
