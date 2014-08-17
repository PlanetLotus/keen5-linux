#include "helpers.h"

bool isTopColliding(SDL_Rect before, SDL_Rect after, SDL_Rect obstacle) {
    int bottomBefore = before.y + before.h;
    int bottomObstacle = obstacle.y + obstacle.h;

    // Handles "teleporting" and bad spawning
    if (bottomBefore >= bottomObstacle && obstacle.y >= after.y) return true;

    // "Normal" collision
    if (after.y < bottomObstacle && after.y > obstacle.y) return true;

    return false;
}

bool isBottomColliding(SDL_Rect before, SDL_Rect after, SDL_Rect obstacle) {
    int bottomAfter = after.y + after.h;
    int bottomObstacle = obstacle.y + obstacle.h;

    // Handles "teleporting" and bad spawning
    if (before.y <= obstacle.y && bottomObstacle <= bottomAfter) return true;

    // "Normal" collision
    if (bottomAfter > obstacle.y && bottomAfter < bottomObstacle) return true;

    return false;
}

bool isLeftColliding(SDL_Rect before, SDL_Rect after, SDL_Rect obstacle) {
    int rightBefore = before.x + before.w;
    int rightObstacle = obstacle.x + obstacle.w;

    // Handles "teleporting" and bad spawning
    if (rightBefore >= rightObstacle && obstacle.x >= after.x) return true;

    // "Normal" collision
    if (after.x < rightObstacle && after.x > obstacle.x) return true;

    return false;
}

bool isRightColliding(SDL_Rect before, SDL_Rect after, SDL_Rect obstacle) {
    int rightAfter = after.x + after.w;
    int rightObstacle = obstacle.x + obstacle.w;

    // Handles "teleporting" and bad spawning
    if (before.x <= obstacle.x && rightObstacle <= rightAfter) return true;

    // "Normal" collision
    if (rightAfter > obstacle.x && rightAfter < rightObstacle) return true;

    return false;
}

bool isTileOnScreen(SDL_Rect* tileBox, SDL_Rect* cameraBox) {
    if (tileBox->x + tileBox->w <= cameraBox->x)
        return false;
    else if (tileBox->x >= cameraBox->x + cameraBox->w)
        return false;

    if (tileBox->y + tileBox->h <= cameraBox->y)
        return false;
    else if (tileBox->y >= cameraBox->y + cameraBox->h)
        return false;
    return true;
}
