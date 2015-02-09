#include <fstream>
#include <iostream>
#include <sstream>
#include "Ampton.h"
#include "BackgroundTile.h"
#include "BlasterShot.h"
#include "Camera.h"
#include "Controller.h"
#include "globals.h"
#include "Item.h"
#include "Level.h"
#include "MovingSprite.h"
#include "Platform.h"
#include "Player.h"
#include "SDL_image.h"
#include "Sparky.h"
#include "Texture.h"
#include "Tile.h"
#include "Timer.h"

using namespace std;

SDL_Window* initWindow();
SDL_Renderer* initRenderer(SDL_Window* window);
bool sdlInit();
bool init(SDL_Window* window, SDL_Renderer* renderer);
bool loadFiles(Texture* keenTexture, Texture* maskTexture);
Level* loadCurrentLevel(Texture* maskTexture);
void cleanUp(SDL_Window* window, SDL_Renderer* renderer, Texture* keenTexture, Texture* maskTexture);

vector< vector<Tile*> > tiles;
const vector< vector<Tile*> >& MovingSprite::tilesRef = tiles;

Level* currentLevel = nullptr;
Level*& MovingSprite::currentLevelRef = currentLevel;
float MovingSprite::timeDelta = 0.0;
float Platform::timeDelta = 0.0;
Level*& Camera::currentLevelRef = currentLevel;

vector<Enemy*> enemyBatch;
const vector<Enemy*>& BlasterShot::enemyBatchRef = enemyBatch;

vector<Item*> itemBatch;
vector<Item*>& Player::itemBatchRef = itemBatch;
vector<Item*>& Item::itemBatchRef = itemBatch;

vector<Platform*> platformBatch;
const vector<Platform*>& Player::platformBatchRef = platformBatch;

vector<BlasterShot*> blasterShotBatch;
vector<BlasterShot*>& BlasterShot::blasterShotBatchRef = blasterShotBatch;

Camera camera;
Camera& Player::cameraRef = camera;

Controller controller;
Controller& Player::controllerRef = controller;

int main (int argc, char **args) {
    // Initialize variables
    bool running = true;
    SDL_Event event;
    Timer fps;

    if (!sdlInit()) return 1;
    SDL_Window* window = initWindow();
    SDL_Renderer* renderer = initRenderer(window);
    if (!init(window, renderer)) return 1;

    Texture* keenTexture = new Texture(renderer);
    Texture* maskTexture = new Texture(renderer);

    if (!loadFiles(keenTexture, maskTexture)) return 1;

    currentLevel = loadCurrentLevel(maskTexture);
    if (currentLevel == nullptr) return 1;

    tiles = currentLevel->getTiles();
    enemyBatch = currentLevel->getEnemies();
    itemBatch = currentLevel->getItems();
    platformBatch = currentLevel->getPlatforms();
    vector<BackgroundTile*> backgroundTiles = currentLevel->getBackgroundTiles();

    Player* player = currentLevel->getPlayer();

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
                    case SDLK_LCTRL: controller.isHoldingCtrl = false; break;
                    case SDLK_SPACE: controller.isHoldingSpace = false; break;
                    case SDLK_LALT: controller.isHoldingAlt = false; break;
                    case SDLK_UP: controller.isHoldingUp = false; break;
                    case SDLK_DOWN: controller.isHoldingDown = false; break;
                    case SDLK_LEFT: controller.isHoldingLeft = false; break;
                    case SDLK_RIGHT: controller.isHoldingRight = false; break;
                }
            }
            // If the user X'd out of the window
            else if (event.type == SDL_QUIT) {
                running = false;
            }
        }

        // Clear screen
        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
        SDL_RenderClear(renderer);

        // Update units
        for (unsigned int i = 0; i < platformBatch.size(); i++)
            platformBatch[i]->update();
        for (unsigned int i = 0; i < enemyBatch.size(); i++)
            enemyBatch[i]->update();
        for (unsigned int i = 0; i < blasterShotBatch.size(); i++)
            blasterShotBatch[i]->update();
        player->update();

        // Update items
        for (unsigned int i = 0; i < itemBatch.size(); i++)
            itemBatch[i]->update();

        // Draw background tiles - Layer 0 (Before units)
        for (unsigned int i = 0; i < backgroundTiles.size(); i++) {
            if (backgroundTiles[i]->getLayer() == 0)
                backgroundTiles[i]->draw(maskTexture, camera.getBox());
        }

        // Draw foreground tiles - Layer 0 (Before units)
        for (unsigned int i = 0; i < tiles.size(); i++) {
            for (unsigned int j = 0; j < tiles[i].size(); j++) {
                if (tiles[i][j] != nullptr && tiles[i][j]->layer == 0)
                    tiles[i][j]->draw(maskTexture, camera.getBox());
            }
        }

        // Draw items
        for (unsigned int i = 0; i < itemBatch.size(); i++)
            itemBatch[i]->draw(keenTexture, camera.getBox());

        // Draw units
        for (unsigned int i = 0; i < platformBatch.size(); i++)
            platformBatch[i]->draw(keenTexture, camera.getBox());
        for (unsigned int i = 0; i < enemyBatch.size(); i++)
            enemyBatch[i]->draw(keenTexture, camera.getBox());
        for (unsigned int i = 0; i < blasterShotBatch.size(); i++)
            blasterShotBatch[i]->draw(keenTexture, camera.getBox());
        player->draw(keenTexture, camera.getBox());

        // Draw background tiles - Layer 1 (After units)
        for (unsigned int i = 0; i < backgroundTiles.size(); i++) {
            if (backgroundTiles[i]->getLayer() == 1)
                backgroundTiles[i]->draw(maskTexture, camera.getBox());
        }

        // Draw foreground tiles - Layer 1 (After units)
        for (unsigned int i = 0; i < tiles.size(); i++) {
            for (unsigned int j = 0; j < tiles[i].size(); j++) {
                if (tiles[i][j] != nullptr && tiles[i][j]->layer == 1)
                    tiles[i][j]->draw(maskTexture, camera.getBox());
            }
        }

        camera.update(player->getBox(), player->getIsOnGround());

        // Update screen
        SDL_RenderPresent(renderer);

        // Cap frame rate
        if (fps.getTicks() < 1000 / FRAMES_PER_SECOND)
            SDL_Delay((1000/FRAMES_PER_SECOND) - fps.getTicks());

        MovingSprite::timeDelta = fps.getDeltaTime();
        Platform::timeDelta = fps.getDeltaTime();
    }

    cleanUp(window, renderer, keenTexture, maskTexture);
    return 0;
}

