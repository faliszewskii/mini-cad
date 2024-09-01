#include "BresenhamAlgorithm.h"

void drawLineLow(int x1, int y1, int x2, int y2, const std::function<void(int, int, float)> &onPutPixelHandler);
void drawLineHigh(int x1, int y1, int x2, int y2, const std::function<void(int, int, float)> &onPutPixelHandler);

void bresenhamAlgorithm(int x1, int y1, int x2, int y2, const std::function<void(int, int, float)> &onPutPixelHandler)
{
    auto dx = x2 - x1;
    auto dy = y2 - y1;
    auto D = 2.0f * dy - dx;

    auto y = y1;

    if (abs(dy) < abs(dx))
    {
        if (dx < 0)
        {
            drawLineLow(x2, y2, x1, y1, onPutPixelHandler);
        }
        else
        {
            drawLineLow(x1, y1, x2, y2, onPutPixelHandler);
        }
    }
    else
    {
        if (dy < 0)
        {
            drawLineHigh(x2, y2, x1, y1, onPutPixelHandler);
        }
        else
        {
            drawLineHigh(x1, y1, x2, y2, onPutPixelHandler);
        }
    }
}

void drawLineLow(int x1, int y1, int x2, int y2, const std::function<void(int, int, float)> &onPutPixelHandler)
{
    auto dx = x2 - x1, dy = y2 - y1;
    auto yi = 1.0f;

    if (dy < 0)
    {
        yi = -1;
        dy = -dy;
    }

    auto D = 2.0f * dy - dx;
    auto y = y1;

    for (int x = x1; x <= x2; x++)
    {
        float t = static_cast<float>(x - x1) / static_cast<float>(x2 - x1);

        onPutPixelHandler(x, y, t);

        if (D > 0.0f)
        {
            y += yi;
            D -= 2.0f * dx;
        }

        D += 2.0f * dy;
    }
}

void drawLineHigh(int x1, int y1, int x2, int y2, const std::function<void(int, int, float)> &onPutPixelHandler)
{
    auto dx = x2 - x1, dy = y2 - y1;
    auto xi = 1.0f;

    if (dx < 0)
    {
        xi = -1;
        dx = -dx;
    }

    auto D = 2.0f * dx - dy;
    auto x = x1;

    for (int y = y1; y <= y2; y++)
    {
        float t = static_cast<float>(y - y1) / static_cast<float>(y2 - y1);

        onPutPixelHandler(x, y, t);

        if (D > 0.0f)
        {
            x += xi;
            D -= 2.0f * dy;
        }

        D += 2.0f * dx;
    }
}