#ifndef LEVEL_H
#define LEVEL_H

#include <vector>

class BackgroundTile;
class Enemy;
class Player;
class Tile;

class Level {
    private:
        int width;
        int height;
        int tilesWide;
        int tilesTall;

        int tileCountLayer1;
        int tileCountLayer2;

        int keenSpawnX;
        int keenSpawnY;
        Player* player;

        // 2D vector of tiles
        std::vector< std::vector<Tile*> > tiles;

        // 1D vector of backgrounds
        std::vector<BackgroundTile*> backgroundTiles;

        std::vector<Enemy*> enemyBatch;

    public:
        Level(
            int width, int height,
            int tilesWide, int tilesTall,
            int tileCountLayer1, int tileCountLayer2,
            std::vector< std::vector<Tile*> > tiles,
            std::vector<BackgroundTile*> backgroundTiles,
            std::vector<Enemy*> enemyBatch,
            int keenSpawnX, int keenSpawnY
        );
        int getWidth() const;
        int getHeight() const;
        int getTilesWide() const;
        int getTilesTall() const;
        Player* getPlayer() const;
        std::vector< std::vector<Tile*> > getTiles() const;
        std::vector<BackgroundTile*> getBackgroundTiles() const;
        std::vector<Enemy*> getEnemies() const;
};

#endif
