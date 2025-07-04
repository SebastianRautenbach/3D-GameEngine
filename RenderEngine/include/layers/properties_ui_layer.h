#pragma once
#include "layer.h"
#include "gl renderer/gl_renderer.h"
#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_glfw.h"
#include "imgui/backends/imgui_impl_opengl3.h"
#include "entity sys/component.h"

using namespace lowlevelsys;

namespace wizm {

	class asset_manager;

	class properties_ui_layer : public core_layer {

	public:
		properties_ui_layer( gl_renderer* renderer, asset_manager* manager);
		~properties_ui_layer();

		virtual void on_attach() override;
		virtual void on_detach() override;
		virtual void update(float delta_time) override;

		// properties panel specific
		
		void component_add_popup();
		void modify_component_attrib(std::string& type, core_component* component);

	private:
		gl_renderer* m_renderer;
		asset_manager* m_asset_manager;
	};


}