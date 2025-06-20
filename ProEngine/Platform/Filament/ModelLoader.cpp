#include "Platform/Filament/ModelLoader.h"
#include <gltfio/MaterialProvider.h>
#include <gltfio/TextureProvider.h>
#include <fstream>
#include <vector>

namespace ProEngine {

ModelLoader::ModelLoader(filament::Engine* engine) : engine_(engine) {
    auto* provider = filament::gltfio::createJitShaderProvider(engine_);
    asset_loader_ = filament::gltfio::AssetLoader::create({engine_, provider});
    resource_loader_ = new filament::gltfio::ResourceLoader({engine_, "", true});
}

ModelLoader::~ModelLoader() {
    if (asset_loader_) filament::gltfio::AssetLoader::destroy(&asset_loader_);
    delete resource_loader_;
}

filament::gltfio::FilamentAsset* ModelLoader::LoadModel(const std::string& path) {
    std::ifstream file(path, std::ios::binary | std::ios::ate);
    if (!file) return nullptr;
    auto size = file.tellg();
    file.seekg(0, std::ios::beg);
    std::vector<uint8_t> buffer(size);
    file.read(reinterpret_cast<char*>(buffer.data()), size);
    file.close();
    auto* asset = asset_loader_->createAsset(buffer.data(), (uint32_t)buffer.size());
    if (!asset) return nullptr;
    resource_loader_->loadResources(asset);
    asset->releaseSourceData();
    return asset;
}

} // namespace ProEngine
