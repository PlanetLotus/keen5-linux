#ifndef HELPERS_H
#define HELPERS_H

#include "SDL.h"

bool init();

bool load_files();

bool set_tiles();

bool IsTopColliding(SDL_Rect before, SDL_Rect after, SDL_Rect obstacle);
bool IsBottomColliding(SDL_Rect before, SDL_Rect after, SDL_Rect obstacle);
bool IsLeftColliding(SDL_Rect before, SDL_Rect after, SDL_Rect obstacle);
bool IsRightColliding(SDL_Rect before, SDL_Rect after, SDL_Rect obstacle);

void UpdateCamera(SDL_Rect* camera, SDL_Rect keenHitbox, bool isOnGround);
bool IsTileOnScreen(SDL_Rect* tileBox, SDL_Rect* camera);

void clean_up();

#endif
