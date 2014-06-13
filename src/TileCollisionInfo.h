#ifndef TILECOLLISIONINFO_H
#define TILECOLLISIONINFO_h

#include "Tile.h"

class TileCollisionInfo {
    public:
        TileCollisionInfo();
        TileCollisionInfo(TileCollisionInfo tciLR, TileCollisionInfo tciTB);

        bool IsTopChecked;
        bool IsBottomChecked;
        bool IsRightChecked;
        bool IsLeftChecked;

        bool IsTopColliding();
        bool IsBottomColliding();
        bool IsRightColliding();
        bool IsLeftColliding();

        Tile* TileCollidingWithTop;
        Tile* TileCollidingWithBottom;
        Tile* TileCollidingWithLeft;
        Tile* TileCollidingWithRight;
};

#endif
