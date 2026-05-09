#pragma once
#include "raylib.h"
#include <string>

class Title
{
private:
    int xPos, yPos, tSize;
    std::string content;

public:
    Title(int xPos, int yPos, std::string content, int tS);
    void draw() const;
    void setPosition(int x, int y);
};