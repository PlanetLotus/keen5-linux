#include "globals.h"
#include "Sparky.h"

using namespace std;

Sparky::Sparky() {
    hitbox.x = TILE_WIDTH * 20;
    hitbox.y = TILE_HEIGHT * 9;
    hitbox.w = TILE_WIDTH * 2;
    hitbox.h = TILE_HEIGHT * 2;
    isClipping = true;

    xVel = 0;
    yVel = 0;

    frame = 0;
    animState = 0;

    srcClip = NULL;

    stunned = false;

    SDL_Rect walkL0 = { TILE_WIDTH * 14, 0, TILE_WIDTH * 2, TILE_HEIGHT * 2 };
    SDL_Rect walkL1 = { TILE_WIDTH * 16, 0, TILE_WIDTH * 2, TILE_HEIGHT * 2 };
    SDL_Rect walkL2 = { TILE_WIDTH * 18, 0, TILE_WIDTH * 2, TILE_HEIGHT * 2 };
    SDL_Rect walkL3 = { TILE_WIDTH * 20, 0, TILE_WIDTH * 2, TILE_HEIGHT * 2 };

    SDL_Rect walkLeftArray[4] = { walkL0, walkL1, walkL2, walkL3 };

    vector<SDL_Rect> walkLeftAnim(walkLeftArray, walkLeftArray + sizeof(walkLeftArray) / sizeof(SDL_Rect));

    anims[0] = walkLeftAnim;
}

void Sparky::animate(int nextState, int frametime) {
    if (animState == nextState) {
        // Get next frame
        frame++;
    } else {
        // Start new animation
        frame = 0;
        animState = nextState;
    }

    if (frame == anims[animState].size() * frametime)
        frame = 0;

    srcClip = &anims[animState][frame / frametime];
}

void Sparky::takeShotByPlayer() {
    stunned = true;
    isClipping = false;
}

void Sparky::update() {
    if (stunned) {
        // Animate stunned
        return;
    }

    animate(0, 3);

    // Check tile collision

    hitbox.x += xVel;
    hitbox.y += yVel;
}

void Sparky::draw(SDL_Rect cameraBox) {
    gKeenTexture->render(hitbox.x - cameraBox.x, hitbox.y - cameraBox.y, srcClip);
}
