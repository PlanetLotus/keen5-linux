#ifndef HELPERS_H
#define HELPERS_H

#include "SDL.h"

bool IsTopColliding(SDL_Rect before, SDL_Rect after, SDL_Rect obstacle);
bool IsBottomColliding(SDL_Rect before, SDL_Rect after, SDL_Rect obstacle);
bool IsLeftColliding(SDL_Rect before, SDL_Rect after, SDL_Rect obstacle);
bool IsRightColliding(SDL_Rect before, SDL_Rect after, SDL_Rect obstacle);
bool IsTileOnScreen(SDL_Rect* tileBox, SDL_Rect* cameraBox);

#endif
