#include "LWindow.h"
#include "LCamera.h"
#include "LTexture.h"
#include "LButton.h"
#include "LText.h"
#include <cmath>

LWindow::LWindow(const std::string& title, unsigned int screen_width, unsigned int screen_height)
{
    //Initialize non-existant window
    mTitle = title;
    mWindow = nullptr;
    mRenderer = nullptr;
    mWindowDisplayID = 0;
    mWindowID = 0;
    mainCamera = nullptr;
    cameras = std::vector<LCameraPtr>();
    mMouseFocus = false;
    mKeyboardFocus = false;
    mFullScreen = false;
    mMinimized = false;
    mWidth = screen_width;
    mHeight = screen_height;
    mShown = false;
}

bool LWindow::init(bool isResizable)
{
    //Create window
    if (isResizable)
        mWindow = SDL_CreateWindow(mTitle.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, mWidth, mHeight, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    else
        mWindow = SDL_CreateWindow(mTitle.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, mWidth, mHeight, SDL_WINDOW_SHOWN);

    if (mWindow != nullptr)
    {
        mMouseFocus = true;
        mKeyboardFocus = true;

        //Create renderer for window
        mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
        if (mRenderer == nullptr)
        {
            printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
            SDL_DestroyWindow(mWindow);
            mWindow = nullptr;
        }
        else
        {
            //Initialize renderer color
            SDL_SetRenderDrawColor(mRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

            //Grab window identifiers
            mWindowID = SDL_GetWindowID(mWindow);
            mWindowDisplayID = SDL_GetWindowDisplayIndex(mWindow);

            //Flag as opened
            mShown = true;

            mainCamera = std::make_shared<LCamera>();
            cameras.push_back(mainCamera);
        }
    }
    else
    {
        printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
    }

    return mWindow != NULL && mRenderer != NULL;
}

void LWindow::handleEvent(SDL_Event& e)
{
    //If an event was detected for this window
    if (e.type == SDL_WINDOWEVENT && e.window.windowID == mWindowID)
    {
        switch (e.window.event)
        {
            //Window moved
        case SDL_WINDOWEVENT_MOVED:
            mWindowDisplayID = SDL_GetWindowDisplayIndex(mWindow);
            break;

            //Window appeared
        case SDL_WINDOWEVENT_SHOWN:
            mShown = true;
            break;

            //Window disappeared
        case SDL_WINDOWEVENT_HIDDEN:
            mShown = false;
            break;

            //Get new dimensions and repaint
        case SDL_WINDOWEVENT_SIZE_CHANGED:
            mWidth = e.window.data1;
            mHeight = e.window.data2;
            update();
            break;

            //Repaint on expose
        case SDL_WINDOWEVENT_EXPOSED:
            update();
            break;

            //Mouse enter
        case SDL_WINDOWEVENT_ENTER:
            mMouseFocus = true;
            break;

            //Mouse exit
        case SDL_WINDOWEVENT_LEAVE:
            mMouseFocus = false;
            break;

            //Keyboard focus gained
        case SDL_WINDOWEVENT_FOCUS_GAINED:
            mKeyboardFocus = true;
            break;

            //Keyboard focus lost
        case SDL_WINDOWEVENT_FOCUS_LOST:
            mKeyboardFocus = false;
            break;

            //Window minimized
        case SDL_WINDOWEVENT_MINIMIZED:
            mMinimized = true;
            break;

            //Window maxized
        case SDL_WINDOWEVENT_MAXIMIZED:
            mMinimized = false;
            break;

            //Window restored
        case SDL_WINDOWEVENT_RESTORED:
            mMinimized = false;
            break;

            //Hide on close
        case SDL_WINDOWEVENT_CLOSE:
            SDL_HideWindow(mWindow);
            break;
        }
    }
}

void LWindow::focus()
{
    //Restore window if needed
    if (!mShown)
    {
        SDL_ShowWindow(mWindow);
    }

    //Move window forward
    SDL_RaiseWindow(mWindow);
}

void LWindow::clear()
{
    if (!mMinimized)
    {
        //Clear screen
        SDL_SetRenderDrawColor(mRenderer, 0x00, 0x00, 0x00, 0xFF);
        SDL_RenderClear(mRenderer);
    }
}

Point2D LWindow::screenPointTranslation(const Point2D& point) {
    Point2D translatedPoint;
    translatedPoint.x = point.x;
    translatedPoint.y = getHeight() - point.y;
    return translatedPoint;
}

void LWindow::DrawPixel(const Point2D& pos, const SDL_Color& color)
{
    if (!mMinimized) {
        Point2D t_pos = screenPointTranslation(pos);
        SDL_SetRenderDrawColor(mRenderer, color.r, color.g, color.b, color.a);
        SDL_RenderDrawPoint(mRenderer, t_pos.x, t_pos.y);
    }
}

void LWindow::DrawPixels(const std::vector<Point2D>& points, const SDL_Color& color)
{
    if (!mMinimized) {
        SDL_SetRenderDrawColor(mRenderer, color.r, color.g, color.b, color.a);

        SDL_Point* pointsToDraw = new SDL_Point[points.size()];
        for (unsigned int i = 0; i < points.size(); i++) {
            pointsToDraw[i].x = screenPointTranslation(points[i]).x;
            pointsToDraw[i].y = screenPointTranslation(points[i]).y;
        }
        SDL_RenderDrawPoints(mRenderer, pointsToDraw, points.size());
        delete[] pointsToDraw;
    }
}

void LWindow::DrawLine(const Point2D& begin, const Point2D& end, const SDL_Color& color)
{
    if (!mMinimized) {
        Point2D t_begin = screenPointTranslation(begin);
        Point2D t_end = screenPointTranslation(end);
        SDL_SetRenderDrawColor(mRenderer, color.r, color.g, color.b, color.a);
        SDL_RenderDrawLine(mRenderer, t_begin.x, t_begin.y, t_end.x, t_end.y);
    }
}

void LWindow::DrawMyLine(const Point2D& begin, const Point2D& end, const SDL_Color& color)
{
    if (!mMinimized) {
        /*float dy = end.y - begin.y;
        float dx = end.x - begin.x;
        float px = abs(dx / (dy + 1.0f));

        float sY = dy != 0 ? dy / abs(dy) : 0.0f;
        float sX = dx != 0 ? dx / abs(dx) : 0.0f;

        for (float y = 0.0f; y <= abs(dy); y += 1.0f) {
            for (float x = px * y; x < px * (y + 1.0f); x += 1.0f) {
                DrawPixel({ begin.x + x * sX, begin.y + y * sY }, color);
            }
        }*/

        /*short dx = end.x - begin.x;
        short dy = end.y - begin.y;
        short t = 0;
        short sdx = dx < 0 ? -1 : 1;
        short sdy = dy < 0 ? -1 : 1;
        short adx = dx * sdx;
        short ady = dy * sdy;

        short gD = (adx >= ady ? adx : ady);
        short lD = (adx >= ady ? ady : adx);

        short parts = lD + 1;

        int x = begin.x;
        int y = begin.y;
        for (int i = 0; i <= gD; ++i)
        {
            DrawPixel({ x , y }, color);
            t += lD;
            if (t >= (gD / parts))
            {
                (adx >= ady ? y += sdy : x += sdx);
                t -= gD;
            }
            (adx >= ady ? x += sdx : y += sdy);            
        }*/

        int x, y, dx, dy, adx, ady, p, e, i;
        dx = end.x - begin.x; dy = end.y - begin.y;
        adx = abs(dx); ady = abs(dy);
        if (ady <= adx)
        {
            p = 2 * ady - adx;
            if (dx >= 0)
            {
                x = begin.x; y = begin.y; e = end.x;
            }
            else
            {
                x = end.x; y = end.y; e = begin.x;
            }

            DrawPixel({ (float)x, (float)y }, color);

            for (i = 0; x < e; i++)
            {
                x = x + 1;
                if (p < 0)
                    p = p + 2 * ady;
                else
                {
                    if ((dx < 0 && dy < 0) || (dx > 0 && dy > 0)) y = y + 1; else y = y - 1;
                    p = p + 2 * (ady - adx);
                }
                DrawPixel({ (float)x, (float)y }, color);
            }
        }
        else
        {
            p = 2 * adx - ady;
            if (dy >= 0)
            {
                x = begin.x; y = begin.y; e = end.y;
            }
            else
            {
                x = end.x; y = end.y; e = begin.y;
            }

            DrawPixel({ (float)x, (float)y }, color);

            for (i = 0; y < e; i++)
            {
                y = y + 1;
                if (p <= 0)
                    p = p + 2 * adx;
                else
                {
                    if ((dx < 0 && dy < 0) || (dx > 0 && dy > 0)) x = x + 1; else x = x - 1;
                    p = p + 2 * (adx - ady);
                }
                DrawPixel({ (float)x, (float)y }, color);
            }
        }
    }
}

void LWindow::DrawLines(const std::vector<Point2D>& points, const SDL_Color& color)
{
    if (!mMinimized) {
        SDL_SetRenderDrawColor(mRenderer, color.r, color.g, color.b, color.a);

        SDL_Point* pointsToDraw = new SDL_Point[points.size()];
        for (unsigned int i = 0; i < points.size(); i++) {
            pointsToDraw[i].x = screenPointTranslation(points[i]).x;
            pointsToDraw[i].y = screenPointTranslation(points[i]).y;
        }
        SDL_RenderDrawLines(mRenderer, pointsToDraw, points.size());
        delete[] pointsToDraw;
    }
}

void LWindow::DrawTriangle(const Point2D& point1, const Point2D& point2, const Point2D& point3, const SDL_Color& color)
{
    if (!mMinimized) {
        DrawLines({ point1, point2, point3 }, color);
        DrawLine(point3, point1, color);
    }
}

void LWindow::DrawFilledTriangle(const Point2D& point1, const Point2D& point2, const Point2D& point3, const SDL_Color& color)
{
    if (!mMinimized) {

        auto SWAP = [](auto& x, auto& y) { auto t = x; x = y; y = t; };
        auto drawline = [&](int sx, int ex, int ny) { for (int i = sx; i <= ex; i++) DrawPixel({ (float)i, (float)ny }, color); };

        int t1x, t2x, y, minx, maxx, t1xp, t2xp;
        bool changed1 = false;
        bool changed2 = false;
        int signx1, signx2, dx1, dy1, dx2, dy2;
        int e1, e2;

        Point2D highest = point3;
        Point2D middle = point2;
        Point2D lowest = point1;

        // Sort vertices
        if (lowest.y > middle.y) { SWAP(lowest, middle); }
        if (lowest.y > highest.y) { SWAP(lowest, highest); }
        if (middle.y > highest.y) { SWAP(middle, highest); }

        t1x = t2x = lowest.x; y = lowest.y;   // Starting points
        dx1 = (int)(middle.x - lowest.x); if (dx1 < 0) { dx1 = -dx1; signx1 = -1; }
        else signx1 = 1;
        dy1 = (int)(middle.y - lowest.y);

        dx2 = (int)(highest.x - lowest.x); if (dx2 < 0) { dx2 = -dx2; signx2 = -1; }
        else signx2 = 1;
        dy2 = (int)(highest.y - lowest.y);

        if (dy1 > dx1) {   // swap values
            SWAP(dx1, dy1);
            changed1 = true;
        }
        if (dy2 > dx2) {   // swap values
            SWAP(dy2, dx2);
            changed2 = true;
        }

        e2 = (int)(dx2 >> 1);
        // Flat top, just process the second half
        if (lowest.y == middle.y) goto next;
        e1 = (int)(dx1 >> 1);

        for (int i = 0; i < dx1;) {
            t1xp = 0; t2xp = 0;
            if (t1x < t2x) { minx = t1x; maxx = t2x; }
            else { minx = t2x; maxx = t1x; }
            // process first line until y value is about to change
            while (i < dx1) {
                i++;
                e1 += dy1;
                while (e1 >= dx1) {
                    e1 -= dx1;
                    if (changed1) t1xp = signx1;//t1x += signx1;
                    else          goto next1;
                }
                if (changed1) break;
                else t1x += signx1;
            }
            // Move line
        next1:
            // process second line until y value is about to change
            while (1) {
                e2 += dy2;
                while (e2 >= dx2) {
                    e2 -= dx2;
                    if (changed2) t2xp = signx2;//t2x += signx2;
                    else          goto next2;
                }
                if (changed2)     break;
                else              t2x += signx2;
            }
        next2:
            if (minx > t1x) minx = t1x; if (minx > t2x) minx = t2x;
            if (maxx < t1x) maxx = t1x; if (maxx < t2x) maxx = t2x;
            drawline(minx, maxx, y);    // Draw line from min to max points found on the y
                                         // Now increase y
            if (!changed1) t1x += signx1;
            t1x += t1xp;
            if (!changed2) t2x += signx2;
            t2x += t2xp;
            y += 1;
            if (y == middle.y) break;

        }
    next:
        // Second half
        dx1 = (int)(highest.x - middle.x); if (dx1 < 0) { dx1 = -dx1; signx1 = -1; }
        else signx1 = 1;
        dy1 = (int)(highest.y - middle.y);
        t1x = middle.x;

        if (dy1 > dx1) {   // swap values
            SWAP(dy1, dx1);
            changed1 = true;
        }
        else changed1 = false;

        e1 = (int)(dx1 >> 1);

        for (int i = 0; i <= dx1; i++) {
            t1xp = 0; t2xp = 0;
            if (t1x < t2x) { minx = t1x; maxx = t2x; }
            else { minx = t2x; maxx = t1x; }
            // process first line until y value is about to change
            while (i < dx1) {
                e1 += dy1;
                while (e1 >= dx1) {
                    e1 -= dx1;
                    if (changed1) { t1xp = signx1; break; }//t1x += signx1;
                    else          goto next3;
                }
                if (changed1) break;
                else   	   	  t1x += signx1;
                if (i < dx1) i++;
            }
        next3:
            // process second line until y value is about to change
            while (t2x != highest.x) {
                e2 += dy2;
                while (e2 >= dx2) {
                    e2 -= dx2;
                    if (changed2) t2xp = signx2;
                    else          goto next4;
                }
                if (changed2)     break;
                else              t2x += signx2;
            }
        next4:

            if (minx > t1x) minx = t1x; if (minx > t2x) minx = t2x;
            if (maxx < t1x) maxx = t1x; if (maxx < t2x) maxx = t2x;
            drawline(minx, maxx, y);
            if (!changed1) t1x += signx1;
            t1x += t1xp;
            if (!changed2) t2x += signx2;
            t2x += t2xp;
            y += 1;
            if (y > highest.y) return;
        }

        /*auto Swap = [](auto& a, auto& b) -> void {
            auto temp = a;
            a = b;
            b = temp;
        };

        SDL_Point highest = point1;
        SDL_Point middle = point2;
        SDL_Point lowest = point3;

        if (lowest.y > middle.y)
            Swap(lowest, middle);
        if (lowest.y > highest.y)
            Swap(lowest, highest);
        if (middle.y > highest.y)
            Swap(middle, highest);

        auto lineWalk = [&](int& x, int& y, short& t, short lD, short gD, short parts, short adx, short ady, short sdx, short sdy) -> bool {
            bool lineDrawed = false;
            DrawPixel({ x , y }, color);
            t += lD;
            if (t >= (gD / parts))
            {
                (adx >= ady ? y += sdy : x += sdx);
                t -= gD;

                (adx >= ady ? lineDrawed = false : lineDrawed = lineDrawed);
            }
            (adx >= ady ? x += sdx : y += sdy);

            (adx >= ady ? lineDrawed = lineDrawed : lineDrawed = false);

            return lineDrawed;
        };

        // First Line (highest -> middle)
        short dx1 = middle.x - highest.x;
        short dy1 = middle.y - highest.y;
        short t1 = 0;
        short sdx1 = dx1 < 0 ? -1 : 1;
        short sdy1 = dy1 < 0 ? -1 : 1;
        short adx1 = dx1 * sdx1;
        short ady1 = dy1 * sdy1;

        short gD1 = adx1;
        short lD1 = ady1;
        if (lD1 >= gD1)
            Swap(gD1, lD1);

        short parts1 = lD1 + 1;

        int x1 = highest.x;
        int y1 = highest.y;

        // Second Line (highest -> lowest)
        short dx2 = lowest.x - highest.x;
        short dy2 = lowest.y - highest.y;
        short t2 = 0;
        short sdx2 = dx2 < 0 ? -1 : 1;
        short sdy2 = dy2 < 0 ? -1 : 1;
        short adx2 = dx2 * sdx2;
        short ady2 = dy2 * sdy2;

        short gD2 = adx2;
        short lD2 = ady2;
        if (lD2 >= gD2)
            Swap(gD2, lD2);

        short parts2 = lD2 + 1;

        int x2 = highest.x;
        int y2 = highest.y;

        int i = 0;
        bool lineDrawed = false;
        while (i < gD1) {

            if (y1 == y2) {
                DrawLine({ x1 , y1 }, { x2 , y2 }, color);
                lineDrawed = true;
            }

            if (lineDrawed) {
                lineDrawed = lineWalk(x1, y1, t1, lD1, gD1, parts1, adx1, ady1, sdx1, sdy1);

                ++i;
            }

            if (y1 != y2) {
                lineWalk(x2, y2, t2, lD2, gD2, parts2, adx2, ady2, sdx2, sdy2);
            }

            //update();
            //SDL_Delay(100);
        }

        // Third Line (middle -> lowest)
        short dx3 = lowest.x - middle.x;
        short dy3 = lowest.y - middle.y;
        short t3 = 0;
        short sdx3 = dx3 < 0 ? -1 : 1;
        short sdy3 = dy3 < 0 ? -1 : 1;
        short adx3 = dx3 * sdx3;
        short ady3 = dy3 * sdy3;

        short gD3 = adx3;
        short lD3 = ady3;
        if (lD3 >= gD3)
            Swap(gD3, lD3);

        short parts3 = lD3 + 1;

        int x3 = middle.x;
        int y3 = middle.y;

        i = 0;
        lineDrawed = false;

        while (i <= gD3) {

            if (y3 == y2) {
                DrawLine({ x3 , y3 }, { x2 , y2 }, color);
                lineDrawed = true;
            }

            if (lineDrawed) {
                lineDrawed = lineWalk(x3, y3, t3, lD3, gD3, parts3, adx3, ady3, sdx3, sdy3);
                ++i;
            }

            if (y3 != y2) {
                lineWalk(x2, y2, t2, lD2, gD2, parts2, adx2, ady2, sdx2, sdy2);
            }

            //update();
            //SDL_Delay(100);
        }*/
    }
}

void LWindow::DrawCircle(const Point2D& center, int radius, const SDL_Color& color)
{
    if (!mMinimized) {
        int x = 0;
        int y = radius;
        int p = 3 - 2 * radius;
        if (!radius) return;

        while (y >= x) // only formulate 1/8 of circle
        {
            DrawPixel({ center.x - x, center.y - y }, color);
            DrawPixel({ center.x - y, center.y - x }, color);
            DrawPixel({ center.x + y, center.y - x }, color);
            DrawPixel({ center.x + x, center.y - y }, color);
            DrawPixel({ center.x - x, center.y + y }, color);
            DrawPixel({ center.x - y, center.y + x }, color);
            DrawPixel({ center.x + y, center.y + x }, color);
            DrawPixel({ center.x + x, center.y + y }, color);
            if (p < 0) p += 4 * x++ + 6;
            else p += 4 * (x++ - y--) + 10;
        }
    }
}

void LWindow::DrawFilledCircle(const Point2D& center, int radius, const SDL_Color& color)
{
    if (!mMinimized) {
        // Taken from wikipedia
        int x = 0;
        int y = radius;
        int p = 3 - 2 * radius;
        if (!radius) return;

        while (y >= x)
        {
            // Modified to draw scan-lines instead of edges
            DrawLine({ center.x - x , center.y - y }, { center.x + x , center.y - y }, color);
            DrawLine({ center.x - y , center.y - x }, { center.x + y , center.y - x }, color);
            DrawLine({ center.x - x , center.y + y }, { center.x + x , center.y + y }, color);
            DrawLine({ center.x - y , center.y + x }, { center.x + y , center.y + x }, color);
            if (p < 0) p += 4 * x++ + 6;
            else p += 4 * (x++ - y--) + 10;
        }
    }
}

void LWindow::DrawTexture(const LTexturePtr& texture, const TextureRenderData& renderData)
{
    if (!mMinimized)
    {
        //Set rendering space and render to screen
        SDL_Rect renderQuad = { renderData.pos.x, renderData.pos.y, texture->getWidth(), texture->getHeight()};

        //Set clip rendering dimensions
        if (renderData.clip != nullptr)
        {
            renderQuad.w = renderData.clip->w;
            renderQuad.h = renderData.clip->h;
        }

        //Render to screen
        SDL_RenderCopyEx(mRenderer, texture->getSDLTexture(), renderData.clip, &renderQuad, renderData.angle, renderData.center, renderData.flip);
    }
}

void LWindow::DrawButton(const LButtonPtr& button, const TextureRenderData& renderData)
{
    if (!mMinimized)
    {
        //Set rendering space and render to screen
        int x = button->getPosition()->x;
        int y = button->getPosition()->y;
        if (button->getParent() != nullptr) {
            x += button->getParent()->x;
            y += button->getParent()->y;
        }
        SDL_Rect renderQuad = { x, y, button->getTexture()->getWidth(), button->getTexture()->getHeight()};

        //Set clip rendering dimensions
        if (renderData.clip != nullptr)
        {
            renderQuad.w = renderData.clip->w;
            renderQuad.h = renderData.clip->h;
        }

        //Render to screen
        SDL_RenderCopyEx(mRenderer, button->getTexture()->getSDLTexture(), renderData.clip, &renderQuad, renderData.angle, renderData.center, renderData.flip);
    }
}

void LWindow::DrawText(const LTextPtr& text, const TextureRenderData& renderData) {
    if (!mMinimized)
    {
        int x = text->getPosition()->x;
        int y = text->getPosition()->y;
        if (text->getParent() != nullptr) {
            x += text->getParent()->x;
            y += text->getParent()->y;
        }
        //Set rendering space and render to screen
        SDL_Rect renderQuad = { x, y, text->getTexture()->getWidth(), text->getTexture()->getHeight() };

        //Set clip rendering dimensions
        if (renderData.clip != nullptr)
        {
            renderQuad.w = renderData.clip->w;
            renderQuad.h = renderData.clip->h;
        }

        //Render to screen
        SDL_RenderCopyEx(mRenderer, text->getTexture()->getSDLTexture(), renderData.clip, &renderQuad, renderData.angle, renderData.center, renderData.flip);
    }
}

void LWindow::update()
{
    if (!mMinimized)
    {
        //Update screen
        SDL_RenderPresent(mRenderer);
    }
}

void LWindow::free() {
    SDL_DestroyRenderer(mRenderer);
    SDL_DestroyWindow(mWindow);
}

int LWindow::getWidth()
{
    return mWidth;
}

int LWindow::getHeight()
{
    return mHeight;
}

SDL_Renderer* LWindow::getRenderer() const
{
    return mRenderer;
}

const LCameraPtr LWindow::getMainCamera() const
{
    return mainCamera;
}

bool LWindow::hasMouseFocus()
{
    return mMouseFocus;
}

bool LWindow::hasKeyboardFocus()
{
    return mKeyboardFocus;
}

bool LWindow::isMinimized()
{
    return mMinimized;
}

bool LWindow::isShown()
{
    return mShown;
}

void LWindow::setTitle(const std::string& newTitle)
{
    SDL_SetWindowTitle(mWindow, newTitle.c_str());
}