bool sdlInit() {
    // Init all SDL subsystems
    if (SDL_Init(SDL_INIT_TIMER | SDL_INIT_AUDIO | SDL_INIT_VIDEO) == -1) {
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

    // Init SDL_ttf
    if (TTF_Init() == -1) return false;

    return true;
}

SDL_Window* initWindow() {
    return SDL_CreateWindow(
        "Commander Keen 5",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        SCREEN_WIDTH,
        SCREEN_HEIGHT,
        0
    );
}

SDL_Renderer* initRenderer(SDL_Window* window) {
    return SDL_CreateRenderer(
        window,
        -1,
        0
    );
}

bool init(SDL_Window* window, SDL_Renderer* renderer) {
    // Make sure window was successfully set up
    if (window == nullptr || renderer == nullptr) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return false;
    }

    // Init renderer color
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);

    // Update screen
    SDL_RenderPresent(renderer);

    return true;
}

bool loadFiles(Texture* keenTexture, Texture* maskTexture) {
    string data = "../data/";

    // Load images
    if (!keenTexture->loadFromFile(data + "keensprite.png", true)) {
        printf("Failed to load keen texture!\n");
        return false;
    }
    if (!maskTexture->loadFromFile(data + "masks.png", false)) {
        printf("Failed to load masks texture!\n");
        return false;
    }

    return true;
}

