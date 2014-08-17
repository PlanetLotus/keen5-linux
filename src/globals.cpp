#include "globals.h"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int SCREEN_BPP = 32;

const int TEXTURE_COUNT = 3;

int TILES_WIDE;
int TILES_TALL;
int LEVEL_WIDTH;
int LEVEL_HEIGHT;

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

Texture* gKeenTexture = new Texture();
Texture* gMaskTexture = new Texture();

std::vector< std::vector<Tile*> > gTiles;

Controller gController;

// Number of timer loops per animation frame
const int FRAMETIME = 2;

std::vector<Sprite*> gSpriteBatch(1);

Camera camera;
