#include "view/assets/TextureResource.hpp"
#include <utility>

TextureResource::TextureResource(const std::string& texturePath) {
    load(texturePath);
}

TextureResource::~TextureResource() {
    unload();
}

TextureResource::TextureResource(TextureResource&& other) noexcept
    : texture(other.texture), loaded(other.loaded), path(std::move(other.path)) {
    other.texture = Texture2D{};
    other.loaded = false;
}

TextureResource& TextureResource::operator=(TextureResource&& other) noexcept {
    if (this != &other) {
        unload();
        texture = other.texture;
        loaded = other.loaded;
        path = std::move(other.path);
        other.texture = Texture2D{};
        other.loaded = false;
    }
    return *this;
}

bool TextureResource::load(const std::string& texturePath) {
    unload();
    path = texturePath;

    if (!FileExists(texturePath.c_str())) {
        TraceLog(LOG_WARNING, "Texture file not found: %s", texturePath.c_str());
        return false;
    }

    texture = LoadTexture(texturePath.c_str());
    loaded = texture.id != 0;

    if (!loaded) {
        TraceLog(LOG_WARNING, "Failed to load texture: %s", texturePath.c_str());
    }

    return loaded;
}

void TextureResource::unload() {
    if (loaded) {
        UnloadTexture(texture);
    }
    texture = Texture2D{};
    loaded = false;
}

bool TextureResource::isLoaded() const { return loaded; }
const Texture2D& TextureResource::get() const { return texture; }
int TextureResource::width() const { return loaded ? texture.width : 0; }
int TextureResource::height() const { return loaded ? texture.height : 0; }
const std::string& TextureResource::getPath() const { return path; }
