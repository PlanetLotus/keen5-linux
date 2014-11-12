#ifndef HELPERS_H
#define HELPERS_H

#include "SDL.h"

bool isTopColliding(SDL_Rect before, SDL_Rect after, SDL_Rect obstacle);
bool isBottomColliding(SDL_Rect before, SDL_Rect after, SDL_Rect obstacle, bool allowOtherCollisions = false);
bool isLeftColliding(SDL_Rect before, SDL_Rect after, SDL_Rect obstacle);
bool isRightColliding(SDL_Rect before, SDL_Rect after, SDL_Rect obstacle);
bool isTileOnScreen(SDL_Rect* tileBox, SDL_Rect* cameraBox);

#endif
