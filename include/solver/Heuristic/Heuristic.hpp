#ifndef HEURISTIC_HPP
#define HEURISTIC_HPP

#include <string>
#include "utils/Helper.hpp"
#include <math.h>

class Board;

float heuristic(Point start, Point target, HeuristicType type, const Board& board);

#endif
