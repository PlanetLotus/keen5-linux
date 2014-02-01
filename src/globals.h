#ifndef GLOBALS_H
#define GLOBALS_H

#include "SDL.h"
#include "Texture.h"

// Screen attributes
extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;
extern const int SCREEN_BPP;

// Window and renderer
extern SDL_Window* window;
extern SDL_Renderer* renderer;
//extern std::vector<SDL_Surface*> surfaces;

// Global textures
extern Texture* background;
extern Texture* keen;

#endif
