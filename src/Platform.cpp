#include "globals.h"
#include "helpers.h"
#include "Platform.h"
#include "Player.h"
#include "Texture.h"
#include "Tile.h"

using namespace std;

Platform::Platform(int spawnX, int spawnY, vector<pair<int, int>> dests) {
    hitbox.x = spawnX;
    hitbox.y = spawnY;
    hitbox.w = TILE_WIDTH * 2;
    hitbox.h = TILE_HEIGHT;

    srcRect.x = TILE_WIDTH * 14;
    srcRect.y = TILE_HEIGHT * 8;
    srcRect.w = TILE_WIDTH * 2;
    srcRect.h = TILE_HEIGHT * 2;

    keen = nullptr;

    xVel = 0;
    yVel = 0;

    numPaths = dests.size() + 1;
    path.reserve(numPaths);

    pair<int, int> spawnPair(spawnX, spawnY);
    path.push_back(spawnPair);

    for (unsigned int i = 0; i < dests.size(); i++)
        path.push_back(dests[i]);

    currentDestIndex = 0;
    currentDest = path[currentDestIndex];
}

bool Platform::playerIsStandingOnThis(SDL_Rect keenBox) {
    int platformLeft = hitbox.x;
    int platformRight = hitbox.x + hitbox.w;
    int platformTop = hitbox.y;
    int keenRight = keenBox.x + keenBox.w;
    int keenLeft = keenBox.x;
    int keenBottom = keenBox.y + keenBox.h;

    if (keenRight <= platformLeft) return false;
    if (keenLeft >= platformRight) return false;

    return keenBottom == platformTop;
}

void Platform::updateDest() {
    // Check if reached current destination. If so, get the next destination and update velocities accordingly.
    printf("%d,%d %d,%d %d\n", hitbox.x, hitbox.y, currentDest.first, currentDest.second, currentDestIndex);
    bool xReached = hitbox.x == currentDest.first ||
        (xVel < 0 && hitbox.x < currentDest.first) ||
        (xVel > 0 && hitbox.x > currentDest.first);
    bool yReached = hitbox.y == currentDest.second ||
        (yVel < 0 && hitbox.y < currentDest.second) ||
        (yVel > 0 && hitbox.y > currentDest.second);

    if (!xReached || !yReached)
        return;

    currentDestIndex++;

    if (currentDestIndex == numPaths)
        currentDestIndex = 0;

    currentDest = path[currentDestIndex];

    // Aim toward new dest
    xVel = currentDest.first < hitbox.x
        ? -speed
        : speed;

    yVel = currentDest.second < hitbox.y
        ? -speed
        : speed;
}

void Platform::update() {
    // 0) Reset platformStandingOn if it points to this platform
    if (keen->platformStandingOn == this)
        keen->platformStandingOn = nullptr;

    // 1) Check if player is standing on this platform. If so, store handle to it in Player.
    SDL_Rect keenBox = keen->getBox();
    if (playerIsStandingOnThis(keenBox))
        keen->platformStandingOn = this;

    // Check to see if next destination is reached. If so, move toward next one.
    updateDest();

    // 2) Update Platform
    hitbox.x += xVel * timeDelta;
    hitbox.y += yVel * timeDelta;
}

void Platform::draw(Texture* texture, SDL_Rect cameraBox) {
    // Center the hitbox vertically
    int offsetY = srcRect.h - TILE_HEIGHT;
    int destY = hitbox.y - offsetY;

    texture->render(hitbox.x - cameraBox.x, destY - cameraBox.y, &srcRect);
}

SDL_Rect Platform::getBox() { return hitbox; }
float Platform::getXVel() { return xVel; }
float Platform::getYVel() { return yVel; }
void Platform::setPlayer(Player* player) { keen = player; }
