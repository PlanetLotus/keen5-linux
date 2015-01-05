#include "globals.h"
#include "helpers.h"
#include "Level.h"
#include "MovingSprite.h"
#include "Tile.h"

using namespace std;

vector<Tile*> MovingSprite::getTilesToLeft() {
    SDL_Rect nextHitbox = getNextHitboxX();

    int col = (nextHitbox.x + nextHitbox.w) / TILE_WIDTH;
    int minRow = nextHitbox.y / TILE_HEIGHT;
    int maxRow = (nextHitbox.y + nextHitbox.h) / TILE_HEIGHT;

    // TODO: Fix this logic. Currently this addresses the case where, if the bottommost point
    // of player and topmost point of tile are equal, it should NOT check this row.
    if ((nextHitbox.y + nextHitbox.h) % TILE_WIDTH == 0 && maxRow > 0)
        maxRow--;

    // Don't let maxRow go out of bounds
    int tilesTall = currentLevelRef->getTilesTall();
    if (maxRow > tilesTall - 1) maxRow = tilesTall - 1;

    vector<Tile*> tilesToLeft;

    for (int i = col; i >= 0; i--) {
        for (int j = minRow; j <= maxRow; j++) {
            Tile* tile = tilesRef[i][j];

            if (tile != nullptr)
                tilesToLeft.push_back(tile);
        }
    }

    return tilesToLeft;
}

vector<Tile*> MovingSprite::getTilesToRight() {
    SDL_Rect nextHitbox = getNextHitboxX();

    int col = nextHitbox.x / TILE_WIDTH;
    int minRow = nextHitbox.y / TILE_HEIGHT;
    int maxRow = (nextHitbox.y + nextHitbox.h) / TILE_HEIGHT;

    // TODO: Fix this logic. Currently this addresses the case where, if the bottommost point
    // of player and topmost point of tile are equal, it should NOT check this row.
    if ((nextHitbox.y + nextHitbox.h) % TILE_WIDTH == 0 && maxRow > 0)
        maxRow--;

    // Don't let maxRow go out of bounds
    int tilesTall = currentLevelRef->getTilesTall();
    if (maxRow > tilesTall - 1) maxRow = tilesTall - 1;

    vector<Tile*> tilesToRight;

    for (unsigned int i = col; i < tilesRef.size(); i++) {
        for (int j = minRow; j <= maxRow; j++) {
            Tile* tile = tilesRef[i][j];

            if (tile != nullptr)
                tilesToRight.push_back(tile);
        }
    }

    return tilesToRight;
}

vector<Tile*> MovingSprite::getTilesToTop() {
    SDL_Rect nextHitbox = getNextHitboxXY();

    int row = (nextHitbox.y + nextHitbox.h) / TILE_HEIGHT;
    int minCol = nextHitbox.x / TILE_WIDTH;
    int maxCol = (nextHitbox.x + nextHitbox.w) / TILE_WIDTH;

    // TODO: Fix this logic. Currently this addresses the case where, if the rightmost point
    // of player and leftmost point of tile are equal, it should NOT check this column.
    if ((nextHitbox.x + nextHitbox.w) % TILE_WIDTH == 0 && maxCol > 0)
        maxCol--;

    // Don't let maxCol go out of bounds
    int tilesWide = currentLevelRef->getTilesWide();
    if (maxCol > tilesWide - 1) maxCol = tilesWide - 1;

    vector<Tile*> tilesToTop;

    for (int i = minCol; i <= maxCol; i++) {
        for (int j = row; j >= 0; j--) {
            Tile* tile = tilesRef[i][j];

            if (tile != nullptr)
                tilesToTop.push_back(tile);
        }
    }

    return tilesToTop;
}

vector<Tile*> MovingSprite::getTilesToBottom() {
    SDL_Rect nextHitbox = getNextHitboxXY();

    int row = nextHitbox.y / TILE_HEIGHT;
    int minCol = nextHitbox.x / TILE_WIDTH;
    int maxCol = (nextHitbox.x + nextHitbox.w) / TILE_WIDTH;

    // TODO: Fix this logic. Currently this addresses the case where, if the rightmost point
    // of player and leftmost point of tile are equal, it should NOT check this column.
    if ((nextHitbox.x + nextHitbox.w) % TILE_WIDTH == 0 && maxCol > 0)
        maxCol--;

    // Don't let maxCol go out of bounds
    int tilesWide = currentLevelRef->getTilesWide();
    if (maxCol > tilesWide - 1) maxCol = tilesWide - 1;

    vector<Tile*> tilesToBottom;

    for (int i = minCol; i <= maxCol; i++) {
        for (unsigned int j = row; j < tilesRef[i].size(); j++) {
            Tile* tile = tilesRef[i][j];

            if (tile != nullptr)
                tilesToBottom.push_back(tile);
        }
    }

    return tilesToBottom;
}

