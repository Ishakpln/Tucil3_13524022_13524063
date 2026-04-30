#include <iostream>
#ifndef POINT_HPP
#define POINT_HPP

struct Point {
    int x;
    int y;
};

enum class Direction {
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

enum class HeuristicType {
    EUCLIDEAN,
    TYPE2,
    TYPE3 //isi nanti
};


#endif