#pragma once
#include "layer.h"
#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_glfw.h"
#include "imgui/backends/imgui_impl_opengl3.h"
#include <vector>
#include <string>
#include <fstream>
#include <unordered_set>

namespace wizm {

	class asset_manager;

	class script_editor_layer : public core_layer {
	public:
		script_editor_layer(asset_manager* passet_manager);
		~script_editor_layer();

		virtual void on_attach() override;
		virtual void on_detach() override;
		virtual void update(float delta_time) override;

		bool write_script(const std::string& filepath, const char* buffer);
		bool load_script(const std::string& filepath, char* buffer, size_t bufferSize);


	private:
		asset_manager* m_asset_manager;
		std::string crnt_script_asset_id; 
		std::string crnt_script_asset_path;


	};

}