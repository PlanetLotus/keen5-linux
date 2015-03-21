#include "StatsManager.h"

void StatsManager::resetIsChanged() {
    isScoreChanged = false;
    isLivesChanged = false;
    isAmmoChanged = false;
    isVitalinChanged = false;
}

void StatsManager::addScore(int value) {
    score += value;
    isScoreChanged = true;

    if (score >= requiredScoreForExtraLife) {
        requiredScoreForExtraLife *= 2;
        lives++;
        isLivesChanged = true;
    }
}

void StatsManager::subtractLives() {
    // Can be negative. When it hits -1, the game should pick this up when respawning and end the game.
    lives--;
    isLivesChanged = true;
}

void StatsManager::addAmmo() {
    if (difficulty == DifficultyLevel::NORMAL) {
        ammo += 5;
        isAmmoChanged = true;
    }
}

void StatsManager::subtractAmmo() {
    if (ammo > 0) {
        ammo--;
    }
    isAmmoChanged = true;
}

void StatsManager::addVitalin() {
    vitalin++;
    isVitalinChanged = true;

    if (vitalin == 100) {
        vitalin = 0;
        lives++;
    }
}

int StatsManager::getScore() { return score; }
int StatsManager::getAmmo() { return ammo; }
int StatsManager::getLives() { return lives; }
int StatsManager::getVitalin() { return vitalin; }
bool StatsManager::getIsScoreChanged() { return isScoreChanged; }
bool StatsManager::getIsAmmoChanged() { return isAmmoChanged; }
bool StatsManager::getIsLivesChanged() { return isLivesChanged; }
bool StatsManager::getIsVitalinChanged() { return isVitalinChanged; }
