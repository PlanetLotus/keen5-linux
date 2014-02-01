#ifndef HELPERS_H
#define HELPERS_H

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include <string>
#include <vector>

bool init();

bool load_files();

void set_clips();

void clean_up();

#endif
