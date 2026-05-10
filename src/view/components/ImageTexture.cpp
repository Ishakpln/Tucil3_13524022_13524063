#include "view/components/ImageTexture.hpp"

ImageTexture::ImageTexture(Rectangle sB, Rectangle dB, const char *path, Vector2 origin) : sourceBounds(sB), destBounds(dB), path(path), origin(origin)
{
    this->arrow = LoadTexture(path);
};

ImageTexture::~ImageTexture() { UnloadTexture(this->arrow); }

void ImageTexture::draw()
{
    DrawTexturePro(this->arrow, this->sourceBounds, this->destBounds, this->origin, 0, WHITE);
}

bool ImageTexture::isClicked()
{
    Vector2 mouse = GetMousePosition();
    if (CheckCollisionPointRec(mouse, destBounds) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        return true;
    }
    return false;
}

void ImageTexture::setDestBounds(Rectangle dB) { this->destBounds = dB; }
void ImageTexture::setSourceBounds(Rectangle sB) { this->sourceBounds = sB; }
void ImageTexture::setOrigin(Vector2 origin) { this->origin = origin; }
void ImageTexture::setImagePath(const char* newPath) 
{
    UnloadTexture(this->arrow);
    this->path = newPath;
    this->arrow = LoadTexture(this->path);
}