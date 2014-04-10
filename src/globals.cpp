#include "globals.h"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int SCREEN_BPP = 32;

const int TEXTURE_COUNT = 3;

int TILES_WIDE;
int TILES_TALL;

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

Texture* gKeenTexture = new Texture();
Texture* gMaskTexture = new Texture();

std::vector< std::vector<Tile*> > gTiles;

// Number of tile sprites on the sprite sheet
// One extra is allocated because it's one-indexed rather than zero
// Zero is reserved for the empty tile which is not assigned
const int TOTAL_TILE_TYPES = 9;
std::vector<SDL_Rect> gTileClips(TOTAL_TILE_TYPES);

// Mapping of tile type int to where to find it on the sprite sheet
const int WALLFILL_BLUE = 1;
const int PLATFORM_BLUE_FLAT_TOP_EDGEL = 2;
const int PLATFORM_BLUE_FLAT_BOT_EDGEL = 3;
const int PLATFORM_BLUE_FLAT_TOP = 4;
const int PLATFORM_BLUE_FLAT_BOT = 5;
const int PLATFORM_BLUE_FLAT_TOP_EDGER = 6;
const int PLATFORM_BLUE_FLAT_BOT_EDGER = 7;
const int PLATFORM_BLUE_FLAT_FILL_EDGE = 8;

// Number of timer loops per animation frame
const int FRAMETIME = 2;
