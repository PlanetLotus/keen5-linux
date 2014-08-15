#include "globals.h"
#include "helpers.h"
#include "Sprite.h"
#include "Tile.h"

using namespace std;

Sprite::~Sprite() {
    delete srcClip;
}

void Sprite::update() {}

void Sprite::draw(SDL_Rect camera) {}

vector<Tile*> Sprite::GetTilesToLeft() {
    SDL_Rect nextHitbox = { hitbox.x + (int)xVel, hitbox.y, hitbox.w, hitbox.h };

    int col = (nextHitbox.x + nextHitbox.w) / TILE_WIDTH;
    int minRow = nextHitbox.y / TILE_HEIGHT;
    int maxRow = (nextHitbox.y + nextHitbox.h) / TILE_HEIGHT;

    // TODO: Fix this logic. Currently this addresses the case where, if the bottommost point
    // of player and topmost point of tile are equal, it should NOT check this row.
    if ((nextHitbox.y + nextHitbox.h) % TILE_WIDTH == 0 && maxRow > 0)
        maxRow--;

    // Don't let maxRow go out of bounds
    if (maxRow > TILES_TALL-1) maxRow = TILES_TALL-1;

    vector<Tile*> tilesToLeft;

    for (int i = col; i >= 0; i--) {
        for (int j = minRow; j <= maxRow; j++) {
            Tile* tile = gTiles[i][j];

            if (tile != NULL)
                tilesToLeft.push_back(tile);
        }
    }

    return tilesToLeft;
}

vector<Tile*> Sprite::GetTilesToRight() {
    SDL_Rect nextHitbox = { hitbox.x + (int)xVel, hitbox.y, hitbox.w, hitbox.h };

    int col = nextHitbox.x / TILE_WIDTH;
    int minRow = nextHitbox.y / TILE_HEIGHT;
    int maxRow = (nextHitbox.y + nextHitbox.h) / TILE_HEIGHT;

    // TODO: Fix this logic. Currently this addresses the case where, if the bottommost point
    // of player and topmost point of tile are equal, it should NOT check this row.
    if ((nextHitbox.y + nextHitbox.h) % TILE_WIDTH == 0 && maxRow > 0)
        maxRow--;

    // Don't let maxRow go out of bounds
    if (maxRow > TILES_TALL-1) maxRow = TILES_TALL-1;

    vector<Tile*> tilesToRight;

    for (unsigned int i = col; i < gTiles.size(); i++) {
        for (int j = minRow; j <= maxRow; j++) {
            Tile* tile = gTiles[i][j];

            if (tile != NULL)
                tilesToRight.push_back(tile);
        }
    }

    return tilesToRight;
}

vector<Tile*> Sprite::GetTilesToTop() {
    SDL_Rect nextHitbox = { hitbox.x + (int)xVel, hitbox.y + (int)yVel, hitbox.w, hitbox.h };

    int row = (nextHitbox.y + nextHitbox.h) / TILE_HEIGHT;
    int minCol = nextHitbox.x / TILE_WIDTH;
    int maxCol = (nextHitbox.x + nextHitbox.w) / TILE_WIDTH;

    // TODO: Fix this logic. Currently this addresses the case where, if the rightmost point
    // of player and leftmost point of tile are equal, it should NOT check this column.
    if ((nextHitbox.x + nextHitbox.w) % TILE_WIDTH == 0 && maxCol > 0)
        maxCol--;

    // Don't let maxCol go out of bounds
    if (maxCol > TILES_WIDE-1) maxCol = TILES_WIDE-1;

    vector<Tile*> tilesToTop;

    for (int i = minCol; i <= maxCol; i++) {
        for (int j = row; j >= 0; j--) {
            Tile* tile = gTiles[i][j];

            if (tile != NULL)
                tilesToTop.push_back(tile);
        }
    }

    return tilesToTop;
}

