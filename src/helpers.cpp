#include <fstream>
#include <stdio.h>
#include <string>
#include <sstream>
#include "globals.h"
#include "helpers.h"
#include "SDL_image.h"
#include "Texture.h"

using namespace std;

bool init() {
    // Init all SDL subsystems
    if (SDL_Init( SDL_INIT_TIMER | SDL_INIT_AUDIO | SDL_INIT_VIDEO ) == -1) {
        printf("SDL couldn't be initialized. SDL_Error: %s\n", SDL_GetError());
        return false;
    }

    // Init IMG
    int flags = IMG_INIT_PNG;
    int initted = IMG_Init(flags);
    if ((initted&flags) != flags) {
        printf("Couldn't init SDL_image. IMG_Error: %s\n", IMG_GetError());
    }

    // Set texture filtering to linear
    if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
        printf("Warning: Linear texture filtering not enabled!\n");

    // Set up screen
    SDL_CreateWindowAndRenderer(
                                SCREEN_WIDTH,
                                SCREEN_HEIGHT,
                                SDL_WINDOW_SHOWN,
                                &window,
                                &renderer
                                );

    // Make sure window was successfully set up
    if (window == NULL || renderer == NULL) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return false;
    }

    // Init renderer color
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);

    // Initialize SDL_ttf
    if (TTF_Init() == -1) return false;

    // Update screen
    SDL_RenderPresent(renderer);

    return true;
}

bool load_files() {
    string data = "../data/";

    // Load images
    if (!gKeenTexture->loadFromFile(data + "keensprite.png", true)) {
        printf("Failed to load keen texture!\n");
        return false;
    }
    if (!gMaskTexture->loadFromFile(data + "masks.png", false)) {
        printf("Failed to load masks texture!\n");
        return false;
    }

    // Open the fonts
    //TTF_Font *font = TTF_OpenFont ("lazy.ttf", 28);

    //if (font == NULL) return false;

    return true;
}

bool set_tiles() {
    // Notes:
    // Does levelWidth / levelHeight matter? Yes, because we have to know when we're at the end of a "row" when reading the file
    // tileCount in file is currently UNUSED

    int tilesWide = -1;
    int tilesTall = -1;
    int tileCountLayer1 = -1;
    int tileCountLayer2 = -1;

    int xSrc = -1;
    int ySrc = -1;
    int heightVal = -1;
    int collideVal = -1;
    int layerVal = -1;
    int propertyVal = -1;
    int leftHeight = 0;
    int rightHeight = 0;
    bool collideT = false;
    bool collideR = false;
    bool collideB = false;
    bool collideL = false;

    int x = 0;
    int y = 0;

    ifstream map("../data/nulltest");
    string line;
    istringstream iss;

    // Special case: First line contains # of tiles wide, # tiles tall, and # tiles (non-blank) per layer
    // For now, it's assumed the number of layers is known (hardcoded)
    // Maybe use a "level" class??
    getline(map, line);
    iss.str(line);
    iss >> tilesWide;
    iss >> tilesTall;
    iss >> tileCountLayer1;
    iss >> tileCountLayer2;
    if (iss.fail() || tilesWide == -1 || tilesTall == -1 || tileCountLayer1 == -1 || tileCountLayer2 == -1) {
        printf("Error getting metadata from line 1.\n");
        return false;
    }

    TILES_WIDE = tilesWide;
    LEVEL_WIDTH = TILES_WIDE * TILE_WIDTH;
    TILES_TALL = tilesTall;
    LEVEL_HEIGHT = TILES_TALL * TILE_HEIGHT;

    // Special case: Second line contains source file absolute path
    // This is mostly used by the level editor. We only want the file name.
    // NOT USED RIGHT NOW, BUT MIGHT BE A GOOD IDEA LATER.
    // Only downside is the level files are computed at runtime...better hardcoded.
    getline(map, line);

    // This is slow because it inits every element when we don't need to
    // Figure out how to assign to it in the code below
    gTiles.resize(tilesWide);
    for (int iter = 0; iter < tilesWide; iter++)
        gTiles[iter].resize(tilesTall);

    // Loop through each line
    while (getline(map, line)) {
        istringstream iss(line);

        iss >> xSrc;

        // Blank line detected - common at end of file
        if (iss.fail())
            continue;

        // Check row position
        if (x == tilesWide) {
            y++;
            x = 0;
        }

        if (xSrc < 0) {
            int nullCount = xSrc * -1;
            for (int i = 0; i < nullCount; i++) {
                // Check row position
                if (x == tilesWide) {
                    y++;
                    x = 0;
                }

                gTiles[x][y] = NULL;
                x++;
            }
            continue;
        }

        iss >> ySrc;

        iss >> heightVal;
        leftHeight = heightVal;
        iss >> heightVal;
        rightHeight = heightVal;
        iss >> collideVal;
        collideT = collideVal == 1 ? true : false;
        iss >> collideVal;
        collideR = collideVal == 1 ? true : false;
        iss >> collideVal;
        collideB = collideVal == 1 ? true : false;
        iss >> collideVal;
        collideL = collideVal == 1 ? true : false;

        iss >> propertyVal;
        bool isPole = false;
        if (propertyVal == 1)
            isPole = true;

        iss >> layerVal;

        gTiles[x][y] = new Tile(xSrc, ySrc, x * TILE_WIDTH, y * TILE_HEIGHT, leftHeight, rightHeight,
            collideT, collideR, collideB, collideL, layerVal, isPole);

        x++;
    }

    map.close();
    return true;
}

