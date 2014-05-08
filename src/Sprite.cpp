#include "globals.h"
#include "helpers.h"
#include "Sprite.h"
#include "Tile.h"

Sprite::~Sprite() {
    delete srcClip;
}

void Sprite::update() {}

void Sprite::draw() {}

void Sprite::CheckTBCollision() {
    // Hitbox after update in y-direction
    SDL_Rect nextHitbox = { hitbox.x + xVel, hitbox.y + yVel, hitbox.w, hitbox.h };

    int minCol = nextHitbox.x / TILE_WIDTH;
    int maxCol = (nextHitbox.x + nextHitbox.w) / TILE_WIDTH;

    // TODO: Fix this logic. Currently this addresses the case where, if the rightmost point
    // of player and leftmost point of tile are equal, it should NOT check this column.
    if ((nextHitbox.x + nextHitbox.w) % TILE_WIDTH == 0 && maxCol > 0)
        maxCol--;

    // Don't let maxCol go out of bounds
    if (maxCol > TILES_WIDE-1) maxCol = TILES_WIDE-1;

    if (yVel > 0)
        isBottomColliding = CheckBottomCollision(minCol, maxCol, nextHitbox);
    else
        isTopColliding = CheckTopCollision(minCol, maxCol, nextHitbox);
}

void Sprite::CheckLRCollision() {
    // Hitbox after update in x-direction
    // Does not include new yVel
    SDL_Rect nextHitbox = { hitbox.x + xVel, hitbox.y, hitbox.w, hitbox.h };

    int minRow = nextHitbox.y / TILE_HEIGHT;
    int maxRow = (nextHitbox.y + nextHitbox.h) / TILE_HEIGHT;

    // TODO: Fix this logic. Currently this addresses the case where, if the bottommost point
    // of player and topmost point of tile are equal, it should NOT check this row.
    if ((nextHitbox.y + nextHitbox.h) % TILE_WIDTH == 0 && maxRow > 0)
        maxRow--;

    // Don't let maxRow go out of bounds
    if (maxRow > TILES_TALL-1) maxRow = TILES_TALL-1;

    if (xVel > 0)
        isRightColliding = CheckRightCollision(minRow, maxRow, nextHitbox);
    else
        isLeftColliding = CheckLeftCollision(minRow, maxRow, nextHitbox);
}

bool Sprite::CheckTopCollision(int minCol, int maxCol, SDL_Rect nextHitbox) {
    int row = (nextHitbox.y + nextHitbox.h) / TILE_HEIGHT;

    for (int i = minCol; i <= maxCol; i++) {
        for (int j = row; j >= 0; j--) {
            Tile* tile = gTiles[i][j];

            if (tile != NULL &&
                tile->CollideBottom() &&
                IsTopColliding(hitbox, nextHitbox, tile->getBox())) {

                // Set yVel to the distance between the player and the
                // tile he's colliding with
                yVel = (tile->getBox().y + tile->getBox().h) - hitbox.y;
                return true;
            }
        }
    }
    return false;
}

bool Sprite::CheckBottomCollision(int minCol, int maxCol, SDL_Rect nextHitbox) {
    int row = nextHitbox.y / TILE_HEIGHT;

    for (int i = minCol; i <= maxCol; i++) {
        for (unsigned int j = row; j < gTiles[i].size(); j++) {
            Tile* tile = gTiles[i][j];

            if (tile != NULL &&
                tile->CollideTop() &&
                IsBottomColliding(hitbox, nextHitbox, tile->getBox())) {

                // Set yVel to the distance between the player and the
                // tile he's colliding with
                yVel = tile->getBox().y - (hitbox.y + hitbox.h);
                return true;
            }
        }
    }
    return false;
}

bool Sprite::CheckLeftCollision(int minRow, int maxRow, SDL_Rect nextHitbox) {
    int col = (nextHitbox.x + nextHitbox.w) / TILE_WIDTH;

    for (int i = col; i >= 0; i--) {
        for (int j = minRow; j <= maxRow; j++) {
            Tile* tile = gTiles[i][j];

            if (tile != NULL &&
                tile->CollideRight() &&
                IsLeftColliding(hitbox, nextHitbox, tile->getBox())) {

                // Set xVel to the distance between the player and the
                // tile he's colliding with
                xVel = (tile->getBox().x + tile->getBox().w) - hitbox.x;
                return true;
            }
        }
    }
    return false;
}

bool Sprite::CheckRightCollision(int minRow, int maxRow, SDL_Rect nextHitbox) {
    int col = nextHitbox.x / TILE_WIDTH;

    for (unsigned int i = col; i < gTiles.size(); i++) {
        for (int j = minRow; j <= maxRow; j++) {
            Tile* tile = gTiles[i][j];

            if (tile != NULL &&
                tile->CollideLeft() &&
                IsRightColliding(hitbox, nextHitbox, tile->getBox())) {

                // Set xVel to the distance between the player and the
                // tile he's colliding with
                xVel = tile->getBox().x - (hitbox.x + hitbox.w);
                return true;
            }
        }
    }
    return false;
}

void Sprite::CheckCollision() {
    // Reset flags from last turn
    isTopColliding = false;
    isBottomColliding = false;
    isLeftColliding = false;
    isRightColliding = false;

    bool xChange = xVel != 0;
    bool yChange = yVel != 0;

    // For static objects (tiles), check per-axis collision only if we're moving on that axis
    if (xChange)
        CheckLRCollision();
    if (yChange)
        CheckTBCollision();
}
