#pragma once
#include "system/assets.h"
#include "texture_asset.h"
#include <filesystem>
#include "other utils/ZER.h"
#include "gl core/core shader.h"

namespace wizm {

	class material_asset : public core_asset {
		
	public:
		std::string _path;

		void save() {
			filedata::ZER save_t;
			save_t.read_file_cntx(this->_path);
			save_t["material"]["textures"].set_string("diffuse", { this->diffuse_asset_id });
			save_t["material"]["textures"].set_string("specular", { this->specular_asset_id });
			save_t["material"].set_float("shininess", { this->m_shininess });
			save_t.save_file(save_t, this->_path);
		}
		
		void load(const std::string& path) override {
			
			filedata::ZER save_object;
			save_object.read_file_cntx(path);
			diffuse_asset_id = save_object["material"]["textures"].get_string("diffuse")[0];
			specular_asset_id = save_object["material"]["textures"].get_string("specular")[0];
			m_shininess = save_object["material"].get_float("shininess")[0];

			_path = path;
			std::filesystem::path file_path(path);
			file_name = file_path.filename().string();
		}

		void unbind() {
			if (m_diffuse_texture) {
				glActiveTexture(GL_TEXTURE0);
				m_diffuse_texture->unbind();
			}
			if (m_specular_texture) {
				glActiveTexture(GL_TEXTURE1);
				m_specular_texture->unbind();
			}
		}

		void apply(std::shared_ptr<core_gl_shader> m_shader) {
		
			if (m_diffuse_texture) {
				glActiveTexture(GL_TEXTURE0);
				m_diffuse_texture->draw(nullptr);
			}
			if (m_specular_texture) {
				glActiveTexture(GL_TEXTURE1);
				m_specular_texture->draw(nullptr);
			}
		}

		void update_mat_specs(std::shared_ptr<core_gl_shader> m_shader) {			
			
			if (!this) { return; }
			m_shader->setFloat("material.shininess", m_shininess);

			// roughness and other cool props will be added
		}
		
		std::string diffuse_asset_id = "";
		std::string specular_asset_id = "";
		
		std::shared_ptr<texture_asset> m_diffuse_texture;
		std::shared_ptr<texture_asset> m_specular_texture;

		float m_shininess = 32.f;
	};
}