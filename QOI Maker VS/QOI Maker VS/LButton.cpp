#include "LButton.h"

LButton::LButton(const Point2D& pos, int w, int h, const LTexturePtr& defaultSprite, const LTexturePtr& mouseOutSprite, const LTexturePtr& mouseOverSprite, const LTexturePtr& mouseDownSprite, const LTexturePtr& mouseUpSprite, const Point2DPtr& parent)
{
    mPosition = std::make_shared<Point2D>(pos);

    mSize.x = w;
    mSize.y = h;

    mCurrentSprite = BUTTON_SPRITE_DEFAULT;

    m_ButtonSprites[BUTTON_SPRITE_DEFAULT] = defaultSprite;
    m_ButtonSprites[BUTTON_SPRITE_MOUSE_OUT] = mouseOutSprite;
    m_ButtonSprites[BUTTON_SPRITE_MOUSE_OVER] = mouseOverSprite;
    m_ButtonSprites[BUTTON_SPRITE_MOUSE_DOWN] = mouseDownSprite;
    m_ButtonSprites[BUTTON_SPRITE_MOUSE_UP] = mouseUpSprite;
}

LButton::~LButton()
{
}

void LButton::setPosition(const Point2D& pos)
{
    mPosition->x = pos.x;
    mPosition->y = pos.y;
}

void LButton::setSize(int w, int h)
{
    mSize.x = w;
    mSize.y = h;
}

void LButton::setDefaultSprite(const LTexturePtr& defaultSprite)
{
    m_ButtonSprites[BUTTON_SPRITE_DEFAULT] = defaultSprite;
}

void LButton::setMouseOutSprite(const LTexturePtr& mouseOutSprite)
{
    m_ButtonSprites[BUTTON_SPRITE_MOUSE_OUT] = mouseOutSprite;
}

void LButton::setMouseOverSprite(const LTexturePtr& mouseOverSprite)
{
    m_ButtonSprites[BUTTON_SPRITE_MOUSE_OVER] = mouseOverSprite;
}

void LButton::setMouseDownSprite(const LTexturePtr& mouseDownSprite)
{
    m_ButtonSprites[BUTTON_SPRITE_MOUSE_DOWN] = mouseDownSprite;
}

void LButton::setMouseUpSprite(const LTexturePtr& mouseUpSprite)
{
    m_ButtonSprites[BUTTON_SPRITE_MOUSE_UP] = mouseUpSprite;
}

void LButton::setParent(const Point2DPtr& parent)
{
    this->parent = parent;
}

void LButton::addCallOnClick(const LButtonCall& onClick)
{
    m_ButtonCallsOnClick.push_back(onClick);
}

void LButton::handleEvent(SDL_Event& e)
{
    int minX, maxX, minY, maxY;
    minX = parent != nullptr ? mPosition->x + parent->x : mPosition->x;
    maxX = parent != nullptr ? mPosition->x + mSize.x + parent->x : mPosition->x + mSize.x;
    minY = parent != nullptr ? mPosition->y + parent->y : mPosition->y;
    maxY = parent != nullptr ? mPosition->y + mSize.y + parent->y : mPosition->y + mSize.y;

    //If mouse event happened
    if (e.type == SDL_MOUSEMOTION || e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_MOUSEBUTTONUP)
    {
        //Get mouse position
        int x, y;
        SDL_GetMouseState(&x, &y);
        //Check if mouse is in button
        bool inside = true;

        //Mouse is left of the button
        if (x < minX)
        {
            inside = false;
        }
        //Mouse is right of the button
        else if (x > maxX)
        {
            inside = false;
        }
        //Mouse above the button
        else if (y < minY)
        {
            inside = false;
        }
        //Mouse below the button
        else if (y > maxY)
        {
            inside = false;
        }
        //Mouse is outside button
        if (!inside)
        {
            mCurrentSprite = BUTTON_SPRITE_MOUSE_OUT;
        }
        //Mouse is inside button
        else
        {
            //Set mouse over sprite
            switch (e.type)
            {
            case SDL_MOUSEMOTION:
                mCurrentSprite = BUTTON_SPRITE_MOUSE_OVER;
                break;

            case SDL_MOUSEBUTTONDOWN:
                mCurrentSprite = BUTTON_SPRITE_MOUSE_DOWN;
                for (auto& onClick : m_ButtonCallsOnClick) {
                    onClick();
                }
                break;

            case SDL_MOUSEBUTTONUP:
                mCurrentSprite = BUTTON_SPRITE_MOUSE_UP;
                break;
            }
        }
    }
}

const LTexturePtr& LButton::getTexture() const
{
    if (m_ButtonSprites[mCurrentSprite] != nullptr) {
        return m_ButtonSprites[mCurrentSprite];
    }
    return m_ButtonSprites[BUTTON_SPRITE_DEFAULT];
}

const Point2DPtr LButton::getPosition() const
{
    return mPosition;
}

const Point2DPtr LButton::getParent() const
{
    return parent;
}

const Point2D LButton::getSize() const
{
    return mSize;
}
