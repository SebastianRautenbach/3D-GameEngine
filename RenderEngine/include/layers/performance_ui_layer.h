#pragma once
#include "layer.h"
#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_glfw.h"
#include "imgui/backends/imgui_impl_opengl3.h"
#include <memory>

namespace wizm {

	class core_scene;

	class performace_ui_layer : public core_layer {
	public:
		performace_ui_layer(core_scene* scene);
		~performace_ui_layer();

		virtual void on_attach() override;
		virtual void on_detach() override;
		virtual void update(float delta_time) override;

	private:
		core_scene* global_scene;
	};

}