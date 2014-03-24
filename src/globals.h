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

// Level globals
// These should really go in a "level" class
extern int TILES_WIDE;
extern int TILES_TALL;

// Window and renderer
extern SDL_Window* window;
extern SDL_Renderer* renderer;

// Global textures
extern Texture* gKeenTexture;
extern Texture* gMaskTexture;

// 2D array of tiles describing a level
extern std::vector< std::vector<Tile*> > gTiles;

// Where to find each type of tile on the sprite sheet
extern std::vector<SDL_Rect> gTileClips;

// Mapping of tile type int to where to find it on the sprite sheet
extern const int WALLFILL_BLUE;
extern const int PLATFORM_BLUE_FLAT_TOP_EDGEL;
extern const int PLATFORM_BLUE_FLAT_BOT_EDGEL;
extern const int PLATFORM_BLUE_FLAT_TOP;
extern const int PLATFORM_BLUE_FLAT_BOT;
extern const int PLATFORM_BLUE_FLAT_TOP_EDGER;
extern const int PLATFORM_BLUE_FLAT_BOT_EDGER;
extern const int PLATFORM_BLUE_FLAT_FILL_EDGE;

#endif
