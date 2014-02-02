#include "globals.h"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int SCREEN_BPP = 32;

const int TEXTURE_COUNT = 3;
const int TILE_COUNT = 10;

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

Texture* gKeenTexture = new Texture();
Texture* gMaskTexture = new Texture();

std::vector<Tile*> gTiles(TILE_COUNT);
