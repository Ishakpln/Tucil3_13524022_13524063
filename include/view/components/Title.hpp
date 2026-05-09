#pragma once
#include "raylib.h"
#include <string>

class Title
{
private:
    int xPos, yPos;
    std::string content;

public:
    Title(int xPos, int yPos, std::string content);
    void draw() const;
};