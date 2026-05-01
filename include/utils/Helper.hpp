#include <iostream>
#ifndef POINT_HPP
#define POINT_HPP

struct Point {
    int x;
    int y;

    bool operator==(const Point& other) const {
        return x == other.x && y == other.y;
    }

    bool operator!=(const Point& other) const {
        return !(*this == other);
    }
};

enum class Direction {
    up,
    right,
    left,
    down
};

struct SlideResult {
    Node position;
    bool isGameOver; //game sudha berakhir tapi tidak valid (misal jatuh ke lava, etc)
    bool isFinished; //game sudah berakhir secara valid
    int cost;
};

enum class HeuristicType {
    EUCLIDEAN,
    TYPE2,
    TYPE3 //isi nanti
};


#endif
