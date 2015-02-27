#include "FireSpinner.h"

using namespace std;

FireSpinner::FireSpinner(int destX, int destY, int layer)
    : Tile(TILE_WIDTH * 10, TILE_HEIGHT * 54, destX, destY, 0, 0, true, true, true, true, layer, false, false, false) {
    spinAnim = {
        { TILE_WIDTH * 10, TILE_HEIGHT * 54, TILE_WIDTH, TILE_HEIGHT },
        { TILE_WIDTH * 11, TILE_HEIGHT * 54, TILE_WIDTH, TILE_HEIGHT },
        { TILE_WIDTH * 12, TILE_HEIGHT * 54, TILE_WIDTH, TILE_HEIGHT },
        { TILE_WIDTH * 13, TILE_HEIGHT * 54, TILE_WIDTH, TILE_HEIGHT },
        { TILE_WIDTH * 14, TILE_HEIGHT * 54, TILE_WIDTH, TILE_HEIGHT },
        { TILE_WIDTH * 15, TILE_HEIGHT * 54, TILE_WIDTH, TILE_HEIGHT },
        { TILE_WIDTH * 16, TILE_HEIGHT * 54, TILE_WIDTH, TILE_HEIGHT },
        { TILE_WIDTH * 17, TILE_HEIGHT * 54, TILE_WIDTH, TILE_HEIGHT }
    };

    frame = 0;
}

void FireSpinner::animate(int frametime) {
    // Get next frame
    frame++;

    if (frame == spinAnim.size() * frametime)
        frame = 0;

    srcBox = spinAnim[frame / frametime];
}

void FireSpinner::update() {
    animate(1);
}
