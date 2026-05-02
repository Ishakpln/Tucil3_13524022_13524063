#include <iostream>
#ifndef POINT_HPP
#define POINT_HPP

#include <vector>

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

struct Node {
    Point position;
    int targetIndex;    //target index number sekarag baut array target number (1,2,3,4,5,O) -> selalu di akhiri O
    int gCost;  //buat path finder yg g ada g cost atau h cost buat aja 0
    float hCost;
    float fCost;  //buat AStar
    int parentIndex;
    Direction move;
};

struct SlideResult {
    Point position;
    int targetIndex;
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
