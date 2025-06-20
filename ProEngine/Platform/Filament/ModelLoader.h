#pragma once
#include <string>
#include <filament/Engine.h>
#include <gltfio/AssetLoader.h>
#include <gltfio/ResourceLoader.h>
#include <gltfio/FilamentAsset.h>

namespace ProEngine {

class ModelLoader {
public:
    explicit ModelLoader(filament::Engine* engine);
    ~ModelLoader();

    filament::gltfio::FilamentAsset* LoadModel(const std::string& path);

private:
    filament::Engine* engine_ = nullptr;
    filament::gltfio::AssetLoader* asset_loader_ = nullptr;
    filament::gltfio::ResourceLoader* resource_loader_ = nullptr;
};

} // namespace ProEngine
