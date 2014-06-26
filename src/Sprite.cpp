#include "globals.h"
#include "helpers.h"
#include "Sprite.h"
#include "Tile.h"

using namespace std;

Sprite::~Sprite() {
    delete srcClip;
}

void Sprite::update() {}

void Sprite::draw() {}

TileCollisionInfo Sprite::CheckTileCollisionLR() {
    TileCollisionInfo tci;

    SDL_Rect nextHitbox = { hitbox.x + (int)xVel, hitbox.y, hitbox.w, hitbox.h };

    int minRow = nextHitbox.y / TILE_HEIGHT;
    int maxRow = (nextHitbox.y + nextHitbox.h) / TILE_HEIGHT;

    // TODO: Fix this logic. Currently this addresses the case where, if the bottommost point
    // of player and topmost point of tile are equal, it should NOT check this row.
    if ((nextHitbox.y + nextHitbox.h) % TILE_WIDTH == 0 && maxRow > 0)
        maxRow--;

    // Don't let maxRow go out of bounds
    if (maxRow > TILES_TALL-1) maxRow = TILES_TALL-1;

    if (xVel > 0) {
        // Check R collision
        tci.IsRightChecked = true;
        tci.TileCollidingWithRight = GetTileCollidingWithRight(minRow, maxRow, nextHitbox);
    } else {
        // Check L collision
        tci.IsLeftChecked = true;
        tci.TileCollidingWithLeft = GetTileCollidingWithLeft(minRow, maxRow, nextHitbox);
    }
    return tci;
}

TileCollisionInfo Sprite::CheckTileCollisionTB() {
    TileCollisionInfo tci;

    SDL_Rect nextHitbox = { hitbox.x + (int)xVel, hitbox.y + (int)yVel, hitbox.w, hitbox.h };

    int minCol = nextHitbox.x / TILE_WIDTH;
    int maxCol = (nextHitbox.x + nextHitbox.w) / TILE_WIDTH;

    // TODO: Fix this logic. Currently this addresses the case where, if the rightmost point
    // of player and leftmost point of tile are equal, it should NOT check this column.
    if ((nextHitbox.x + nextHitbox.w) % TILE_WIDTH == 0 && maxCol > 0)
        maxCol--;

    // Don't let maxCol go out of bounds
    if (maxCol > TILES_WIDE-1) maxCol = TILES_WIDE-1;

    if (yVel > 0) {
        // Check B collision
        tci.IsBottomChecked = true;
        tci.TileCollidingWithBottom = GetTileCollidingWithBottom(minCol, maxCol, nextHitbox);
    } else {
        // Check T collision
        tci.IsTopChecked = true;
        tci.TileCollidingWithTop = GetTileCollidingWithTop(minCol, maxCol, nextHitbox);
    }
    return tci;
}

Tile* Sprite::GetTileCollidingWithRight(int minRow, int maxRow, SDL_Rect nextHitbox) {
    int col = nextHitbox.x / TILE_WIDTH;

    for (unsigned int i = col; i < gTiles.size(); i++) {
        for (int j = minRow; j <= maxRow; j++) {
            Tile* tile = gTiles[i][j];

            if (tile != NULL &&
                tile->CollideLeft() &&
                IsRightColliding(hitbox, nextHitbox, tile->getBox())) {

                return tile;
            }
        }
    }
    return NULL;
}

Tile* Sprite::GetTileCollidingWithLeft(int minRow, int maxRow, SDL_Rect nextHitbox) {
    int col = (nextHitbox.x + nextHitbox.w) / TILE_WIDTH;

    for (int i = col; i >= 0; i--) {
        for (int j = minRow; j <= maxRow; j++) {
            Tile* tile = gTiles[i][j];

            if (tile != NULL &&
                tile->CollideRight() &&
                IsLeftColliding(hitbox, nextHitbox, tile->getBox())) {

                return tile;
            }
        }
    }
    return NULL;
}

Tile* Sprite::GetTileCollidingWithBottom(int minCol, int maxCol, SDL_Rect nextHitbox) {
    int row = nextHitbox.y / TILE_HEIGHT;

    for (int i = minCol; i <= maxCol; i++) {
        for (unsigned int j = row; j < gTiles[i].size(); j++) {
            Tile* tile = gTiles[i][j];

            if (tile != NULL &&
                tile->CollideTop() &&
                IsBottomColliding(hitbox, nextHitbox, tile->getBox())) {

                return tile;
            }
        }
    }
    return NULL;
}

Tile* Sprite::GetTileCollidingWithTop(int minCol, int maxCol, SDL_Rect nextHitbox) {
    int row = (nextHitbox.y + nextHitbox.h) / TILE_HEIGHT;

    for (int i = minCol; i <= maxCol; i++) {
        for (int j = row; j >= 0; j--) {
            Tile* tile = gTiles[i][j];

            if (tile != NULL &&
                tile->CollideBottom() &&
                IsTopColliding(hitbox, nextHitbox, tile->getBox())) {

                return tile;
            }
        }
    }
    return NULL;
}
