#pragma once
#include "system/assets.h"
#include "gl core/core shader.h"
#include <filesystem>

namespace wizm {


	class texture_asset : public core_asset {

	public:
		~texture_asset() override {
			texture->delete_texture();
			delete texture;
		}

		void load(const std::string& path) override {
			texture = new core_gl_texture(path.c_str(),true);
			std::filesystem::path file_path(path);
			file_name = file_path.filename().string();
		}

		void unbind() {
			texture->unbind_texture();
			bounded = false;
		}

		void draw(core_gl_shader* shader) {
			(void)shader;
			texture->bind_texture();
			bounded = true;
		}

		bool is_bounded() { return bounded; }


	public:
		
		core_gl_texture* texture;
		std::string file_name;


	private:
		bool bounded = false;
	};
}
