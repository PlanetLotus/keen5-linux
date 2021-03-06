#ifndef LEVEL_H
#define LEVEL_H

#include <vector>

class BackgroundTile;
class Enemy;
class EnemyLaserManager;
class FireSpinner;
class Item;
class Platform;
class Player;
class StatsManager;
class Tile;

class Level {
    private:
        int width;
        int height;
        int tilesWide;
        int tilesTall;

        int keenSpawnX;
        int keenSpawnY;
        Player* player;

        EnemyLaserManager* enemyLaserManager;

        // 2D vector of tiles
        std::vector< std::vector<Tile*> > tiles;

        // 1D vector of backgrounds
        std::vector<BackgroundTile*> backgroundTiles;

        // Tiles that actually need updated
        std::vector<FireSpinner*> deadlyTileBatch;

        std::vector<Enemy*> enemyBatch;
        std::vector<Item*> itemBatch;
        std::vector<Platform*> platformBatch;

    public:
        Level(
            int width, int height,
            int tilesWide, int tilesTall,
            EnemyLaserManager* enemyLaserManager,
            StatsManager* statsManager,
            std::vector< std::vector<Tile*> > tiles,
            std::vector<FireSpinner*> deadlyTileBatch,
            std::vector<BackgroundTile*> backgroundTiles,
            std::vector<Enemy*> enemyBatch,
            std::vector<Item*> itemBatch,
            std::vector<Platform*> platformBatch,
            int keenSpawnX, int keenSpawnY
        );
        int getWidth() const;
        int getHeight() const;
        int getTilesWide() const;
        int getTilesTall() const;
        EnemyLaserManager* getEnemyLaserManager() const;
        Player* getPlayer() const;
        std::vector< std::vector<Tile*> > getTiles() const;
        std::vector<FireSpinner*> getDeadlyTileBatch() const;
        std::vector<BackgroundTile*> getBackgroundTiles() const;
        std::vector<Enemy*> getEnemies() const;
        std::vector<Item*> getItems() const;
        std::vector<Platform*> getPlatforms() const;
};

#endif
