#include "BlasterShot.h"

BlasterShot::BlasterShot(int startX, int startY, int velocityX, int velocityY) {
    hitbox.x = startX;
    hitbox.y = startY;
    hitbox.w = TILE_WIDTH;
    hitbox.h = TILE_HEIGHT;

    xVel = velocityX;
    yVel = velocityY;

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

void BlasterShot::update() {
    // Check collision
    animate(0);
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

void BlasterShot::draw() {
    hitbox.x += xVel;
    hitbox.y += yVel;

    gKeenTexture->render(hitbox.x, hitbox.y, srcClip);
}
