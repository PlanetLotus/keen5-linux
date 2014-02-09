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
extern const int TOTAL_TILE_TYPES;

// Window and renderer
extern SDL_Window* window;
extern SDL_Renderer* renderer;

// Global textures
extern Texture* gKeenTexture;
extern Texture* gMaskTexture;

extern std::vector<Tile*> gTiles;

// Where to find each type of tile on the sprite sheet
extern std::vector<SDL_Rect> gTileClips;

// Mapping of tile type int to where to find it on the sprite sheet
extern const int PLATFORM_BLUE_FLAT_TOP;
extern const int PLATFORM_BLUE_FLAT_BOT;

#endif
