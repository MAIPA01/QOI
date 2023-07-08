#pragma once
#include "LTexture.h"
#include "LWindow.h"
#include "typedefs.h"

//Particle count
const int TOTAL_PARTICLES = 20;

class Particle
{
private:
    //Offsets
    int mPosX, mPosY;

    //Current frame of animation
    int mFrame;

    //Type of particle
    LTexturePtr mTexture;
public:
    //Initialize position and animation
    Particle(int x, int y, LTexturePtr gRedTexture, LTexturePtr gGreenTexture, LTexturePtr gBlueTexture);

    //Shows the particle
    void render(LWindowPtr window, LTexturePtr gShimmerTexture);

    //Checks if particle is dead
    bool isDead();
};