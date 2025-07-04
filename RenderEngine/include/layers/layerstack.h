#pragma once
#include "layer.h"
#include <vector>

namespace wizm {

    class layer_stack {
    public:
        layer_stack() = default;
        ~layer_stack();

        void push_layer(core_layer* layer);
        void pop_layer(core_layer* layer);

        std::vector<core_layer*>::iterator begin() { return m_Layers.begin(); }
        std::vector<core_layer*>::iterator end() { return m_Layers.end(); }

        std::vector<core_layer*>::const_iterator begin() const { return m_Layers.begin(); }
        std::vector<core_layer*>::const_iterator end() const { return m_Layers.end(); }

        std::vector<core_layer*>::const_reverse_iterator rbegin() const { return m_Layers.rbegin(); }
        std::vector<core_layer*>::const_reverse_iterator rend() const { return m_Layers.rend(); }

    public:
        std::vector<core_layer*> m_Layers;
    };

}

