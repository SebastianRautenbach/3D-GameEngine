#pragma once
#include "entity sys/entity.h"
#include <vector>


namespace wizm {


	class core_scene {
	public:
		~core_scene();
		// default scene updates
		void scene_preupdate();
		void scene_update(float delta_time, std::shared_ptr<core_gl_shader>& shader);
		void scene_postupdate();
		unsigned int total_component_count();
		bool does_ent_name_exist(std::string name);
		void on_component_change();

		// serialization
		void read_map_data(std::string file_path);
		void save_map_data(std::string path);
		
		
		// entity related 
		core_entity* add_entity(core_entity* entity);
		core_entity* add_entity(std::string entity_name, std::string entity_guid = "");
		void load_entity(filedata::ZER& entity_save);
		void process_entity(filedata::ZER& new_read, core_entity* parent, const std::string& guid);
		filedata::ZER save_entity(core_entity* entity);


		core_entity* get_entity(std::string entity_id) { 
			for (auto& ent : m_entities) { 
				if (ent->m_guid == entity_id) {
					return ent; 
				} 
				
			}
			return NULL;
		}
		
		void delete_entity(core_entity* entity);
		void delete_all_entities();

		void delete_component(core_entity* entity, core_component* component);

		// modifying only one entity
		//-------------------------------------------------------------------------------------------------------- NEW SELECTION
		
		std::vector<core_entity*> get_selected_entities() {
			return m_selected_entities;
		}

		void add_selected_entity(core_entity* ent) {
			for (auto& e : m_selected_entities) {
				if (e == ent) { return; }
			}
			m_selected_entities.emplace_back(ent);
		}

		void remove_selected_entity(core_entity* ent) {
			m_selected_entities.erase(std::find(m_selected_entities.begin(), m_selected_entities.end(), ent));
		}

		void clear_selected_entities() {			
			m_selected_entities.clear();
		}


		core_entity* get_crnt_entity() {
			if (m_selected_entities.empty()) { return nullptr; }
			return m_selected_entities[m_selected_entities.size() - 1] ? m_selected_entities[m_selected_entities.size() - 1] : nullptr;
		}

		//-------------------------------------------------------------------------------------------------------- NEW SELECTION


		

		core_entity* get_entity_by_id(std::string id) {
			for (auto& ent : m_entities) {
				if (ent->m_guid == id)
					return ent;
			}
		}



		// temporarly
		void pre_update_light_components();
		void update_light_components(float delta_time, std::shared_ptr<core_gl_shader>& shader);
		void post_update_light_components();


	private:

		// remove all entities

		void clear_entities();

	public:		
		
		std::vector<core_entity*> m_entities;
		std::vector<core_component*> m_dirty_components;


		bool m_rebuild_lights = false;		
		std::string current_scene = "";

	private:
		std::vector<core_entity*> m_selected_entities;		
		bool m_frustum_loaded = false;
	};


}
