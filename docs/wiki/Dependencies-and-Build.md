# ProEngine Wiki

This wiki describes the dependencies used by the project, the build process and provides a summary of the main engine modules.

## Dependencies

All code dependencies are located under `ProEngine/ThirdParty`. The engine relies on the following libraries:

- **GLFW** – window creation and OpenGL context management
- **Glad** – OpenGL loader
- **glm** – 3D math
- **spdlog** – logging system
- **imgui** – runtime user interface
- **ImGuizmo** – 3D gizmos for ImGui
- **imnodes** – node system for the editor
- **entt** – ECS library used for entity management
- **stbimage** – image loading
- **assimp** – 3D model import (pre‑compiled)
- **bullet** – physics engine
- **boost** and **tinyexr** – assorted utilities

The `setup.sh` script installs OpenGL development packages and other system dependencies via `apt`. Check the script for the complete list.

## Building

1. Run `setup.sh` to install the system libraries (requires sudo):
   ```bash
   ./setup.sh
   ```
2. To build the engine and the example use `build.sh`:
   ```bash
   ./build.sh
   ```
   The script creates the `build` directory, generates files with CMake (using Ninja) and compiles with `clang++`. The default build is `Debug` and enables the editor (`PROENGINE_ENABLE_EDITOR=ON`).
3. The resulting binaries will be placed under `build/`.

If you wish to configure manually, run the equivalent commands:
```bash
cmake -G Ninja -S . -B build -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_C_COMPILER=clang -DCMAKE_BUILD_TYPE=Debug -DPROENGINE_ENABLE_EDITOR=ON
cmake --build build
```

See the other wiki pages for details on modules and customization:
- [Engine-Modules](Engine-Modules.md)
- [Getting-Started](Getting-Started.md)
- [Customization](Customization.md)