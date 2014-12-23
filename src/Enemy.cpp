#include "Enemy.h"
#include "Player.h"
#include "Tile.h"

bool Enemy::isCollidingWithPlayer() {
    return isUnitColliding(keen->getBox());
}

Tile* Enemy::getTileUnderFeet() {
    int enemyBottom = hitbox.y + hitbox.h;

    if (enemyBottom % TILE_HEIGHT != 0)
        return nullptr;

    int enemyRight = hitbox.x + hitbox.w;
    unsigned int leftCol = hitbox.x / TILE_WIDTH;
    unsigned int rightCol = (enemyRight + TILE_WIDTH) / TILE_WIDTH;
    int tileRow = enemyBottom / TILE_HEIGHT;
    Tile* tile = nullptr;

    // If moving left, return leftmost tile. If moving right, return rightmost
    // If the unit is pushed by something else, will that mess up this logic?
    for (unsigned int i = leftCol; i < rightCol; i++) {
        tile = tilesRef[i][tileRow];
        if (tile != nullptr) {
            if (facing == Facing::LEFT)
                return tile;
        }
    }
    return tile;
}

void Enemy::setPlayer(Player* player) { keen = player; }