TileCollisionInfo MovingSprite::checkTileCollisionLR() {
    TileCollisionInfo tci;

    if (xVel > 0) {
        // Check R collision
        tci.isRightChecked = true;
        tci.tileCollidingWithRight = getTileCollidingWithRight();
    } else {
        // Check L collision
        tci.isLeftChecked = true;
        tci.tileCollidingWithLeft = getTileCollidingWithLeft();
    }
    return tci;
}

TileCollisionInfo MovingSprite::checkTileCollisionTB() {
    TileCollisionInfo tci;

    if (yVel > 0) {
        // Check B collision
        tci.isBottomChecked = true;
        tci.tileCollidingWithBottom = getTileCollidingWithBottom();
    } else {
        // Check T collision
        tci.isTopChecked = true;
        tci.tileCollidingWithTop = getTileCollidingWithTop();
    }
    return tci;
}

Tile* MovingSprite::getTileCollidingWithRight() {
    SDL_Rect nextHitbox = getNextHitboxX();
    vector<Tile*> tiles = getTilesToRight();

    for (unsigned int i = 0; i < tiles.size(); i++) {
        if (tiles[i]->isColliding(Tile::TileProperty::LEFT, hitbox, nextHitbox))
            return tiles[i];
    }
    return nullptr;
}

Tile* MovingSprite::getTileCollidingWithLeft() {
    SDL_Rect nextHitbox = getNextHitboxX();
    vector<Tile*> tiles = getTilesToLeft();

    for (unsigned int i = 0; i < tiles.size(); i++) {
        if (tiles[i]->isColliding(Tile::TileProperty::RIGHT, hitbox, nextHitbox))
            return tiles[i];
    }
    return nullptr;
}

Tile* MovingSprite::getTileCollidingWithBottom(bool checkOnlyTouching) {
    SDL_Rect nextHitbox = getNextHitboxXY();
    vector<Tile*> tiles = getTilesToBottom();

    if (checkOnlyTouching) {
        for (unsigned int i = 0; i < tiles.size(); i++) {
            if (tiles[i]->isTouching(Tile::TileProperty::TOP, nextHitbox))
                return tiles[i];
        }
    } else {
        for (unsigned int i = 0; i < tiles.size(); i++) {
            if (tiles[i]->isColliding(Tile::TileProperty::TOP, hitbox, nextHitbox))
                return tiles[i];
        }
    }
    return nullptr;
}

Tile* MovingSprite::getTileCollidingWithTop() {
    SDL_Rect nextHitbox = getNextHitboxXY();
    vector<Tile*> tiles = getTilesToTop();

    for (unsigned int i = 0; i < tiles.size(); i++) {
        if (tiles[i]->isColliding(Tile::TileProperty::BOTTOM, hitbox, nextHitbox))
            return tiles[i];
    }
    return nullptr;
}

bool MovingSprite::isUnitColliding(SDL_Rect unitBox) {
    int thisTop = hitbox.y;
    int thisBottom = hitbox.y + hitbox.h;
    int thisLeft = hitbox.x;
    int thisRight = hitbox.x + hitbox.w;
    int unitTop = unitBox.y;
    int unitBottom = unitBox.y + unitBox.h;
    int unitLeft = unitBox.x;
    int unitRight = unitBox.x + unitBox.w;

    // Check if any sides are outside unit
    if (thisBottom <= unitTop) return false;
    if (thisTop >= unitBottom) return false;
    if (thisRight <= unitLeft) return false;
    if (thisLeft >= unitRight) return false;

    return true;
}

void MovingSprite::checkAndHandleSlope(Tile* tile) {
    if (tile->getIsSloped()) {
        int nextLeft = getNextHitboxX().x;
        int xPosInTile = nextLeft - tile->getBox().x;

        // y = mx + b
        float yDesiredPosInTile = tile->getSlope() * xPosInTile + tile->getLeftHeight();
        yVel += (TILE_HEIGHT - yDesiredPosInTile) / timeDelta;
        //printf("%f: %f = %f * %d + %d\n", yVel, yDesiredPosInTile, tile->getSlope(), xPosInTile, tile->getLeftHeight());
    }
}

SDL_Rect MovingSprite::getNextHitboxX() {
    return {
        hitbox.x + (int)(xVel * timeDelta + xVelRem),
        hitbox.y,
        hitbox.w,
        hitbox.h
    };
}

SDL_Rect MovingSprite::getNextHitboxXY() {
    return {
        hitbox.x + (int)(xVel * timeDelta + xVelRem),
        hitbox.y + (int)(yVel * timeDelta + yVelRem),
        hitbox.w,
        hitbox.h
    };
}

bool MovingSprite::getIsStunned() { return isStunned; }
