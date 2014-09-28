#ifndef GLOBALS_H
#define GLOBALS_H

#include <vector>
#include "BlasterShot.h"
#include "Camera.h"
#include "Controller.h"
#include "SDL.h"
#include "Sprite.h"
#include "Texture.h"
#include "Tile.h"
#include "Platform.h"

// Screen attributes
extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;
extern const int SCREEN_BPP;

extern const int TEXTURE_COUNT;

// Number of timer loops per animation frame
extern const int FRAMETIME;

// Level globals
// These should really go in a "level" class
extern int TILES_WIDE;
extern int TILES_TALL;
extern int LEVEL_WIDTH;
extern int LEVEL_HEIGHT;

// Window and renderer
extern SDL_Window* gWindow;
extern SDL_Renderer* gRenderer;

// Global textures
extern Texture* gKeenTexture;
extern Texture* gMaskTexture;

// 2D array of tiles describing a level
extern std::vector< std::vector<Tile*> > gTiles;

extern Controller gController;

extern std::vector<Sprite*> gEnemyBatch;
extern std::vector<BlasterShot*> gBlasterShotBatch;
extern std::vector<Platform*> gPlatformBatch;

extern Camera gCamera;

#endif
