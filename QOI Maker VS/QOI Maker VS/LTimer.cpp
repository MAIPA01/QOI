#include "LTimer.h"

LTimer::LTimer()
{
    //Initialize the variables
    startTime = std::chrono::system_clock::now();
    pausedTime = std::chrono::system_clock::now();

    mPaused = false;
    mStarted = false;
}

void LTimer::start()
{
    //Start the timer
    mStarted = true;

    //Unpause the timer
    mPaused = false;

    //Get the current clock time
    startTime = std::chrono::system_clock::now();
    pausedTime = std::chrono::system_clock::now();
}

void LTimer::stop()
{
    //Stop the timer
    mStarted = false;

    //Unpause the timer
    mPaused = false;

    //Clear tick variables
    startTime = std::chrono::system_clock::now();
    pausedTime = std::chrono::system_clock::now();
}

void LTimer::pause()
{
    //If the timer is running and isn't already paused
    if (mStarted && !mPaused)
    {
        //Pause the timer
        mPaused = true;

        //Calculate the paused ticks
        pausedTime = std::chrono::system_clock::now();
    }
}

void LTimer::unpause()
{
    //If the timer is running and paused
    if (mStarted && mPaused)
    {
        //Unpause the timer
        mPaused = false;
    }
}

float LTimer::getTime() const
{
    //The actual timer time
    std::chrono::duration<float> time;

    //If the timer is running
    if (mStarted)
    {
        //If the timer is paused
        if (mPaused)
        {
            //Return the number of ticks when the timer was paused
            time = pausedTime - startTime;
        }
        else
        {
            //Return the current time minus the start time
            time = std::chrono::system_clock::now() - startTime;
        }
    }

    return time.count();
}

bool LTimer::isStarted() const
{
    //Timer is running and paused or unpaused
    return mStarted;
}

bool LTimer::isPaused() const
{
    //Timer is running and paused
    return mPaused && mStarted;
}