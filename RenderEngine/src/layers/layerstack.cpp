#include "layers/layerstack.h"

wizm::layer_stack::~layer_stack()
{
    for (core_layer* layer : m_Layers) {
        layer->on_detach();
        delete layer;
    }
  
}


//----------------------------------------------------------------------

void wizm::layer_stack::push_layer(core_layer* layer)
{
    m_Layers.emplace_back(layer);
    layer->on_attach();
}

void wizm::layer_stack::pop_layer(core_layer* layer)
{
    auto it = std::find(m_Layers.begin(), m_Layers.end(), layer);
    if (it != m_Layers.end()) {
        (*it)->on_detach();
        m_Layers.erase(it);
    }
}
