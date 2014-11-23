#ifndef LEVEL_H
#define LEVEL_H

#include <vector>
#include "BackgroundTile.h"
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

        // 1D vector of backgrounds
        std::vector<BackgroundTile*> backgroundTiles;

    public:
        Level(
            int width, int height,
            int tilesWide, int tilesTall,
            int tileCountLayer1, int tileCountLayer2,
            std::vector< std::vector<Tile*> > tiles,
            std::vector<BackgroundTile*> backgroundTiles
        );
        int getWidth() const;
        int getHeight() const;
        int getTilesWide() const;
        int getTilesTall() const;
        std::vector< std::vector<Tile*> > getTiles() const;
        std::vector<BackgroundTile*> getBackgroundTiles() const;
};

#endif
