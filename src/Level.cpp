#include "Level.h"

Level::Level(
    int width, int height,
    int tilesWide, int tilesTall,
    int tileCountLayer1, int tileCountLayer2,
    std::vector< std::vector<Tile*> > tiles
) {
    this->width = width;
    this->height = height;
    this->tilesWide = tilesWide;
    this->tilesTall = tilesTall;
    this->tiles = tiles;
}

int Level::getWidth() { return width; }
int Level::getHeight() { return height; }
int Level::getTilesWide() { return tilesWide; }
int Level::getTilesTall() { return tilesTall; }
