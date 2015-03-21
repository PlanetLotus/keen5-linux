#include "Hud.h"
#include "StatsManager.h"
#include "Texture.h"

Hud::Hud(Texture* texture, StatsManager* statsManager) {
    this->texture = texture;
    this->statsManager = statsManager;

    srcClip = { srcClipX, srcClipY, srcClipWidth, srcClipHeight };

    valueSrcClips = {
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr
    };
}

void Hud::updateProperty(int value, int minClipIndex, int maxClipIndex) {
    int clipIndex = maxClipIndex;
    int digits = value;
    int digit = 0;
    bool isDigitZeroAndMostSigificantDigit = false;
    bool isValueZeroButNotDigit = false;

    while (clipIndex >= minClipIndex) {
        digit = digits % 10;

        isDigitZeroAndMostSigificantDigit = digit == 0 && digits / 10 == 0;
        isValueZeroButNotDigit = digit != 0 || clipIndex != maxClipIndex;

        if (isDigitZeroAndMostSigificantDigit && isValueZeroButNotDigit) {
            valueSrcClips[clipIndex] = &grayEightClip;
        } else {
            valueSrcClips[clipIndex] = clipsByDigit[digit];
        }

        digits /= 10;
        clipIndex--;
    }
}

void Hud::update() {
    if (statsManager->getIsScoreChanged()) {
        updateProperty(statsManager->getScore(), clipMinIndexScore, clipMaxIndexScore);
    }

    if (statsManager->getIsLivesChanged()) {
        updateProperty(statsManager->getLives(), clipMinIndexLives, clipMaxIndexLives);
    }

    if (statsManager->getIsAmmoChanged()) {
        updateProperty(statsManager->getAmmo(), clipMinIndexAmmo, clipMaxIndexAmmo);
    }
}

void Hud::draw() {
    texture->render(screenMarginX, screenMarginY, &srcClip);

    for (int i = clipMinIndexScore; i <= clipMaxIndexScore; i++) {
        if (valueSrcClips[i] == nullptr)
            continue;

        texture->render(
            screenMarginX + score1OffsetX + valueWidth * i + spaceBetweenValuesX * i,
            screenMarginY + row1OffsetY,
            valueSrcClips[i]
        );
    }

    for (int i = clipMinIndexLives; i <= clipMaxIndexLives; i++) {
        if (valueSrcClips[i] == nullptr)
            continue;

        int livesValueIndex = i - clipMinIndexLives;

        texture->render(
            screenMarginX + lives1OffsetX + valueWidth * livesValueIndex + spaceBetweenValuesX * livesValueIndex,
            screenMarginY + row2OffsetY,
            valueSrcClips[i]
        );
    }

    for (int i = clipMinIndexAmmo; i <= clipMaxIndexAmmo; i++) {
        if (valueSrcClips[i] == nullptr)
            continue;

        int ammoValueIndex = i - clipMinIndexAmmo;

        texture->render(
            screenMarginX + ammo1OffsetX + valueWidth * ammoValueIndex + spaceBetweenValuesX * ammoValueIndex,
            screenMarginY + row2OffsetY,
            valueSrcClips[i]
        );
    }
}
