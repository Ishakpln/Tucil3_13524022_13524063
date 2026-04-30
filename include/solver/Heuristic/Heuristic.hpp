#include <string>
#include "utils/Helper.hpp"
#include <math.h>

float heuristic(Point start, Point target, HeuristicType type) {
    if (type == HeuristicType::EUCLIDEAN) {
        int dx = target.x - start.x;
        int dy = target.y - start.y;
        return sqrt(dx * dx + dy * dy);
    }
    
}
