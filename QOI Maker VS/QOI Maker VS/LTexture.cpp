#include "LTexture.h"

LTexture::LTexture()
{
    //Initialize
    m_Texture = NULL;
    m_width = 0;
    m_height = 0;
}

LTexture::~LTexture()
{
    //Deallocate
    free();
}

bool LTexture::loadFromFile(std::string path, SDL_Renderer* renderer, int colorKeyR, int colorKeyG, int colorKeyB)
{
    //Get rid of preexisting texture
    free();

    //The final texture
    SDL_Texture* newTexture = nullptr;

    //Load image at specified path
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    if (loadedSurface == nullptr)
    {
        printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
    }
    else
    {
        //Color key image
        SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, colorKeyR, colorKeyG, colorKeyB));
        //Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
        if (newTexture == nullptr)
        {
            printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
        }
        else
        {
            //Get image dimensions
            m_width = loadedSurface->w;
            m_height = loadedSurface->h;
        }

        //Get rid of old loaded surface
        SDL_FreeSurface(loadedSurface);
    }

    //Return success
    m_Texture = newTexture;
    return m_Texture != nullptr;
}

#if defined(SDL_TTF_MAJOR_VERSION)
bool LTexture::loadFromRenderedText(std::string textureText, SDL_Color textColor, TTF_Font* gFont, SDL_Renderer* renderer)
{
    //Get rid of preexisting texture
    free();

    //Render text surface
    SDL_Surface* textSurface = TTF_RenderText_Solid(gFont, textureText.c_str(), textColor);
    if (textSurface == nullptr)
    {
        printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
    }
    else
    {
        //Create texture from surface pixels
        m_Texture = SDL_CreateTextureFromSurface(renderer, textSurface);
        if (m_Texture == nullptr)
        {
            printf("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
        }
        else
        {
            //Get image dimensions
            m_width = textSurface->w;
            m_height = textSurface->h;
        }

        //Get rid of old surface
        SDL_FreeSurface(textSurface);
    }

    //Return success
    return m_Texture != nullptr;
}
#endif

void LTexture::free()
{
    //Free texture if it exists
    if (m_Texture != nullptr)
    {
        SDL_DestroyTexture(m_Texture);
        m_Texture = nullptr;
        m_width = 0;
        m_height = 0;
    }
}

void LTexture::setColor(uint8_t red, uint8_t green, uint8_t blue)
{
    //Modulate texture
    SDL_SetTextureColorMod(m_Texture, red, green, blue);
}

void LTexture::setBlendMode(SDL_BlendMode blending)
{
    //Set blending function
    SDL_SetTextureBlendMode(m_Texture, blending);
}

void LTexture::setAlpha(Uint8 alpha)
{
    //Modulate texture alpha
    SDL_SetTextureAlphaMod(m_Texture, alpha);
}

int LTexture::getWidth() const
{
    return m_width;
}

int LTexture::getHeight() const
{
    return m_height;
}

SDL_Texture* LTexture::getSDLTexture() const
{
    return m_Texture;
}
