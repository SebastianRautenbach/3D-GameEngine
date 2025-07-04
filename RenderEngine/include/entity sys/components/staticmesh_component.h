#pragma once
#include "entity sys/component.h"
#include "system/core_model.h"
#include "system/material.h"
#include "system/assets/mesh_asset.h"
#include "system/assets/material_asset.h"
#include "system/asset_manager.h"

namespace wizm {
	class staticmesh_component : public lowlevelsys::core_component {
	
	public:
		
		staticmesh_component();
		staticmesh_component(std::shared_ptr<staticmesh_asset> mesh);
		~staticmesh_component();
		
		void component_preupdate() override;
		void component_update(float delta_time, std::shared_ptr<core_gl_shader>& shader) override;
		void component_postupdate() override;
		core_component* _copy() const;


		//////////////////////////////////////////////////
		//////////  SERIALIZATION METHOD /////////////////
		//////////////////////////////////////////////////


		void read_saved_data(std::string parent_name, std::string index, filedata::ZER& save_t) override {
		
		
			set_position(glm::vec3(
				save_t[index]["transform"].get_float("position")[0],
				save_t[index]["transform"].get_float("position")[1],
				save_t[index]["transform"].get_float("position")[2]
			));
			set_rotation(glm::vec3(
				save_t[index]["transform"].get_float("rotation")[0],
				save_t[index]["transform"].get_float("rotation")[1],
				save_t[index]["transform"].get_float("rotation")[2]
			));
			set_scale(glm::vec3(
				save_t[index]["transform"].get_float("scale")[0],
				save_t[index]["transform"].get_float("scale")[1],
				save_t[index]["transform"].get_float("scale")[2]
			));

			m_mesh_asset_id = save_t[index].get_string("asset id")[0];
			m_material_asset_ids = save_t[index].get_string("m_material_asset_ids");
			is_static = save_t[index].get_int("is_static")[0];
		};


		void save_data(std::string parent_name, std::string index, filedata::ZER& save_t) const override {

			save_t["staticmesh" + index]["transform"].set_float("position", { get_position().x, get_position().y, get_position().z });
			save_t["staticmesh" + index]["transform"].set_float("rotation", { get_rotation().x, get_rotation().y, get_rotation().z });
			save_t["staticmesh" + index]["transform"].set_float("scale", { get_scale().x, get_scale().y, get_scale().z });

			save_t["staticmesh" + index].set_string("asset id", { m_mesh_asset_id });
			save_t["staticmesh" + index].set_string("m_material_asset_ids", m_material_asset_ids);
			save_t["staticmesh" + index].set_int("is_static", { is_static});

			for(auto& mat : m_materials)
			{
				if(mat)
					mat->save();
			}
		}


	public:
		bool is_static = true;
		std::vector<std::shared_ptr<material_asset>> m_materials;
		std::shared_ptr<staticmesh_asset> m_model;
		std::string m_mesh_asset_id;
		std::vector<std::string> m_material_asset_ids;


	private:
		bool is_dirty = false;
		int LOD_level = 0;
		int previous_LOD_level = 0;
	};
}