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
    // Tile offsets in pixels
    int x = 0;
    int y = 0;
    int i = 0;

    int tileType = -1;
    int tileCount = -1;

    ifstream map("../data/level1");
    string line;
    istringstream iss;

    // Special case: First line contains number of tiles
    // Reserve this many tiles in gTiles
    getline(map, line);
    iss.str(line);
    iss >> tileCount;
    if (iss.fail() || tileCount == -1) {
        printf("Error getting number of tiles from line.\n");
        return false;
    }

    // This is slow because it inits every element when we don't need to
    // Figure out how to assign to it in the code below
    gTiles.resize(tileCount);

    // Loop through each line...
    while (getline(map, line)) {
        istringstream iss(line);

        // Loop through each number on each line...
        while (!iss.eof()) {
            iss >> tileType;
            if (iss.fail() || tileType == -1) {
                printf("Error getting data from line.\n");
                return false;
            }

            // 0 represents empty space, therefore no tile
            if (tileType != 0) {
                gTiles[i] = new Tile(x, y, tileType);
                i++;
            }

            x += TILE_WIDTH;
        }
        x = 0;
        y += TILE_HEIGHT;
    }

    map.close();

    // Set clips for each tile
    gTileClips[WALLFILL_BLUE].x = TILE_OFFSET + (TILE_WIDTH * 6);
    gTileClips[WALLFILL_BLUE].y = TILE_OFFSET + (TILE_HEIGHT * 3);

    gTileClips[PLATFORM_BLUE_FLAT_TOP_EDGEL].x = TILE_OFFSET + (TILE_WIDTH * 1);
    gTileClips[PLATFORM_BLUE_FLAT_TOP_EDGEL].y = TILE_OFFSET + (TILE_HEIGHT * 1);

    gTileClips[PLATFORM_BLUE_FLAT_BOT_EDGEL].x = TILE_OFFSET + (TILE_WIDTH * 0);
    gTileClips[PLATFORM_BLUE_FLAT_BOT_EDGEL].y = TILE_OFFSET + (TILE_HEIGHT * 2);

    gTileClips[PLATFORM_BLUE_FLAT_TOP].x = TILE_OFFSET + (TILE_WIDTH * 2);
    gTileClips[PLATFORM_BLUE_FLAT_TOP].y = TILE_OFFSET + (TILE_HEIGHT * 1);

    gTileClips[PLATFORM_BLUE_FLAT_BOT].x = TILE_OFFSET + (TILE_WIDTH * 2);
    gTileClips[PLATFORM_BLUE_FLAT_BOT].y = TILE_OFFSET + (TILE_HEIGHT * 2);

    gTileClips[PLATFORM_BLUE_FLAT_TOP_EDGER].x = TILE_OFFSET + (TILE_WIDTH * 4);
    gTileClips[PLATFORM_BLUE_FLAT_TOP_EDGER].y = TILE_OFFSET + (TILE_HEIGHT * 1);

    gTileClips[PLATFORM_BLUE_FLAT_BOT_EDGER].x = TILE_OFFSET + (TILE_WIDTH * 3);
    gTileClips[PLATFORM_BLUE_FLAT_BOT_EDGER].y = TILE_OFFSET + (TILE_HEIGHT * 2);

    gTileClips[PLATFORM_BLUE_FLAT_FILL_EDGE].x = TILE_OFFSET + (TILE_WIDTH * 4);
    gTileClips[PLATFORM_BLUE_FLAT_FILL_EDGE].y = TILE_OFFSET + (TILE_HEIGHT * 2);

    for (unsigned int i=0; i<gTileClips.size(); i++) {
        gTileClips[i].w = TILE_WIDTH;
        gTileClips[i].h = TILE_HEIGHT;
    }

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
