#include "library/raygui.h"
#include "view/components/MenuButton.hpp"

MenuButton::MenuButton(Rectangle bounds, int iconId, const char* text, Color textColor, Color iconCol):bounds(bounds), iconId(iconId), text(text), textColor(textColor) {}

bool MenuButton::draw()
{
    bool pressed = GuiButton(this->bounds, this->text);
    if (this->iconId > 0) GuiDrawIcon(this->iconId, this->bounds.x + 4, this->bounds.y + 4, 4, this-> iconColor);
    return pressed;
}

void MenuButton::setText(const char* newText) { this->text = newText; }
void MenuButton::setBounds(Rectangle newBounds) {this->bounds = newBounds;}
void MenuButton::setEnabeled(bool newVal) {this->enabeled = newVal;}

