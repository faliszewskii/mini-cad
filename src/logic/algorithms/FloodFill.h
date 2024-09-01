//
// Created by USER on 23/08/2024.
//

#ifndef FLOODFILL_H
#define FLOODFILL_H
#include <functional>

void floodFill(int x1, int y1, int xMin, int xMax, int yMin, int yMax, const std::function<void(int, int)> &onPutPixelHandler, const std::function<bool(int, int)> &onReadPixelHandler);


#endif //FLOODFILL_H
