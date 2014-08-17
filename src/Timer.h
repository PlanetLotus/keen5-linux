#ifndef TIMER_H
#define TIMER_H

#include "SDL.h"

extern const int FRAMES_PER_SECOND;

class Timer {
    private:
        int startTicks;
        int pausedTicks;

        bool started;
        bool paused;
    public:
        Timer();

        void start();
        void stop();
        void pause();
        void unpause();

        int getTicks();

        bool isStarted();
        bool isPaused();
};

#endif
