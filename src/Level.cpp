#include "Enemy.h"
#include "Level.h"
#include "Platform.h"
#include "Player.h"

using namespace std;

Level::Level(
    int width, int height,
    int tilesWide, int tilesTall,
    vector< vector<Tile*> > tiles,
    vector<FireSpinner*> deadlyTileBatch,
    vector<BackgroundTile*> backgroundTiles,
    vector<Enemy*> enemyBatch,
    vector<Item*> itemBatch,
    vector<Platform*> platformBatch,
    int keenSpawnX, int keenSpawnY
) {
    this->width = width;
    this->height = height;
    this->tilesWide = tilesWide;
    this->tilesTall = tilesTall;
    this->tiles = tiles;
    this->deadlyTileBatch = deadlyTileBatch;
    this->backgroundTiles = backgroundTiles;
    this->enemyBatch = enemyBatch;
    this->itemBatch = itemBatch;
    this->platformBatch = platformBatch;

    player = new Player(keenSpawnX, keenSpawnY);

    for (unsigned int i = 0; i < enemyBatch.size(); i++)
        enemyBatch[i]->setPlayer(player);

    for (unsigned int i = 0; i < platformBatch.size(); i++)
        platformBatch[i]->setPlayer(player);
}

int Level::getWidth() const { return width; }
int Level::getHeight() const { return height; }
int Level::getTilesWide() const { return tilesWide; }
int Level::getTilesTall() const { return tilesTall; }
Player* Level::getPlayer() const { return player; }
vector< vector<Tile*> > Level::getTiles() const { return tiles; }
vector<FireSpinner*> Level::getDeadlyTileBatch() const { return deadlyTileBatch; }
vector<BackgroundTile*> Level::getBackgroundTiles() const { return backgroundTiles; }
vector<Enemy*> Level::getEnemies() const { return enemyBatch; }
vector<Item*> Level::getItems() const { return itemBatch; }
vector<Platform*> Level::getPlatforms() const { return platformBatch; }
