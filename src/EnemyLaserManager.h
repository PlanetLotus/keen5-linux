#ifndef ENEMYLASERMANAGER_H
#define ENEMYLASERMANAGER_H

#include <vector>
#include "Texture.h"

class Player;

class EnemyLaserManager {
    private:
        // Value is in seconds
        const int cooldown = 3;
        float elapsedCooldown;

        Player* keen;
        Texture* texture;
    public:
        enum class Direction { LEFT, RIGHT, UP, DOWN };
        struct LaserData {
            std::pair<int, int> spawnCoords;
            Direction direction;
        };

        EnemyLaserManager();
        std::vector<LaserData> laserDataList;
        static float timeDelta;
        void update();
        void setPlayer(Player* player);
};

#endif
