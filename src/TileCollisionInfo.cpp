#include "TileCollisionInfo.h"

using namespace std;

TileCollisionInfo::TileCollisionInfo() {
    IsTopChecked = false;
    IsBottomChecked = false;
    IsRightChecked = false;
    IsLeftChecked = false;

    TileCollidingWithTop = make_pair(-1, -1);
    TileCollidingWithBottom = make_pair(-1, -1);
    TileCollidingWithLeft = make_pair(-1, -1);
    TileCollidingWithRight = make_pair(-1, -1);
}

bool TileCollisionInfo::IsTopColliding() {
    return TileCollidingWithTop.first != -1;
}

bool TileCollisionInfo::IsBottomColliding() {
    return TileCollidingWithBottom.first != -1;
}

bool TileCollisionInfo::IsRightColliding() {
    return TileCollidingWithRight.first != -1;
}

bool TileCollisionInfo::IsLeftColliding() {
    return TileCollidingWithLeft.first != -1;
}
