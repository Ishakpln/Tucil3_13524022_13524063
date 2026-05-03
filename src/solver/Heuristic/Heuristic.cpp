#include "solver/Heuristic/Heuristic.hpp"

float heuristic(Point start, Point target, HeuristicType type) {
    if (type == HeuristicType::EUCLIDEAN) {
        int dx = target.x - start.x;
        int dy = target.y - start.y;
        return sqrt(dx * dx + dy * dy);
    } 
    else if (type == HeuristicType::MANHATTAN) {
        int dx = abs(target.x - start.x);
        int dy = abs(target.y - start.y); 
        return dx + dy;
    }

    return 0;
}
    