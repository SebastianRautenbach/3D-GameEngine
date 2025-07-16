#pragma once
#include <string>


namespace wizm {

	class core_layer {

    public:
        core_layer(const std::string& name = "Layer") : m_DebugName(name) {}
        virtual ~core_layer() = default;

        virtual void on_attach() {} 
        virtual void on_detach() {} 
        virtual void update(float delta_time) = 0;
        virtual void on_imgui_render() {}

        std::string get_layer_name() { return m_DebugName; }

    protected:
        std::string m_DebugName;


	};

}