#include <algorithm>
#include "BlasterShot.h"
#include "globals.h"
#include "helpers.h"

using namespace std;

BlasterShot::BlasterShot(int startX, int startY, float velocityX, float velocityY) {
    hitbox.x = startX;
    hitbox.y = startY;
    hitbox.w = TILE_WIDTH;
    hitbox.h = TILE_HEIGHT;

    xVel = velocityX * 20;
    yVel = velocityY * 20;

    /*
    isTopColliding = false;
    isBottomColliding = false;
    isLeftColliding = false;
    isRightColliding = false;
    */

    expireTimer = 0;

    frame = 0;
    animState = 0;

    srcClip = NULL;

    // Add self to sprite batch
    gSpriteBatch.push_back(this);

    // Animation instantiation
    SDL_Rect move0 = {TILE_WIDTH * 6, TILE_HEIGHT * 7, TILE_WIDTH, TILE_HEIGHT};
    SDL_Rect move1 = {TILE_WIDTH * 7, TILE_HEIGHT * 7, TILE_WIDTH, TILE_HEIGHT};
    SDL_Rect move2 = {TILE_WIDTH * 8, TILE_HEIGHT * 7, TILE_WIDTH, TILE_HEIGHT};
    SDL_Rect move3 = {TILE_WIDTH * 9, TILE_HEIGHT * 7, TILE_WIDTH, TILE_HEIGHT};

    SDL_Rect collide0 = {TILE_WIDTH * 10, TILE_HEIGHT * 7, TILE_WIDTH, TILE_HEIGHT};
    SDL_Rect collide1 = {TILE_WIDTH * 11, TILE_HEIGHT * 7, TILE_WIDTH, TILE_HEIGHT};

    SDL_Rect move_array[4] = { move0, move1, move2, move3 };
    SDL_Rect collide_array[2] = { collide0, collide1 };

    vector<SDL_Rect> move_anim(move_array, move_array + sizeof(move_array) / sizeof(SDL_Rect));
    vector<SDL_Rect> collide_anim(collide_array, collide_array + sizeof(collide_array) / sizeof(SDL_Rect));

    anims[0] = move_anim;
    anims[1] = collide_anim;
}

TileCollisionInfo BlasterShot::update() {
    TileCollisionInfo tci = CheckTileCollision();

    if (xVel == 0 && yVel == 0) {
        expire();
        animate(1);
    } else {
        animate(0);
    }

    if (tci.IsTopColliding()) {
        Tile* tile = gTiles[tci.TileCollidingWithTop.first][tci.TileCollidingWithTop.second];
        yVel = (tile->getBox().y + tile->getBox().h) - hitbox.y;
    } else if (tci.IsBottomColliding()) {
        Tile* tile = gTiles[tci.TileCollidingWithBottom.first][tci.TileCollidingWithBottom.second];
        yVel = tile->getBox().y - (hitbox.y + hitbox.h);
    }

    if (tci.IsLeftColliding()) {
        Tile* tile = gTiles[tci.TileCollidingWithLeft.first][tci.TileCollidingWithLeft.second];
        xVel = (tile->getBox().x + tile->getBox().w) - hitbox.x;
    } else if (tci.IsRightColliding()) {
        Tile* tile = gTiles[tci.TileCollidingWithRight.first][tci.TileCollidingWithRight.second];
        xVel = tile->getBox().x - (hitbox.x + hitbox.w);
    }

    return tci;
}

void BlasterShot::animate(int nextState) {
    if (animState == nextState) {
        // Get next frame
        frame++;
    } else {
        // Start new animation
        frame = 0;
        animState = nextState;
    }

    if (frame == anims[animState].size() * FRAMETIME)
        frame = 0;

    srcClip = &anims[animState][frame / FRAMETIME];
}

void BlasterShot::draw(TileCollisionInfo tci) {
    hitbox.x += xVel;
    hitbox.y += yVel;

    // Reset velocity if collision
    if (tci.IsTopColliding() || tci.IsBottomColliding()) yVel = 0;
    if (tci.IsLeftColliding() || tci.IsRightColliding()) xVel = 0;

    gKeenTexture->render(hitbox.x, hitbox.y, srcClip);
}

void BlasterShot::expire() {
    if (expireTimer / FRAMETIME >= 2) {
        die();
        return;
    }

    expireTimer++;
}

void BlasterShot::die() {
    // Erase this BlasterShot from gSpriteBatch
    // This calls the destructor internally
    vector<Sprite*>::iterator it = find(gSpriteBatch.begin(), gSpriteBatch.end(), this);

    if (it != gSpriteBatch.end())
        gSpriteBatch.erase(it);
}
