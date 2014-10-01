#include "globals.h"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int SCREEN_BPP = 32;

const int TEXTURE_COUNT = 3;

// Number of timer loops per animation frame
const int FRAMETIME = 2;

int TILES_WIDE;
int TILES_TALL;
int LEVEL_WIDTH;
int LEVEL_HEIGHT;

std::vector< std::vector<Tile*> > gTiles;

Controller gController;

std::vector<Platform*> gPlatformBatch(1);

Camera gCamera;
