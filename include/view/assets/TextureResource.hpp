#pragma once

#include "raylib.h"
#include <string>

// Move-only RAII wrapper so every loaded raylib texture is unloaded once.
class TextureResource {
private:
    Texture2D texture{};
    bool loaded = false;
    std::string path;

public:
    TextureResource() = default;
    explicit TextureResource(const std::string& texturePath);
    ~TextureResource();

    TextureResource(const TextureResource&) = delete;
    TextureResource& operator=(const TextureResource&) = delete;

    TextureResource(TextureResource&& other) noexcept;
    TextureResource& operator=(TextureResource&& other) noexcept;

    bool load(const std::string& texturePath);
    void unload();

    bool isLoaded() const;
    const Texture2D& get() const;
    int width() const;
    int height() const;
    const std::string& getPath() const;
};