bool IsTopColliding(SDL_Rect before, SDL_Rect after, SDL_Rect obstacle) {
    int bottomBefore = before.y + before.h;
    int bottomObstacle = obstacle.y + obstacle.h;

    // Handles "teleporting" and bad spawning
    if (bottomBefore >= bottomObstacle && obstacle.y >= after.y) return true;

    // "Normal" collision
    if (after.y < bottomObstacle && after.y > obstacle.y) return true;

    return false;
}

bool IsBottomColliding(SDL_Rect before, SDL_Rect after, SDL_Rect obstacle) {
    int bottomAfter = after.y + after.h;
    int bottomObstacle = obstacle.y + obstacle.h;

    // Handles "teleporting" and bad spawning
    if (before.y <= obstacle.y && bottomObstacle <= bottomAfter) return true;

    // "Normal" collision
    if (bottomAfter > obstacle.y && bottomAfter < bottomObstacle) return true;

    return false;
}

bool IsLeftColliding(SDL_Rect before, SDL_Rect after, SDL_Rect obstacle) {
    int rightBefore = before.x + before.w;
    int rightObstacle = obstacle.x + obstacle.w;

    // Handles "teleporting" and bad spawning
    if (rightBefore >= rightObstacle && obstacle.x >= after.x) return true;

    // "Normal" collision
    if (after.x < rightObstacle && after.x > obstacle.x) return true;

    return false;
}

bool IsRightColliding(SDL_Rect before, SDL_Rect after, SDL_Rect obstacle) {
    int rightAfter = after.x + after.w;
    int rightObstacle = obstacle.x + obstacle.w;

    // Handles "teleporting" and bad spawning
    if (before.x <= obstacle.x && rightObstacle <= rightAfter) return true;

    // "Normal" collision
    if (rightAfter > obstacle.x && rightAfter < rightObstacle) return true;

    return false;
}

void UpdateCamera(SDL_Rect* camera, SDL_Rect keenHitbox) {
    // Center camera over Keen
    camera->x = (keenHitbox.x + keenHitbox.w / 2) - SCREEN_WIDTH / 2;
    camera->y = (keenHitbox.y + keenHitbox.h / 2) - SCREEN_HEIGHT / 2;

    // Keep camera in level bounds
    if (camera->x < 0)
        camera->x = 0;
    else if (camera->x > LEVEL_WIDTH - camera->w)
        camera->x = LEVEL_WIDTH - camera->w;

    if (camera->y < 0)
        camera->y = 0;
    else if (camera->y > LEVEL_HEIGHT - camera->h)
        camera->y = LEVEL_HEIGHT - camera->h;
}

bool IsTileOnScreen(SDL_Rect* tileBox, SDL_Rect* camera) {
    if (tileBox->x + tileBox->w <= camera->x)
        return false;
    else if (tileBox->x >= camera->x + camera->w)
        return false;

    if (tileBox->y + tileBox->h <= camera->y)
        return false;
    else if (tileBox->y >= camera->y + camera->h)
        return false;
    return true;
}

void clean_up() {
    gKeenTexture->free();
    gMaskTexture->free();

    for (unsigned int i=0; i<gTiles.size(); i++) {
        for (unsigned int j=0; j<gTiles[i].size(); j++) {
            if (gTiles[i][j] != NULL)
                delete gTiles[i][j];
        }
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    // Close font
    //TTF_CloseFont(font);

    // Quit SDL
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
}
