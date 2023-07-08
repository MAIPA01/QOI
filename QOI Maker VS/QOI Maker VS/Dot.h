#pragma once
#include <SDL.h>
#include "LTexture.h"
#include <vector>
#include "Particle.h"
#include "Tile.h"
#include "LWindow.h"
#include "typedefs.h"

//A circle stucture
struct Circle
{
    int x, y;
    int r;
};

class Dot
{
private:
    LTexturePtr mDotTexture;

    //The X and Y offsets of the dot
    int mPosX, mPosY;

    //The velocity of the dot
    int mVelX, mVelY;

    //Dot's collision circle
    Circle mCollider;

    //Moves the collision boxes relative to the dot's offset
    void shiftColliders();

    //The particles
    Particle* particles[TOTAL_PARTICLES];
    LTexturePtr mShimmerTexture;
    LTexturePtr mRedTexture;
    LTexturePtr mGreenTexture;
    LTexturePtr mBlueTexture;

    //Shows the particles
    void renderParticles(LWindowPtr window);
public:
    //The dimensions of the dot
    static const int DOT_WIDTH = 20;
    static const int DOT_HEIGHT = 20;

    //Maximum axis velocity of the dot
    static const int DOT_VEL = 1;

    //Initializes the variables
    Dot(int x, int y, LTexturePtr dotTexture, LTexturePtr redTexture, LTexturePtr greenTexture, LTexturePtr blueTexture);
    Dot(LTexturePtr dotTexture);

    //Deallocates particles
    ~Dot();

    //Takes key presses and adjusts the dot's velocity
    void handleEvent(SDL_Event& e);

    //Moves the dot and checks collision
    void move(int level_width, int level_height, Tile* tiles[]);

    //Shows the dot on the screen
    void render(LWindowPtr window, int camX, int camY);

    //Gets collision circle
    Circle& getCollider();

    //Position accessors
    int getPosX();
    int getPosY();
};