vector<Tile*> Sprite::GetTilesToBottom() {
    SDL_Rect nextHitbox = { hitbox.x + (int)xVel, hitbox.y + (int)yVel, hitbox.w, hitbox.h };

    int row = nextHitbox.y / TILE_HEIGHT;
    int minCol = nextHitbox.x / TILE_WIDTH;
    int maxCol = (nextHitbox.x + nextHitbox.w) / TILE_WIDTH;

    // TODO: Fix this logic. Currently this addresses the case where, if the rightmost point
    // of player and leftmost point of tile are equal, it should NOT check this column.
    if ((nextHitbox.x + nextHitbox.w) % TILE_WIDTH == 0 && maxCol > 0)
        maxCol--;

    // Don't let maxCol go out of bounds
    if (maxCol > TILES_WIDE-1) maxCol = TILES_WIDE-1;

    vector<Tile*> tilesToBottom;

    for (int i = minCol; i <= maxCol; i++) {
        for (unsigned int j = row; j < gTiles[i].size(); j++) {
            Tile* tile = gTiles[i][j];

            if (tile != NULL)
                tilesToBottom.push_back(tile);
        }
    }

    return tilesToBottom;
}

TileCollisionInfo Sprite::CheckTileCollisionLR() {
    TileCollisionInfo tci;

    if (xVel > 0) {
        // Check R collision
        tci.IsRightChecked = true;
        tci.TileCollidingWithRight = GetTileCollidingWithRight();
    } else {
        // Check L collision
        tci.IsLeftChecked = true;
        tci.TileCollidingWithLeft = GetTileCollidingWithLeft();
    }
    return tci;
}

TileCollisionInfo Sprite::CheckTileCollisionTB() {
    TileCollisionInfo tci;

    if (yVel > 0) {
        // Check B collision
        tci.IsBottomChecked = true;
        tci.TileCollidingWithBottom = GetTileCollidingWithBottom();
    } else {
        // Check T collision
        tci.IsTopChecked = true;
        tci.TileCollidingWithTop = GetTileCollidingWithTop();
    }
    return tci;
}

Tile* Sprite::GetTileCollidingWithRight() {
    SDL_Rect nextHitbox = { hitbox.x + (int)xVel, hitbox.y, hitbox.w, hitbox.h };
    vector<Tile*> tiles = GetTilesToRight();

    for (unsigned int i = 0; i < tiles.size(); i++) {
        if (tiles[i]->IsColliding(Tile::LEFT, hitbox, nextHitbox))
            return tiles[i];
    }
    return NULL;
}

Tile* Sprite::GetTileCollidingWithLeft() {
    SDL_Rect nextHitbox = { hitbox.x + (int)xVel, hitbox.y, hitbox.w, hitbox.h };
    vector<Tile*> tiles = GetTilesToLeft();

    for (unsigned int i = 0; i < tiles.size(); i++) {
        if (tiles[i]->IsColliding(Tile::RIGHT, hitbox, nextHitbox))
            return tiles[i];
    }
    return NULL;
}

Tile* Sprite::GetTileCollidingWithBottom(bool checkOnlyTouching) {
    SDL_Rect nextHitbox = { hitbox.x + (int)xVel, hitbox.y + (int)yVel, hitbox.w, hitbox.h };
    vector<Tile*> tiles = GetTilesToBottom();

    if (checkOnlyTouching) {
        for (unsigned int i = 0; i < tiles.size(); i++) {
            if (tiles[i]->IsTouching(Tile::TOP, nextHitbox))
                return tiles[i];
        }
    } else {
        for (unsigned int i = 0; i < tiles.size(); i++) {
            if (tiles[i]->IsColliding(Tile::TOP, hitbox, nextHitbox))
                return tiles[i];
        }
    }
    return NULL;
}

Tile* Sprite::GetTileCollidingWithTop() {
    SDL_Rect nextHitbox = { hitbox.x + (int)xVel, hitbox.y + (int)yVel, hitbox.w, hitbox.h };
    vector<Tile*> tiles = GetTilesToTop();

    for (unsigned int i = 0; i < tiles.size(); i++) {
        if (tiles[i]->IsColliding(Tile::BOTTOM, hitbox, nextHitbox))
            return tiles[i];
    }
    return NULL;
}

int Sprite::GetX() { return hitbox.x; }
int Sprite::GetY() { return hitbox.y; }
SDL_Rect Sprite::GetBox() { return hitbox; }
