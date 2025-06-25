# Getting Started

This guide shows how to build the engine and create a basic project using ProEngine.

## 1. Prepare dependencies
Run the `setup.sh` script to install system libraries (requires super user permission):
```bash
./setup.sh
```

## 2. Build the engine
Use the `build.sh` script to generate the CMake files and compile via Ninja:
```bash
./build.sh
```
The binaries will be generated inside the `build/` directory.

## 3. Creating a minimal project
A simple application example can be found in `Program/Program.cpp`. The basic structure is:
```cpp
#include <ProEngine.h>
using namespace ProEngine;

class SampleApp : public Application {
public:
    explicit SampleApp(const ApplicationSpecification& spec) : Application(spec) {
        PushLayer(new Layer("GameLayer"));
    }
};

Application* CreateApplication(ApplicationCommandLineArgs args) {
    ApplicationSpecification spec;
    spec.Name = "Sample";
    spec.CommandLineArgs = args;
    return new SampleApp(spec);
}
```
Compile this code together with the engine and run the resulting binary. The engine's default window will appear.

## 4. Enabling or disabling the editor
The runtime editor is controlled by the compilation flag `PROENGINE_ENABLE_EDITOR`. In `build.sh` it is enabled by default. To disable it simply change the option:
```bash
cmake -G Ninja -S . -B build -DPROENGINE_ENABLE_EDITOR=OFF
```
Then rebuild with `cmake --build build` or run `./build.sh` after editing the script.

## 5. Adding a camera to the scene
Inside a `Scene`, create an entity and add the `CameraComponent`:
```cpp
auto cameraEntity = scene->CreateEntity("Camera");
CameraComponent& cam = cameraEntity.AddComponent<CameraComponent>();
cam.primary = true; // set as primary camera
```
You can control the camera with the `Camera3DController`.

## 6. Layers
The application uses a stack of layers to organize logic. Use `PushLayer()` to add new layers. Overlays (such as the editor) are added with `PushOverlay()` and are rendered on top of the other layers.