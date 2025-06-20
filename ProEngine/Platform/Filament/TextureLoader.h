#pragma once
#include <string>
#include <filament/Engine.h>
#include <filament/Texture.h>

namespace ProEngine {

class TextureLoader {
public:
    explicit TextureLoader(filament::Engine* engine) : engine_(engine) {}
    filament::Texture* LoadTexture(const std::string& path);
private:
    filament::Engine* engine_ = nullptr;
};

} // namespace ProEngine
