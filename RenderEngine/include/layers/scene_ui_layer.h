#pragma once
#include "layer.h"
#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_glfw.h"
#include "imgui/backends/imgui_impl_opengl3.h"
#include "gl renderer/gl_renderer.h"

using namespace lowlevelsys;

namespace wizm {


	class scene_ui_layer : public core_layer {
	public:
		scene_ui_layer( gl_renderer* renderer, wizm::core_scene* scene);
		~scene_ui_layer();

		virtual void on_attach() override;
		virtual void on_detach() override;
		virtual void update(float delta_time) override;

		void render_entity_node(core_entity* entity);
		void render_modify_popup();

		void open_create_ent_menu();

	private:
		wizm::core_scene* global_scene;
		gl_renderer* m_renderer;
		bool open_mod_popup = false;
	};

}