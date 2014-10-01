#include "Level.h"

using namespace std;

Level::Level(
    int width, int height,
    int tilesWide, int tilesTall,
    int tileCountLayer1, int tileCountLayer2,
    vector< vector<Tile*> > tiles
) {
    this->width = width;
    this->height = height;
    this->tilesWide = tilesWide;
    this->tilesTall = tilesTall;
    this->tiles = tiles;
}

int Level::getWidth() const { return width; }
int Level::getHeight() const { return height; }
int Level::getTilesWide() const { return tilesWide; }
int Level::getTilesTall() const { return tilesTall; }
vector< vector<Tile*> > Level::getTiles() const { return tiles; }