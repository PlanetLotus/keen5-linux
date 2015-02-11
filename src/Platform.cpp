#include "globals.h"
#include "helpers.h"
#include "Platform.h"
#include "Player.h"
#include "Texture.h"
#include "Tile.h"

using namespace std;

Platform::Platform(int spawnX, int spawnY, vector<pair<int, int>> dests) {
    hitbox.x = spawnX;
    hitbox.y = spawnY + TILE_HEIGHT * 3 / 4;
    hitbox.w = TILE_WIDTH * 2;
    hitbox.h = TILE_HEIGHT;

    srcRect.x = TILE_WIDTH * 14;
    srcRect.y = TILE_HEIGHT * 8;
    srcRect.w = TILE_WIDTH * 2;
    srcRect.h = TILE_HEIGHT;

    keen = nullptr;

    xVel = 0;
    yVel = 0;

    numPaths = dests.size() + 1;
    path.reserve(numPaths);

    pair<int, int> spawnPair(spawnX, spawnY);
    path.push_back(spawnPair);

    for (unsigned int i = 0; i < dests.size(); i++)
        path.push_back(dests[i]);

    for (int i = 0; i < numPaths; i++)
        path[i].second += TILE_HEIGHT * 3 / 4;

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
    bool xReached = hitbox.x == currentDest.first;
    bool yReached = hitbox.y == currentDest.second;

    if (!xReached || !yReached)
        return;

    currentDestIndex++;

    if (currentDestIndex == numPaths)
        currentDestIndex = 0;

    currentDest = path[currentDestIndex];
}

void Platform::setVelocity() {
    if (currentDest.first < hitbox.x + xVel)
        xVel = -speed;
    else if (currentDest.first == hitbox.x + xVel)
        xVel = 0;
    else
        xVel = speed;

    if (currentDest.second < hitbox.y + yVel)
        yVel = -speed;
    else if (currentDest.second == hitbox.y + yVel)
        yVel = 0;
    else
        yVel = speed;

    xVel *= timeDelta;
    yVel *= timeDelta;
}

void Platform::update() {
    // Reset platformStandingOn if it points to this platform
    if (keen->platformStandingOn == this)
        keen->platformStandingOn = nullptr;

    // Check if player is standing on this platform. If so, store handle to it in Player.
    SDL_Rect keenBox = keen->getBox();
    if (playerIsStandingOnThis(keenBox))
        keen->platformStandingOn = this;

    // Check to see if next destination is reached. If so, move toward next one.
    updateDest();

    setVelocity();

    // Don't overshoot destination
    int xDistToGo = currentDest.first - hitbox.x;
    if ((xVel < 0 && xVel < xDistToGo) || (xVel > 0 && xVel > xDistToGo))
        xVel = xDistToGo;

    int yDistToGo = currentDest.second - hitbox.y;
    if ((yVel < 0 && yVel < yDistToGo) || (yVel > 0 && yVel > yDistToGo))
        yVel = yDistToGo;

    // Update Platform
    hitbox.x += xVel;
    hitbox.y += yVel;
}

void Platform::draw(Texture* texture, SDL_Rect cameraBox) {
    // Move drawing down by half a tile
    int offsetY = TILE_HEIGHT / 2;
    int destY = hitbox.y - offsetY;

    texture->render(hitbox.x - cameraBox.x, destY - cameraBox.y, &srcRect);
}

SDL_Rect Platform::getBox() { return hitbox; }
float Platform::getXVel() { return xVel; }
float Platform::getYVel() { return yVel; }
void Platform::setPlayer(Player* player) { keen = player; }
