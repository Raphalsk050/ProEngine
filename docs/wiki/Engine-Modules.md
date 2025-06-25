# Engine Modules

The main engine structure resides in `ProEngine/Core`. Each module is responsible for a specific part of the system.

## Application
Manages the main application loop, the window, the layer stack and integrates the editor when enabled.

## Assert and Log
Assertion macros (`Assert.h`) and logging system (`Log/PELog.h`) built on spdlog.

## Camera
3D camera implementations (`Camera3D`, `Camera3DController`) with frustum culling and control modes.

## Debug
Instrumentation tools for profiling (`Debug/Instrumentor.h`).

## Editor
Layers and utilities for the runtime editor, based on ImGui, ImGuizmo and imnodes.

## Event and Input
Event system (keyboard, mouse, window) and input queries.

## Layer
Layer interface and `LayerStack` manager to organize application logic and overlays.

## Renderer
Rendering abstractions: buffers, shaders, materials, textures, 3D renderer control (`Renderer3D`) and render commands (`RenderCommand`).

## Scene
Entity and component system using entt. Contains `Scene`, `EntityHandle` and basic rendering components.

## Time and Timestep
Utilities for time measurement and delta time between frames.

## Window
Window implementation (`GenericGLFWWindow`) and graphics context (`GraphicsContext`).

## Platform
Platform specific modules. Currently OpenGL (`Platform/OpenGL`) is supported, with buffer, shader, framebuffer and texture implementations.

## ThirdParty
Third-party submodules used by the engine.

## Program
Example application using the engine. Defines demonstration layers (`SampleLayer`, `SceneLayer`) and the entry point (`Program.cpp`).