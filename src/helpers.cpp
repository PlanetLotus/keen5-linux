//#include <string>

#include "helpers.h"
#include "Texture.h"
#include "globals.h"

#include <stdio.h>
#include <string>
#include <fstream>
#include <sstream>

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
    // Does tileCount (actual tiles, not blank) matter? A little bit. We need gTiles, and it's faster to initialize it rather than keep adding to it

    int tilesWide = -1;
    int tilesTall = -1;
    int tileCount = -1;

    int xSrc = -1;
    int ySrc = -1;
    int collideVal = -1;
    bool collideT = false;
    bool collideR = false;
    bool collideB = false;
    bool collideL = false;

    int xDest = 0;
    int yDest = 0;
    int i = 0;

    ifstream map("../data/level1");
    string line;
    istringstream iss;

    // Special case: First line contains # of tiles wide, # tiles tall, and tileCount
    // Maybe use a "level" class??
    getline(map, line);
    iss.str(line);
    iss >> tilesWide;
    iss >> tilesTall;
    iss >> tileCount;
    if (iss.fail() || tilesWide == -1 || tilesTall == -1 || tileCount == -1) {
        printf("Error getting number of tiles from line 1.\n");
        return false;
    }

    // Special case: Second line contains source file absolute path
    // This is mostly used by the level editor. We only want the file name.
    // NOT USED RIGHT NOW, BUT MIGHT BE A GOOD IDEA LATER.
    // Only downside is the level files are computed at runtime...better hardcoded.
    getline(map, line);

    // This is slow because it inits every element when we don't need to
    // Figure out how to assign to it in the code below
    gTiles.resize(tileCount);

    // Loop through each line
    while (getline(map, line)) {
        // Check row position
        if (xDest / TILE_WIDTH > tilesWide - 1) {
            yDest += TILE_HEIGHT;
            xDest = 0;
        }

        istringstream iss(line);

        iss >> xSrc;

        if (xSrc == -1) {
            xDest += TILE_WIDTH;
            continue;
        }

        iss >> ySrc;

        iss >> collideVal;
        collideT = collideVal == 1 ? true : false;
        iss >> collideVal;
        collideR = collideVal == 1 ? true : false;
        iss >> collideVal;
        collideB = collideVal == 1 ? true : false;
        iss >> collideVal;
        collideL = collideVal == 1 ? true : false;

        gTiles[i] = new Tile(xSrc, ySrc, xDest, yDest, collideT, collideR, collideB, collideL);

        xDest += TILE_WIDTH;
        i++;
    }

    map.close();
    return true;
}

void clean_up() {
    gKeenTexture->free();
    gMaskTexture->free();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    // Close font
    //TTF_CloseFont(font);

    // Quit SDL
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
}
