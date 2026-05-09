#pragma once
#include "raylib.h"

class MenuButton
{
private:
    Rectangle bounds;
    int iconId;
    const char* text;
    bool enabeled = true;
    Color textColor, iconColor;

public:
    MenuButton(Rectangle bound, int iconId, const char* text, Color textColor, Color iconColor);
    bool draw();
    void setText(const char* newText);
    void setBounds(Rectangle newBounds);
    void setEnabeled(bool newVal);
};