#ifndef HELPERS_H
#define HELPERS_H

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include <string>
#include <vector>

bool init();

bool load_files();

bool set_tiles();

bool IsTopColliding(SDL_Rect a, SDL_Rect b);
bool IsBottomColliding(SDL_Rect a, SDL_Rect b);
bool IsLeftColliding(SDL_Rect a, SDL_Rect b);
bool IsRightColliding(SDL_Rect a, SDL_Rect b);

void clean_up();

#endif
