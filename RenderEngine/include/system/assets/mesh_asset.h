#pragma once
#include "system/assets.h"
#include "system/core_model.h"
#include "system/core_renderable.h"
#include <filesystem>

using namespace lowlevelsys;


namespace wizm {

	class staticmesh_asset : public core_asset, public core_renderable {
	public:

		//~staticmesh_asset() override {
		//}

		void set_mesh(std::shared_ptr<core_model> model) { m_model = model; }
		std::shared_ptr<core_model> get_mesh() { return m_model; }

		std::vector<int> _gettest() { return test_del; }

		int material_count() { return m_model->m_num_materials; }
		
		void load(const std::string& path) override {
			if(!path.empty())
			{
				std::filesystem::path file_path(path);
				
				m_model = std::make_unique<core_model>(path.c_str());
				file_name = file_path.filename().string();
			}
		}
		void draw(unsigned int index) {
			if(index < m_model->meshes.size())
				m_model->meshes[index].draw_mesh();
		}

		unsigned int get_triangles() {
			return m_model->total_triangles;
		}

		unsigned int get_vertices_count() {
			return m_model->total_vertices;
		}

		std::vector<unsigned int> retrieve_all_indices() {
			std::vector<unsigned int> indices;
			for (const auto& mesh : m_model->meshes) {
				for (const auto& i : mesh.indices) {
					indices.emplace_back(i);
				}
			}
			return indices;
		}

		std::vector<vertex_data> retrieve_all_vertices() {
			std::vector<vertex_data> vertices;
			for (const auto& mesh : m_model->meshes) {
				for (const auto& i : mesh.vertices) {
					vertices.emplace_back(i);
				}
			}
			return vertices;
		}

	private:
		std::shared_ptr<core_model> m_model;
		std::vector<int> test_del;
	};

}