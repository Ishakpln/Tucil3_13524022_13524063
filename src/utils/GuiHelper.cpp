#include "utils/GuiHelper.hpp"
#include "library/raygui.h"
#include <string>

Color HexToColor(const char* hex) {
    unsigned int r, g, b;
    if (hex[0] == '#') hex++; // skip '#'

    sscanf(hex, "%02x%02x%02x", &r, &g, &b);
    return (Color){ (unsigned char)r, (unsigned char)g, (unsigned char)b, 255 };
}

static int colorToInt(Color c) {
    return (c.r << 24) | (c.g << 16) | (c.b << 8) | c.a;
}

void ApplyGuiTheme() {
    GuiSetStyle(DEFAULT, BACKGROUND_COLOR, colorToInt(Theme::Background));

    GuiSetStyle(DEFAULT, TEXT_COLOR_NORMAL, colorToInt(Theme::Text));
    GuiSetStyle(DEFAULT, TEXT_COLOR_FOCUSED, colorToInt(Theme::Text));
    GuiSetStyle(DEFAULT, TEXT_COLOR_PRESSED, colorToInt(Theme::Text));

    GuiSetStyle(DEFAULT, BORDER_COLOR_NORMAL, colorToInt(Theme::Border));
    GuiSetStyle(DEFAULT, BORDER_COLOR_FOCUSED, colorToInt(Theme::AccentDark));
    GuiSetStyle(DEFAULT, BORDER_COLOR_PRESSED, colorToInt(Theme::AccentDark));

    GuiSetStyle(DEFAULT, BASE_COLOR_NORMAL, colorToInt(Theme::Accent));
    GuiSetStyle(DEFAULT, BASE_COLOR_FOCUSED, colorToInt(Theme::Surface));
    GuiSetStyle(DEFAULT, BASE_COLOR_PRESSED, colorToInt(Theme::AccentDark));

    GuiSetStyle(BUTTON, BORDER_WIDTH, 2);
    GuiSetStyle(DEFAULT, TEXT_SIZE, 20);
}
