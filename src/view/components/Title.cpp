#include "view/components/Title.hpp"

Title::Title(int xPos, int yPos, std::string content):
    xPos(xPos), yPos(yPos), content(content){}

void Title::draw() const
{
    DrawText(content.c_str(), this->xPos, this->yPos, 20, BLACK);
}