#pragma once
#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_glfw.h"
#include "imgui/backends/imgui_impl_opengl3.h"
#include "layer.h"
#include "gl core/core shader.h"
#include "system/asset_manager.h"
#include "system/filewatcher.h"
#include "system/asset_importer.h"
#include "gl core/core shader.h"

namespace wizm { 
	
	
	class content_browser_layer : public core_layer
	{
	public:
		content_browser_layer(asset_manager* p_asset_manager);
		~content_browser_layer();
	
		virtual void on_attach() override;
		virtual void on_detach() override;
		virtual void update(float delta_time) override;

		void refresh_assets();
		
		std::vector<std::filesystem::path> get_directory_content(const std::filesystem::path& path);


	public:

		filewatcher* watcher;
		asset_importer asset_import;
		asset_manager* m_asset_manager;
		std::vector<asset_details> assets;
		std::filesystem::path current_directory = "GAME";

		core_gl_texture* folder_file_texture;
		core_gl_texture* mesh_file_icon;
		core_gl_texture* file_texture;
		core_gl_texture* image_file_icon;
		core_gl_texture* map_file_icon;
		core_gl_texture* script_file_icon; 
		core_gl_texture* sound_file_icon; 
		core_gl_texture* material_file_icon; 
		core_gl_texture* entity_file_icon;

	private:
		std::string generate_unique_name(
			const std::vector<std::filesystem::path>& all_files_in_dir,
			const std::string& base_name,
			const std::string& extension);

		std::vector <std::filesystem::path> all_content;
	}; 


}
