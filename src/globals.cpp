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

Controller gController;

// Number of tile sprites on the sprite sheet
// One extra is allocated because it's one-indexed rather than zero
// Zero is reserved for the empty tile which is not assigned
const int TOTAL_TILE_TYPES = 9;
std::vector<SDL_Rect> gTileClips(TOTAL_TILE_TYPES);

// Number of timer loops per animation frame
const int FRAMETIME = 2;
