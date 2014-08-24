#include "globals.h"
#include "Sparky.h"

using namespace std;

Sparky::Sparky() {
    hitbox.x = TILE_WIDTH * 20;
    hitbox.y = TILE_HEIGHT * 9;
    hitbox.w = TILE_WIDTH * 2;
    hitbox.h = TILE_HEIGHT * 2;
    isStunned = false;

    xVel = 0;
    yVel = 0;

    frame = 0;
    animState = 0;

    srcClip = NULL;

    SDL_Rect walkL0 = { TILE_WIDTH * 14, 0, TILE_WIDTH * 2, TILE_HEIGHT * 2 };
    SDL_Rect walkL1 = { TILE_WIDTH * 16, 0, TILE_WIDTH * 2, TILE_HEIGHT * 2 };
    SDL_Rect walkL2 = { TILE_WIDTH * 18, 0, TILE_WIDTH * 2, TILE_HEIGHT * 2 };
    SDL_Rect walkL3 = { TILE_WIDTH * 20, 0, TILE_WIDTH * 2, TILE_HEIGHT * 2 };

    SDL_Rect die0 = { TILE_WIDTH * 16, TILE_HEIGHT * 2, TILE_WIDTH * 2, TILE_HEIGHT * 2 };
    SDL_Rect die1 = { TILE_WIDTH * 18, TILE_HEIGHT * 2, TILE_WIDTH * 2, TILE_HEIGHT * 2 };
    SDL_Rect die2 = { TILE_WIDTH * 20, TILE_HEIGHT * 2, TILE_WIDTH * 2, TILE_HEIGHT * 2 };

    SDL_Rect walkLeftArray[4] = { walkL0, walkL1, walkL2, walkL3 };
    SDL_Rect dieArray[3] = { die0, die1, die2 };

    vector<SDL_Rect> walkLeftAnim(walkLeftArray, walkLeftArray + sizeof(walkLeftArray) / sizeof(SDL_Rect));
    vector<SDL_Rect> dieAnim(dieArray, dieArray + sizeof(dieArray) / sizeof(SDL_Rect));

    anims[0] = walkLeftAnim;
    anims[1] = dieAnim;
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
    isStunned = true;
}

void Sparky::update() {
    if (isStunned) {
        animate(1, 3);
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
