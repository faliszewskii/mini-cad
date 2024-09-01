//
// Created by USER on 23/08/2024.
//

#include "FloodFill.h"

#include <queue>

void floodFill(int x1, int y1, int xMin, int xMax, int yMin, int yMax, const std::function<void(int, int)> &onPutPixelHandler, const std::function<bool(int, int)> &onReadPixelHandler) {
    std::queue<std::pair<int, int>> queue;
    queue.push(std::make_pair(x1, y1));

    while(!queue.empty()) {
        int x = queue.front().first;
        int y = queue.front().second;
        queue.pop();
        if (x < xMin || y < yMin || x >= xMax || y >= yMax) continue;
        if(onReadPixelHandler(x, y)) {
            onPutPixelHandler(x, y);
            queue.push(std::make_pair(x+1, y));
            queue.push(std::make_pair(x-1, y));
            queue.push(std::make_pair(x, y+1));
            queue.push(std::make_pair(x, y-1));
        }
    }
}
