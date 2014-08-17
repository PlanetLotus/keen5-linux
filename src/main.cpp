#include <fstream>
#include <iostream>
#include <sstream>
#include "Camera.h"
#include "globals.h"
#include "Player.h"
#include "SDL_image.h"
#include "Timer.h"

using namespace std;

bool init();
bool loadFiles();
bool setTiles();
void cleanUp();

int main (int argc, char **args) {
    // Initialize variables
    bool running = true;
    SDL_Event event;
    Timer fps;

    // Initialize SDL
    if (!init()) return 1;
    if (!loadFiles()) return 1;
    if (!setTiles()) return 1;

    Sprite* character = new Player();
    Player* characterPlayerPtr = dynamic_cast<Player*>(character);
    gSpriteBatch[0] = character;

    while (running) {
        // Start timer
        fps.start();

        while (SDL_PollEvent(&event)) {
            // If a key was pressed
            if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                    case SDLK_ESCAPE: running = false; break;
                }
            } else if (event.type == SDL_KEYUP) {
                switch (event.key.keysym.sym) {
                    case SDLK_LCTRL: gController.isHoldingCtrl = false; break;
                    case SDLK_SPACE: gController.isHoldingSpace = false; break;
                    case SDLK_LALT: gController.isHoldingAlt = false; break;
                }
            }
            // If the user X'd out of the window
            else if (event.type == SDL_QUIT) {
                running = false;
            }
        }

        // Clear screen
        SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);
        SDL_RenderClear(gRenderer);

        // Update units
        for (unsigned int i = 0; i < gSpriteBatch.size(); i++)
            gSpriteBatch[i]->update();

        // Render tiles - Layer 0 (Before units)
        for (unsigned int i=0; i<gTiles.size(); i++) {
            for (unsigned int j=0; j<gTiles[i].size(); j++) {
                if (gTiles[i][j] != NULL && gTiles[i][j]->layer == 0)
                    gTiles[i][j]->render(gCamera.getBox());
            }
        }

        for (unsigned int i = 0; i < gSpriteBatch.size(); i++)
            gSpriteBatch[i]->draw(gCamera.getBox());

        // Render tiles - Layer 1 (After units)
        for (unsigned int i=0; i<gTiles.size(); i++) {
            for (unsigned int j=0; j<gTiles[i].size(); j++) {
                if (gTiles[i][j] != NULL && gTiles[i][j]->layer == 1)
                    gTiles[i][j]->render(gCamera.getBox());
            }
        }

        gCamera.update(character->getBox(), characterPlayerPtr->getIsOnGround());

        // Update screen
        SDL_RenderPresent(gRenderer);

        // Cap frame rate
        if (fps.getTicks() < 1000 / FRAMES_PER_SECOND)
            SDL_Delay((1000/FRAMES_PER_SECOND) - fps.getTicks());
    }

    cleanUp();
    return 0;
}

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
        &gWindow,
        &gRenderer
    );

    // Make sure window was successfully set up
    if (gWindow == NULL || gRenderer == NULL) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return false;
    }

    // Init renderer color
    SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

    // Initialize SDL_ttf
    if (TTF_Init() == -1) return false;

    // Update screen
    SDL_RenderPresent(gRenderer);

    return true;
}

bool loadFiles() {
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

    return true;
}

bool setTiles() {
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

void cleanUp() {
    gKeenTexture->free();
    gMaskTexture->free();

    for (unsigned int i=0; i<gTiles.size(); i++) {
        for (unsigned int j=0; j<gTiles[i].size(); j++) {
            if (gTiles[i][j] != NULL)
                delete gTiles[i][j];
        }
    }

    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);

    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
}
