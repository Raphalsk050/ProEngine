#include "Platform/Filament/TextureLoader.h"
#include <stb_image.h>
#include <backend/PixelBufferDescriptor.h>

namespace ProEngine {

filament::Texture* TextureLoader::LoadTexture(const std::string& path) {
    int w,h,c;
    stbi_uc* data = stbi_load(path.c_str(), &w, &h, &c, 4);
    if(!data) return nullptr;
    auto* tex = filament::Texture::Builder()
        .width(w)
        .height(h)
        .levels(1)
        .format(filament::Texture::InternalFormat::RGBA8)
        .build(*engine_);
    filament::backend::PixelBufferDescriptor pbd(
        data, (size_t)(w*h*4),
        filament::backend::PixelDataFormat::RGBA,
        filament::backend::PixelDataType::UBYTE);
    tex->setImage(*engine_, 0, std::move(pbd));
    stbi_image_free(data);
    return tex;
}

} // namespace ProEngine
