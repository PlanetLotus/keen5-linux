#include "globals.h"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int SCREEN_BPP = 32;

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

Texture* background = new Texture();
Texture* keen = new Texture();
