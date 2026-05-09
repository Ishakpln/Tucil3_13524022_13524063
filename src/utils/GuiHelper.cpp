#include "utils/GuiHelper.hpp"
#include <string>

Color HexToColor(const char* hex) {
    unsigned int r, g, b;
    if (hex[0] == '#') hex++; // skip '#'

    sscanf(hex, "%02x%02x%02x", &r, &g, &b);
    return (Color){ (unsigned char)r, (unsigned char)g, (unsigned char)b, 255 };
}
