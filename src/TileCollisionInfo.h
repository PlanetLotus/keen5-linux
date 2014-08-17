#ifndef TILECOLLISIONINFO_H
#define TILECOLLISIONINFO_h

#include "Tile.h"

class TileCollisionInfo {
    public:
        TileCollisionInfo();
        TileCollisionInfo(TileCollisionInfo tciLR, TileCollisionInfo tciTB);

        bool isTopChecked;
        bool isBottomChecked;
        bool isRightChecked;
        bool isLeftChecked;

        bool isTopColliding();
        bool isBottomColliding();
        bool isRightColliding();
        bool isLeftColliding();

        Tile* tileCollidingWithTop;
        Tile* tileCollidingWithBottom;
        Tile* tileCollidingWithLeft;
        Tile* tileCollidingWithRight;
};

#endif
