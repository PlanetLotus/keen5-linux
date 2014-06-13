#ifndef TILECOLLISIONINFO_H
#define TILECOLLISIONINFO_h

#include <utility>

class TileCollisionInfo {
    public:
        TileCollisionInfo();

        bool IsTopChecked;
        bool IsBottomChecked;
        bool IsRightChecked;
        bool IsLeftChecked;

        bool IsTopColliding();
        bool IsBottomColliding();
        bool IsRightColliding();
        bool IsLeftColliding();

        std::pair<int, int> TileCollidingWithTop;
        std::pair<int, int> TileCollidingWithBottom;
        std::pair<int, int> TileCollidingWithLeft;
        std::pair<int, int> TileCollidingWithRight;
};

#endif
