#include <iostream>
#ifndef POINT_HPP
#define POINT_HPP

struct Point {
    int x;
    int y;
};

enum class Dir {
    up,
    right,
    left,
    down
};

struct SlideResult {
    Point position;
    bool isGameOver;
    int cost;
};

#endif