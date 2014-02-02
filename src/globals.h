#ifndef GLOBALS_H
#define GLOBALS_H

#include "SDL.h"
#include "Texture.h"
#include "Tile.h"
#include <vector>

// Screen attributes
extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;
extern const int SCREEN_BPP;

extern const int TEXTURE_COUNT;
extern const int TILE_COUNT;

// Window and renderer
extern SDL_Window* window;
extern SDL_Renderer* renderer;

// Global textures
extern Texture* gKeenTexture;
extern Texture* gMaskTexture;

extern std::vector<Tile*> gTiles;

#endif
