#pragma once
#include <SDL.h>
#include <string>
#include <vector>
#include "typedefs.h"

class LWindow
{
private:
    //Window data
    std::string mTitle;
    SDL_Window* mWindow;
    SDL_Renderer* mRenderer;
    int mWindowID;
    int mWindowDisplayID;

    //Window Cameras
    LCameraPtr mainCamera = nullptr;
    std::vector<LCameraPtr> cameras = std::vector<LCameraPtr>();

    //Window dimensions
    int mWidth;
    int mHeight;

    //Window focus
    bool mMouseFocus;
    bool mKeyboardFocus;
    bool mFullScreen;
    bool mMinimized;
    bool mShown;

    Point2D screenPointTranslation(const Point2D& point);
    void DrawMyLine(const Point2D& begin, const Point2D& end, const SDL_Color& color);

public:
    //Intializes internals
    LWindow(const std::string& title, unsigned int screen_width, unsigned int screen_height);

    //Creates window
    bool init(bool isResizable = true);

    //Handles window events
    void handleEvent(SDL_Event& e);

    //Focuses on window
    void focus();

    //Clears windows content
    void clear();

    //Draws One Pixel
    void DrawPixel(const Point2D& pos, const SDL_Color& color);

    //Draws List of Pixels
    void DrawPixels(const std::vector<Point2D>& points, const SDL_Color& color);

    //Draws Line
    void DrawLine(const Point2D& begin, const Point2D& end, const SDL_Color& color);

    //Draws Lines
    void DrawLines(const std::vector<Point2D>& points, const SDL_Color& color);

    //Draws Triangle
    void DrawTriangle(const Point2D& point1, const Point2D& point2, const Point2D& point3, const SDL_Color& color);

    //Draws Filled Triangle
    void DrawFilledTriangle(const Point2D& point1, const Point2D& point2, const Point2D& point3, const SDL_Color& color);

    //Draws Circle
    void DrawCircle(const Point2D& center, int radius, const SDL_Color& color);

    //Draws Filled Circle
    void DrawFilledCircle(const Point2D& center, int radius, const SDL_Color& color);

    //Draws Texture
    void DrawTexture(const LTexturePtr& texture, const TextureRenderData& renderData);

    //Draws Button
    void DrawButton(const LButtonPtr& button, const TextureRenderData& renderData);

    //Draws Text
    void DrawText(const LTextPtr& text, const TextureRenderData& renderData);

    //Updates windows content
    void update();

    //Deallocates internals
    void free();

    //Window dimensions
    int getWidth();
    int getHeight();

    SDL_Renderer* getRenderer() const;

    const LCameraPtr getMainCamera() const;

    //Window focii
    bool hasMouseFocus();
    bool hasKeyboardFocus();
    bool isMinimized();
    bool isShown();

    void setTitle(const std::string& newTitle);
};

