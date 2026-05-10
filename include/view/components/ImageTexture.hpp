#pragma once
#include "raylib.h"
class ImageTexture
{
private:
    Rectangle sourceBounds;
    Rectangle destBounds;
    Vector2 origin;
    Texture2D arrow;
    const char *path;

public:
    ImageTexture(Rectangle sB, Rectangle dP, const char *path, Vector2 origin);
    ~ImageTexture();

    ImageTexture(const ImageTexture &) = delete;
    ImageTexture &operator=(const ImageTexture &) = delete;

    void draw();
    bool isClicked();
    void setDestBounds(Rectangle dB);
    void setSourceBounds(Rectangle sB);
    void setOrigin(Vector2 origin);
    void setImagePath(const char* newPath);
};