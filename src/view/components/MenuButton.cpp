#include "library/raygui.h"
#include "view/components/MenuButton.hpp"

MenuButton::MenuButton(Rectangle bounds, int iconId, const char* text, Color textColor, Color iconCol, int fSize):bounds(bounds), iconId(iconId), text(text), textColor(textColor), fSize(fSize) {}

bool MenuButton::draw()
{
    bool pressed = GuiButton(this->bounds, GuiIconText(this->iconId, this->text));
    return pressed;
}

void MenuButton::setText(const char* newText) { this->text = newText; }
void MenuButton::setBounds(Rectangle newBounds) {this->bounds = newBounds;}
void MenuButton::setEnabeled(bool newVal) {this->enabeled = newVal;}

