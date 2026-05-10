#include "utils/Helper.hpp"
#include <math.h>

float getEuclideanDist(Point start, Point target, int minCost) {
    int dx = target.x - start.x;
    int dy = target.y - start.y;
    return sqrt(dx * dx + dy * dy) * minCost;
}

float getManhattanDist(Point start, Point target, int minCost) {
    int dx = abs(target.x - start.x);
    int dy = abs(target.y - start.y);
    return (dx + dy) * minCost;
}
