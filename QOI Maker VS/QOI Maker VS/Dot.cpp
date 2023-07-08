#include "Dot.h"

Dot::Dot(int x, int y, LTexturePtr dotTexture, LTexturePtr redTexture, LTexturePtr greenTexture, LTexturePtr blueTexture)
{
    //Initialize the offsets
    mPosX = x;
    mPosY = y;

    //Set collision circle size
    mCollider.r = DOT_WIDTH / 2;

    //Initialize the velocity
    mVelX = 0;
    mVelY = 0;

    //Move collider relative to the circle
    shiftColliders();

    //Initialize particles
    for (int i = 0; i < TOTAL_PARTICLES; ++i)
    {
        particles[i] = new Particle(mPosX, mPosY, redTexture, greenTexture, blueTexture);
    }

    mDotTexture = dotTexture;
    mRedTexture = redTexture;
    mGreenTexture = greenTexture;
    mBlueTexture = blueTexture;
}

Dot::Dot(LTexturePtr dotTexture) {
    //Initialize the offsets
    mPosX = 0;
    mPosY = 0;

    //Set collision circle size
    mCollider.r = DOT_WIDTH / 2;

    //Initialize the velocity
    mVelX = 0;
    mVelY = 0;

    //Move collider relative to the circle
    shiftColliders();

    //Initialize particles
    for (int i = 0; i < TOTAL_PARTICLES; ++i)
    {
        particles[i] = nullptr;
    }

    mDotTexture = dotTexture;
    mRedTexture = nullptr;
    mGreenTexture = nullptr;
    mBlueTexture = nullptr;
}

Dot::~Dot()
{
    //Delete particles
    for (int i = 0; i < TOTAL_PARTICLES; ++i)
    {
        if (particles[i] != nullptr)
            delete particles[i];
    }
}

void Dot::handleEvent(SDL_Event& e)
{
    //If a key was pressed
    if (e.type == SDL_KEYDOWN && e.key.repeat == 0)
    {
        //Adjust the velocity
        switch (e.key.keysym.sym)
        {
        case SDLK_UP: mVelY -= DOT_VEL; break;
        case SDLK_DOWN: mVelY += DOT_VEL; break;
        case SDLK_LEFT: mVelX -= DOT_VEL; break;
        case SDLK_RIGHT: mVelX += DOT_VEL; break;
        }
    }
    //If a key was released
    else if (e.type == SDL_KEYUP && e.key.repeat == 0)
    {
        //Adjust the velocity
        switch (e.key.keysym.sym)
        {
        case SDLK_UP: mVelY += DOT_VEL; break;
        case SDLK_DOWN: mVelY -= DOT_VEL; break;
        case SDLK_LEFT: mVelX += DOT_VEL; break;
        case SDLK_RIGHT: mVelX -= DOT_VEL; break;
        }
    }
}

void Dot::move(int level_width, int level_height, Tile* tiles[])
{
    //Move the dot left or right
    mPosX += mVelX;

    //If the dot went too far to the left or right
    if ((mPosX < 0) || (mPosX + DOT_WIDTH > level_width))
    {
        //Move back
        mPosX -= mVelX;
    }

    //Move the dot up or down
    mPosY += mVelY;

    //If the dot went too far up or down
    if ((mPosY < 0) || (mPosY + DOT_HEIGHT > level_height))
    {
        //Move back
        mPosY -= mVelY;
    }
}

void Dot::render(LWindowPtr window, int camX, int camY)
{
    //Show the dot relative to the camera
    TextureRenderData renderData;
    renderData.pos.x = mPosX - camX;
    renderData.pos.y = mPosY - camY;
    window->DrawTexture(mDotTexture, renderData);

    //Show particles on top of dot
    renderParticles(window);
}

void Dot::renderParticles(LWindowPtr window)
{
    //Go through particles
    for (int i = 0; i < TOTAL_PARTICLES; ++i)
    {
        //Delete and replace dead particles
        if (particles[i]->isDead())
        {
            delete particles[i];
            particles[i] = new Particle(mPosX, mPosY, mRedTexture, mGreenTexture, mBlueTexture);
        }
    }

    //Show particles
    for (int i = 0; i < TOTAL_PARTICLES; ++i)
    {
        particles[i]->render(window, mShimmerTexture);
    }
}

void Dot::shiftColliders()
{
    //The row offset
    int r = 0;

    //Center the collision box
    mCollider.x = mPosX + (DOT_WIDTH - mCollider.r*2) / 2;

    //Set the collision box at its row offset
    mCollider.y = mPosY + r;

    //Move the row offset down the height of the collision box
    r += mCollider.r;
}

Circle& Dot::getCollider()
{
    return mCollider;
}

int Dot::getPosX() {
    return mPosX;
}

int Dot::getPosY() {
    return mPosY;
}