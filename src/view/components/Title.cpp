#include "view/components/Title.hpp"
#include "utils/GuiHelper.hpp"

Title::Title(int xPos, int yPos, std::string content, int tS):
    xPos(xPos), yPos(yPos), content(content), tSize(tS){}

void Title::draw() const
{
    DrawText(content.c_str(), this->xPos, this->yPos, this->tSize, Theme::Text);
}

void Title::setPosition(int x, int y)
{
    this->xPos = x;
    this->yPos = y;
}