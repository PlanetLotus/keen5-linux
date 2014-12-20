#include "Camera.h"
#include "globals.h"

Camera::Camera() {
    box.x = 0;
    box.y = 0;
    box.w = SCREEN_WIDTH;
    box.h = SCREEN_HEIGHT;

    topMargin = box.y + SCREEN_HEIGHT / 20;
    bottomMargin = box.y + box.h - SCREEN_HEIGHT / 20;
    leftBubble = box.x + SCREEN_WIDTH / 3;
    rightBubble = box.x + SCREEN_WIDTH * 2 / 3;
    yOffset = 0;
    isDisabled = false;
}

void Camera::update(SDL_Rect keenHitbox, bool isOnGround) {
    if (isDisabled) return;

    leftBubble = box.x + SCREEN_WIDTH / 3;
    rightBubble = box.x + SCREEN_WIDTH * 2 / 3;

    // Update bubble if Keen moves outside of it
    if (keenHitbox.x < leftBubble) {
        leftBubble = keenHitbox.x;
    } else if (keenHitbox.x + keenHitbox.w > rightBubble) {
        rightBubble = keenHitbox.x + keenHitbox.w;
        leftBubble = rightBubble - SCREEN_WIDTH / 3;
    }

    topMargin = box.y + SCREEN_HEIGHT / 20;
    bottomMargin = box.y + box.h - SCREEN_HEIGHT / 20;

    // Center camera horizontally over bubble
    box.x = leftBubble - SCREEN_WIDTH / 3;

    // Center vertically if keen is on ground
    if (isOnGround) {
        int desiredY = (keenHitbox.y + keenHitbox.h / 2) - SCREEN_HEIGHT / 2;
        box.y -= (box.y - desiredY - yOffset) / smoothUpdateFactor;
    } else if (keenHitbox.y < topMargin) {
        // if not on ground, update MARGIN ONLY if keen is at camera margin
        box.y -= topMargin - keenHitbox.y;
        yOffset = 0;
    } else if (keenHitbox.y + keenHitbox.h > bottomMargin) {
        box.y += keenHitbox.y + keenHitbox.h - bottomMargin;
        yOffset = 0;
    } else {
        yOffset = 0;
    }

    // Keep camera in level bounds
    /*
    int levelWidth = currentLevelRef->getWidth();
    int levelHeight = currentLevelRef->getHeight();
    */
    int levelWidth = 0;
    int levelHeight = 0;

    if (box.x < 0)
        box.x = 0;
    else if (box.x > levelWidth - box.w)
        box.x = levelWidth - box.w;

    if (box.y < 0) {
        yOffset -= box.y;
        box.y = 0;
    } else if (box.y + box.h > levelHeight) {
        yOffset -= box.y + box.h - levelHeight;
        box.y = levelHeight - box.h;
    }
}

void Camera::lookUp() {
    yOffset -= 5;
}

void Camera::lookDown() {
    yOffset += 5;
}

SDL_Rect Camera::getBox() { return box; }
int Camera::getX() { return box.x; }
int Camera::getY() { return box.y; }
int Camera::getRight() { return box.x + box.w; }
int Camera::getBottom() { return box.y + box.h; }
int Camera::getTopMargin() { return topMargin; }
int Camera::getBottomMargin() { return bottomMargin; }
