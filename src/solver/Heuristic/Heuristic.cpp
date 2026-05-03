#include "solver/Heuristic/Heuristic.hpp"

float heuristic(Point start, Point target, HeuristicType type, float avgCost, float minCost) {
    if (type == HeuristicType::EUCLIDEAN) {
        int dx = target.x - start.x;
        int dy = target.y - start.y;
        return sqrt(dx * dx + dy * dy) * minCost;
    } 
    else if (type == HeuristicType::ENHANCEDEUCLIDEAN) {
        int dx = target.x - start.x;
        int dy = target.y - start.y;
        return sqrt(dx * dx + dy * dy) * avgCost; 
    }
    else if (type == HeuristicType::MANHATTAN) {
        int dx = abs(target.x - start.x);
        int dy = abs(target.y - start.y); 
        return (dx + dy) * minCost;
    }
    else if (type == HeuristicType::ENHANCEDMANHATTAN) {
        int dx = abs(target.x - start.x);
        int dy = abs(target.y - start.y); 
        return (dx + dy) * avgCost;
    }

    return 0;
}
    