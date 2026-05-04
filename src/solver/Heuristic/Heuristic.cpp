#include "solver/Heuristic/Heuristic.hpp"
#include "board/Board.hpp"

float heuristic(Point start, Point target, HeuristicType type, const Board& board) {
    if (type == HeuristicType::EUCLIDEAN_MIN) {
        int dx = target.x - start.x;
        int dy = target.y - start.y;
        return sqrt(dx * dx + dy * dy) * board.getMinCost();
    } 
    else if (type == HeuristicType::EUCLIDEAN_AVG) {
        int dx = target.x - start.x;
        int dy = target.y - start.y;
        return sqrt(dx * dx + dy * dy) * board.getAvgCost(); 
    }
    else if (type == HeuristicType::MANHATTAN_MIN) {
        int dx = abs(target.x - start.x);
        int dy = abs(target.y - start.y); 
        return (dx + dy) * board.getMinCost();
    }
    else if (type == HeuristicType::MANHATTAN_AVG) {
        int dx = abs(target.x - start.x);
        int dy = abs(target.y - start.y); 
        return (dx + dy) * board.getAvgCost();
    }

    return 0;
}
    
