#ifndef LTEXTURE_H
#define LTEXTURE_H

#include "typedefs.h"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <string>
#include <cmath>

struct TextureRenderData {
    Point2D pos;
    SDL_Rect* clip = nullptr;
    double angle = 0.0;
    SDL_Point* center = nullptr;
    SDL_RendererFlip flip = SDL_FLIP_NONE;
};

class LTexture
{
private:
	SDL_Texture* m_Texture;

	int m_width;
	int m_height;
public:
    //Initializes variables
    LTexture();

    //Deallocates memory
    virtual ~LTexture();

    //Loads image at specified path
    bool loadFromFile(std::string path, SDL_Renderer* renderer, int colorKeyR = 0x00, int colorKeyG = 0xFF, int colorKeyB = 0xFF);

#if defined(SDL_TTF_MAJOR_VERSION)
    //Creates image from font string
    bool loadFromRenderedText(std::string textureText, SDL_Color textColor, TTF_Font* gFont, SDL_Renderer* renderer);
#endif

    //Deallocates texture
    void free();

    //Set color modulation
    void setColor(uint8_t red, uint8_t green, uint8_t blue);

    //Set blending
    void setBlendMode(SDL_BlendMode blending);

    //Set alpha modulation
    void setAlpha(Uint8 alpha);

    //Gets image dimensions
    int getWidth() const;
    int getHeight() const;

    SDL_Texture* getSDLTexture() const;
};

#endif //LTEXTURE_H

