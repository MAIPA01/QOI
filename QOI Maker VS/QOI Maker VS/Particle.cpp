#include "Particle.h"

Particle::Particle(int x, int y, LTexturePtr gRedTexture, LTexturePtr gGreenTexture, LTexturePtr gBlueTexture)
{
    //Set offsets
    mPosX = x - 5 + (rand() % 25);
    mPosY = y - 5 + (rand() % 25);

    //Initialize animation
    mFrame = rand() % 5;

    //Set type
    switch (rand() % 3)
    {
    case 0: mTexture = gRedTexture; break;
    case 1: mTexture = gGreenTexture; break;
    case 2: mTexture = gBlueTexture; break;
    }
}

void Particle::render(LWindowPtr window, LTexturePtr gShimmerTexture)
{
    //Show image
    TextureRenderData data;
    data.pos.x = mPosX;
    data.pos.y = mPosY;
    window->DrawTexture(mTexture, data);

    //Show shimmer
    if (mFrame % 2 == 0)
    {
        window->DrawTexture(gShimmerTexture, data);
    }

    //Animate
    mFrame++;
}

bool Particle::isDead()
{
    return mFrame > 10;
}