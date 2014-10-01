#ifndef LEVEL_H
#define LEVEL_H

#include <vector>
#include "Tile.h"

class Level {
    private:
        int width;
        int height;
        int tilesWide;
        int tilesTall;

        int tileCountLayer1;
        int tileCountLayer2;

        // 2D vector of tiles
        std::vector< std::vector<Tile*> > tiles;

    public:
        int getWidth();
        int getHeight();
        int getTilesWide();
        int getTilesTall();
};

#endif
