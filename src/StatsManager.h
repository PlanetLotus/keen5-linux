#ifndef STATSMANAGER_H
#define STATSMANAGER_H

class StatsManager {
    /*
     * The idea here is that Keen collides with an item or ammo, or his score
     * or vitalin gets high enough to generate a new life, and his stats
     * change. The stats are stored here. This should not be tied to a specific
     * level or instance of Keen. It needs to persist throughout the whole
     * game. I'm unclear on whether I want this to also include Keen's
     * inventory. That should probably be a separate class since inventory is
     * really just keycard and keys, and only lasts for one level.
     */
    private:
        int lives = 3;
        int ammo = 5;
        int score = 0;
        int vitalin = 0;
        int requiredScoreForExtraLife = 20000;
        enum class DifficultyLevel { EASY, NORMAL, HARD };
        DifficultyLevel difficulty = DifficultyLevel::NORMAL;

    public:
        void addScore(int value);
        void subtractLives();
        void addAmmo();
        void subtractAmmo();
        void addVitalin();
        int getScore();
};

#endif
