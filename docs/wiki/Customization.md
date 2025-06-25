# Customization

This page explains how to extend the engine with new components and details the rendering flow.

## Adding new components
Components are defined in `ProEngine/Core/Scene/Components.h`. To create a new one:
1. Declare a struct deriving from `ComponentBase`.
2. Add any fields you need.
3. Include the header where the component will be used.

```cpp
struct HealthComponent : public ComponentBase {
    float value = 100.f;
    HealthComponent() : ComponentBase("Health") {}
};
```
To attach the component to an entity:
```cpp
auto player = scene->CreateEntity("Player");
player.AddComponent<HealthComponent>();
```

## Rendering with `RendererComponent`
When an entity has a `RendererComponent`, the rendering system creates the appropriate mesh and material. During the scene update the `SceneRenderer` iterates over all entities with this component and issues draw commands through `Renderer3D`.

## Overriding components
You can derive from an existing component or replace it with another. Simply create a new struct and use `AddComponent` with your custom type. To store extra rendering data, for example, create a new component that inherits from `RendererComponent`.

```cpp
struct MyRenderer : public RendererComponent {
    bool cast_shadows = true;
};
```

## Layers and logic organization
The application holds a `LayerStack`. Each layer receives events and is updated every frame. To add functionality, create a class derived from `Layer` and register it with `PushLayer()`. The editor is an overlay added with `PushOverlay()` and can be enabled or disabled at compile time.