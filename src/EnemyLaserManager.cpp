#include "EnemyLaser.h"
#include "EnemyLaserManager.h"
#include "Player.h"

using namespace std;

EnemyLaserManager::EnemyLaserManager() {
    elapsedCooldown = 0;
    keen = nullptr;
    texture = nullptr;
}

void EnemyLaserManager::update() {
    elapsedCooldown += timeDelta;

    if (elapsedCooldown < cooldown)
        return;

    elapsedCooldown = 0;

    for (size_t i = 0; i < laserDataList.size(); i++) {
        int xShotVel = 0;
        int yShotVel = 0;

        Direction direction = laserDataList[i].direction;
        if (direction == Direction::LEFT)
            xShotVel = -1;
        else if (direction == Direction::RIGHT)
            xShotVel = 1;
        else if (direction == Direction::UP)
            yShotVel = -1;
        else if (direction == Direction::DOWN)
            yShotVel = 1;

        new EnemyLaser(laserDataList[i].spawnCoords.first, laserDataList[i].spawnCoords.second, xShotVel, yShotVel, keen);
    }
}

void EnemyLaserManager::setPlayer(Player* player) { keen = player; }
