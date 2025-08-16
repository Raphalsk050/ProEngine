#pragma once

#include <functional>
#include <unordered_map>
#include <entt.hpp>
#include <imgui.h>

namespace ProEngine
{
    class EditorPropertiesFactory
    {
    public:
        using DrawerFn = std::function<void(void*)>;
        using GetterFn = std::function<void*(entt::registry&, entt::entity)>;

        // Registers the drawer for component T
        template <typename T>
        void registerDrawer(const std::string& displayName, DrawerFn drawer)
        {
            auto id = entt::type_hash<T>::value();

            // 1) _drawers
            _drawers.insert_or_assign(id, std::move(drawer));

            // 2) _getters
            _getters.insert_or_assign(
                id,
                [](entt::registry& r, entt::entity e) -> void*
                {
                    return &r.get<T>(e);
                }
            );

            // 3) _names
            _names.insert_or_assign(
                id,
                displayName
            );
        }

        // Draws all registered components for a single entity
        void drawFor(entt::registry& reg, entt::entity e) const
        {
            int ui_id = rand();
            for (auto const& [id, drawer] : _drawers)
            {

                // does this entity actually have T?
                if (reg.storage(id)->contains(e))
                {
                    // fetch the raw pointer
                    void* ptr = _getters.at(id)(reg, e);
                    if (ptr == nullptr) return;
                    // draw UI for it
                    ImGui::Separator();
                    ImGui::Text("%s", _names.at(id).c_str());
                    drawer(ptr);
                }
            }
        }

    private:
        std::unordered_map<entt::id_type, DrawerFn> _drawers;
        std::unordered_map<entt::id_type, GetterFn> _getters;
        std::unordered_map<entt::id_type, std::string> _names;
    };
} // namespace ProEngine
