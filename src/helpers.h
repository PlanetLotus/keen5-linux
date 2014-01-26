#ifndef HELPERS_H
#define HELPERS_H

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_ttf.h"
#include <string>
#include <vector>

bool init();

bool load_files();

void set_clips();

void clean_up();

#endif
