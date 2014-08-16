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
}

void Camera::Update(SDL_Rect keenHitbox, bool isOnGround) {
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
    if (box.x < 0)
        box.x = 0;
    else if (box.x > LEVEL_WIDTH - box.w)
        box.x = LEVEL_WIDTH - box.w;

    if (box.y < 0) {
        yOffset -= box.y;
        box.y = 0;
    } else if (box.y + box.h > LEVEL_HEIGHT) {
        yOffset -= box.y + box.h - LEVEL_HEIGHT;
        box.y = LEVEL_HEIGHT - box.h;
    }
}

void Camera::LookUp() {
    yOffset -= 5;
}

void Camera::LookDown() {
    yOffset += 5;
}

SDL_Rect Camera::GetBox() { return box; }
int Camera::GetX() { return box.x; }
int Camera::GetY() { return box.y; }
int Camera::GetRight() { return box.x + box.w; }
int Camera::GetBottom() { return box.y + box.h; }
int Camera::GetTopMargin() { return topMargin; }
int Camera::GetBottomMargin() { return bottomMargin; }