Level* loadCurrentLevel(Texture* maskTexture) {
    // Notes:
    // Does levelWidth / levelHeight matter? Yes, because we have to know when we're at the end of a "row" when reading the file
    // tileCount in file is currently UNUSED

    vector<BackgroundTile*> backgroundTiles;
    vector<Enemy*> enemies;
    vector<Item*> items;
    vector<Platform*> platforms;
    enum class UnitType { NONE, KEEN, SPARKY, AMPTON, PLATFORM };

    int tilesWide = -1;
    int tilesTall = -1;
    int tileCountLayer1 = -1;
    int tileCountLayer2 = -1;

    int xSrc = -1;
    int ySrc = -1;
    int heightVal = -1;
    int collideVal = -1;
    int layerVal = -1;
    int edgeVal = -1;
    int propertyVal = -1;
    int leftHeight = 0;
    int rightHeight = 0;
    bool collideT = false;
    bool collideR = false;
    bool collideB = false;
    bool collideL = false;
    bool isEdge = false;
    int unitVal = -1;
    int itemVal = 0;
    int keenSpawnX = -1;
    int keenSpawnY = -1;

    int x = 0;
    int y = 0;

    ifstream map("../data/level1");
    string line;
    istringstream iss;

    // Special case: First line contains # of tiles wide, # tiles tall, and # tiles (non-blank) per layer
    // For now, it's assumed the number of layers is known (hardcoded)
    getline(map, line);
    iss.str(line);
    iss >> tilesWide;
    iss >> tilesTall;
    iss >> tileCountLayer1;
    iss >> tileCountLayer2;
    if (iss.fail() || tilesWide == -1 || tilesTall == -1 || tileCountLayer1 == -1 || tileCountLayer2 == -1) {
        printf("Error getting metadata from line 1.\n");
        return nullptr;
    }

    // Special case: Second line contains source file absolute path
    // This is mostly used by the level editor. We only want the file name.
    // NOT USED RIGHT NOW, BUT MIGHT BE A GOOD IDEA LATER.
    // Only downside is the level files are computed at runtime...better hardcoded.
    getline(map, line);

    // This is slow because it inits every element when we don't need to
    // Figure out how to assign to it in the code below
    tiles.resize(tilesWide);
    for (int iter = 0; iter < tilesWide; iter++)
        tiles[iter].resize(tilesTall);

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

                tiles[x][y] = nullptr;
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
        iss >> edgeVal;
        isEdge = edgeVal == 1 ? true : false;

        iss >> propertyVal;
        bool isPole = false;
        bool isPoleEdge = false;
        if (propertyVal == 1)
            isPole = true;
        else if (propertyVal == 2)
            isPoleEdge = true;

        iss >> layerVal;

        iss >> unitVal;
        iss >> itemVal;

        if ((UnitType)unitVal == UnitType::KEEN) {
            keenSpawnX = TILE_WIDTH * x;
            keenSpawnY = TILE_HEIGHT * y;
        } else if ((UnitType)unitVal == UnitType::SPARKY) {
            enemies.push_back(new Sparky(TILE_WIDTH * x, TILE_HEIGHT * y));
        } else if ((UnitType)unitVal == UnitType::AMPTON) {
            enemies.push_back(new Ampton(TILE_WIDTH * x, TILE_HEIGHT * y));
        } else if ((UnitType)unitVal == UnitType::PLATFORM) {
            vector<pair<int, int>> dests;
            int tileX = 0;
            int tileY = 0;

            while (1) {
                iss >> tileX;
                iss >> tileY;

                if (iss.fail())
                    break;

                pair<int, int> dest(tileX, tileY);
                dests.push_back(dest);
            }

            platforms.push_back(new Platform(TILE_WIDTH * x, TILE_HEIGHT * y, dests));
        }

        if (itemVal != 0)
            items.push_back(new Item(TILE_WIDTH * x, TILE_HEIGHT * y, itemVal));

        if (collideT || collideR || collideB || collideL || isEdge || isPole || isPoleEdge) {
            tiles[x][y] = new Tile(xSrc, ySrc, x * TILE_WIDTH, y * TILE_HEIGHT, leftHeight, rightHeight,
                collideT, collideR, collideB, collideL, layerVal, isPole, isPoleEdge, isEdge);
        } else {
            backgroundTiles.push_back(new BackgroundTile(xSrc, ySrc, x * TILE_WIDTH, y * TILE_WIDTH, layerVal));
            tiles[x][y] = nullptr;
        }

        x++;
    }

    map.close();

    // TODO: Throw exception instead of returning null
    if (keenSpawnX == -1 || keenSpawnY == -1)
        return nullptr;

    return new Level(
        tilesWide * TILE_WIDTH, tilesTall * TILE_HEIGHT,
        tilesWide, tilesTall,
        tileCountLayer1, tileCountLayer2,
        tiles,
        backgroundTiles,
        enemies,
        items,
        platforms,
        keenSpawnX, keenSpawnY
    );
}

void cleanUp(SDL_Window* window, SDL_Renderer* renderer, Texture* keenTexture, Texture* maskTexture) {
    keenTexture->free();
    maskTexture->free();

    for (unsigned int i = 0; i < tiles.size(); i++) {
        for (unsigned int j = 0; j < tiles[i].size(); j++) {
            if (tiles[i][j] != nullptr)
                delete tiles[i][j];
        }
    }

    delete currentLevel;

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
}
