#include "StatsManager.h"

void StatsManager::addScore(int value) {
    score += value;

    if (score >= requiredScoreForExtraLife)
        requiredScoreForExtraLife *= 2;

    lives++;
}

void StatsManager::subtractLives() {
    // Can be negative. When it hits -1, the game should pick this up when respawning and end the game.
    lives--;
}

void StatsManager::addAmmo() {
    if (difficulty == DifficultyLevel::NORMAL)
        ammo += 5;
}

void StatsManager::subtractAmmo() {
    if (ammo > 0)
        ammo--;
}

void StatsManager::addVitalin() {
    vitalin++;

    if (vitalin == 100) {
        vitalin = 0;
        lives++;
    }
}

int StatsManager::getScore() { return score; }
