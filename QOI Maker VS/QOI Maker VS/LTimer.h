#pragma once
#include <SDL.h>
#include <chrono>

//The application time based timer
class LTimer
{
private:
    //The clock time when the timer started
    std::chrono::system_clock::time_point startTime;

    //The ticks stored when the timer was paused
    std::chrono::system_clock::time_point pausedTime;

    //The timer status
    bool mPaused;
    bool mStarted;
public:
    //Initializes variables
    LTimer();

    //The various clock actions
    void start();
    void stop();
    void pause();
    void unpause();

    //Gets the timer's time
    float getTime() const;

    //Checks the status of the timer
    bool isStarted() const;
    bool isPaused() const;
};

