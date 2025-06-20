#include "MinimalScene.h"

namespace filament {

void RenderMinimalScene(uint32_t* buffer, uint32_t width, uint32_t height) {
    for (uint32_t y = 0; y < height; ++y) {
        for (uint32_t x = 0; x < width; ++x) {
            uint8_t r = uint8_t((float)x / width * 255.0f);
            uint8_t g = uint8_t((float)y / height * 255.0f);
            buffer[y * width + x] = (255u << 24) | (uint32_t(g) << 8) | uint32_t(r);
        }
    }
}

} // namespace filament
