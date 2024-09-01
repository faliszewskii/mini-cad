//
// Created by USER on 23/08/2024.
//

#ifndef BRESENHAMALGORITHM_H
#define BRESENHAMALGORITHM_H

#include <functional>

void bresenhamAlgorithm(int x1, int y1, int x2, int y2, const std::function<void(int, int, float)> &onPutPixelHandler);

#endif //BRESENHAMALGORITHM_H
