#include "solver/Heuristic/Heuristic.hpp"
#include "board/Board.hpp"

float heuristic(Point start, int index /*lagi ngejar apa sekarang*/, HeuristicType type, const Board& board) { 
    if (type == HeuristicType::EUCLIDEAN_CHECKPOINT) {
        float distance = 0;
        distance += getEuclideanDist(start, board.getCheckpointDistance(index).position, board.getMinCost());
        for (int i = index;i < board.getCheckpointCount()-1;i++) {
            distance += board.getCheckpointDistance(i).euclideanDist;
        }
        return distance;
    }
    else if (type == HeuristicType::MANHATTAN_CHECKPOINT) {     
        float distance = 0;
        distance += getManhattanDist(start, board.getCheckpointDistance(index).position, board.getMinCost());
        for (int i = index;i < board.getCheckpointCount()-1;i++) {
            distance += board.getCheckpointDistance(i).manhattanDist;
        }
        return distance;  
    }
    else if (type == HeuristicType::EUCLIDEAN_MIN) {
        return getEuclideanDist(start, board.getFinishPosition(), board.getMinCost());
    }
    else if (type == HeuristicType::MANHATTAN_MIN) {
        return getManhattanDist(start, board.getFinishPosition(), board.getMinCost());
    }
    
    return 0;
}
    
