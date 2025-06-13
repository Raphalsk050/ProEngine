#pragma once

#include <vector>
#include "Config.h"
#include "Layer.h"

namespace ProEngine
{
    class LayerStack
    {
    public:
        LayerStack() = default;
        ~LayerStack();

        void PushLayer(Layer* layer);
        void PushOverlay(Layer* overlay);
        void PopLayer(Layer* layer);
        void PopOverlay(Layer* overlay);

        std::vector<Layer*>::iterator begin() { return layers_.begin(); }
        std::vector<Layer*>::iterator end() { return layers_.end(); }
        std::vector<Layer*>::reverse_iterator rbegin() { return layers_.rbegin(); }
        std::vector<Layer*>::reverse_iterator rend() { return layers_.rend(); }

        std::vector<Layer*>::const_iterator begin() const { return layers_.begin(); }
        std::vector<Layer*>::const_iterator end() const { return layers_.end(); }

        std::vector<Layer*>::const_reverse_iterator rbegin() const
        {
            return layers_.rbegin();
        }

        std::vector<Layer*>::const_reverse_iterator rend() const
        {
            return layers_.rend();
        }

    private:
        std::vector<Layer*> layers_;
        unsigned int layer_insert_index_ = 0;
    };
} // namespace BEngine
