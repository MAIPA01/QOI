#pragma once

#include <SDL.h>
#include <vector>
#include "typedefs.h"

//Button constants

enum LButtonSprite
{
    BUTTON_SPRITE_DEFAULT = 0,
    BUTTON_SPRITE_MOUSE_OUT = 1,
    BUTTON_SPRITE_MOUSE_OVER = 2,
    BUTTON_SPRITE_MOUSE_DOWN = 3,
    BUTTON_SPRITE_MOUSE_UP = 4,
    BUTTON_SPRITE_TOTAL = 5
};

class LButton
{
private:
    Point2DPtr parent;

    //Top left position
    Point2DPtr mPosition;

    //Button Size
    Point2D mSize;

    //Currently used global sprite
    LButtonSprite mCurrentSprite;

    std::vector<LTexturePtr> m_ButtonSprites = std::vector<LTexturePtr>(BUTTON_SPRITE_TOTAL);

    std::vector<LButtonCall> m_ButtonCallsOnClick = std::vector<LButtonCall>();

public:
    //Initializes internal variables
    LButton(const Point2D& pos, int w, int h, const LTexturePtr& defaultSprite, const LTexturePtr& mouseOutSprite = nullptr, const LTexturePtr& mouseOverSprite = nullptr, const LTexturePtr& mouseDownSprite = nullptr, const LTexturePtr& mouseUpSprite = nullptr, const Point2DPtr& parent = nullptr);

    virtual ~LButton();

    //Sets top left position
    void setPosition(const Point2D& pos);

    //Sets top left position
    void setSize(int w, int h);

    void setDefaultSprite(const LTexturePtr& defaultSprite);
    void setMouseOutSprite(const LTexturePtr& mouseOutSprite);
    void setMouseOverSprite(const LTexturePtr& mouseOverSprite);
    void setMouseDownSprite(const LTexturePtr& mouseDownSprite);
    void setMouseUpSprite(const LTexturePtr& mouseUpSprite);

    void setParent(const Point2DPtr& parent);

    void addCallOnClick(const LButtonCall& onClick);

    //Handles mouse event
    void handleEvent(SDL_Event& e);

    const LTexturePtr& getTexture() const;
    
    const Point2DPtr getPosition() const;
    const Point2DPtr getParent() const;
    const Point2D getSize() const;
};

