#include "scene.h"
#include <functional>

namespace wizm {
	core_scene::~core_scene()
	{
		clear_entities();
	}
	void core_scene::scene_preupdate()
	{

		for (auto& i : m_entities)
		{
			for (auto comp : i->m_components_list) {

				if (!is_light_component(comp->m_component_type)) {
					comp->component_preupdate();
				}
			}
		}

		
	}
	
	void core_scene::scene_update(float delta_time, std::shared_ptr<core_gl_shader>& shader)
	{		
		#pragma omp parallel for
		for (size_t j = 0; j < m_entities.size(); ++j) {
			auto& i = m_entities[j];
			for (auto& comp : i->m_components_list) {
				if (!is_light_component(comp->m_component_type)) {
					comp->component_update(delta_time, shader);
				}
			}
		}
	}
	
	void core_scene::scene_postupdate()
	{
		for (auto& i : m_entities)
		{
			for (auto comp : i->m_components_list) {

				if (!is_light_component(comp->m_component_type)) {
					comp->component_postupdate();
				}

			}
		}
	}

	unsigned int core_scene::total_component_count()
	{
		unsigned int total = 0;
		for (auto& i : m_entities)
			total += i->m_components_list.size();
	
		return total;
	}

	bool core_scene::does_ent_name_exist(std::string name)
	{
		bool does_exist = false;
		for (auto& i : m_entities)
			if (i->m_ent_name == name) 
				does_exist = true;
		
		return does_exist;
	}


	// if a component is deleted changed or added
	void core_scene::on_component_change()
	{
		m_rebuild_lights = true;
	}



	core_entity* core_scene::add_entity(core_entity* entity) {
		
		entity->m_guid = generate_unique_id(); 

		entity->global_scene = this;
		
		m_entities.push_back(entity);

		m_dirty_components.insert(m_dirty_components.end(), entity->m_components_list.begin(),
			entity->m_components_list.end());

		on_component_change();

		return entity;
	}
	
	core_entity* core_scene::add_entity(std::string entity_name, std::string entity_guid)
	{
		auto ptr = new core_entity(entity_name, entity_guid, this);
		m_entities.push_back(ptr);
		return ptr;
	}

	void core_scene::load_entity(filedata::ZER& entity_save)
	{
		for (auto& entity : entity_save.class_properties) {
			process_entity(*entity.second, nullptr, entity.first);
		}
	}

	void core_scene::process_entity(filedata::ZER& new_read, core_entity* parent, const std::string& guid)
	{
		if (!new_read["specs"].get_int("is_entity")[0])
			return;		

		auto new_ent = add_entity(new_read["specs"].get_string("m_ent_name")[0], guid );

		if (new_ent) {

			new_ent->read_saved_data("", "", new_read);
			
			m_dirty_components.insert(m_dirty_components.end(), new_ent->m_components_list.begin(),
				new_ent->m_components_list.end());

			on_component_change();

			if (parent)
				new_ent->add_parent(parent);

			for (auto& r : new_read.class_properties) {
				process_entity(new_read[r.first], new_ent, r.first);
			}
		}
	}

	filedata::ZER core_scene::save_entity(core_entity* entity)
	{
		std::function<void(core_entity*, filedata::ZER&)> process_entity = [&](core_entity* entity, filedata::ZER& read) {

			if (!entity) { return; }

			read["specs"].set_int("is_entity", { 1 });
			entity->save_data("", "", read);

			for (auto& child : entity->get_children()) {

				auto ent_child = dynamic_cast<core_entity*>(child);
				if (!ent_child) { continue; }
				process_entity(ent_child, read[ent_child->m_guid]);
			}
			};

		filedata::ZER read;
	
		process_entity(entity, read[entity->m_guid]);

		return read;
	}	


	void core_scene::delete_entity(core_entity* entity)
	{
		if (entity == NULL) { return; }
		
		on_component_change();
		
		// clear all entities with the same parent from the entity list to ensure that a call to a nullptr isn't made.
		for (auto& child : entity->get_children()) {		
			std::function<void(core_entity*)> go_to_child = [&](core_entity* pchild) {

				if (!pchild) { return nullptr; }

				for (auto& c : pchild->get_children()) {
					go_to_child(dynamic_cast<core_entity*>(c));
				}

				m_entities.erase(std::find(m_entities.begin(), m_entities.end(), pchild));				
			};

			go_to_child(dynamic_cast<core_entity*>(child));
		}
		
		m_entities.erase(std::find(m_entities.begin(), m_entities.end(), entity));

		std::cout << "deleting:" << entity << "\n";
		delete entity;
	}

	void core_scene::delete_all_entities()
	{
		on_component_change();
		while (!m_entities.empty()) {
			auto ent = m_entities.back();

			m_entities.pop_back();

			delete ent;
		}
	}

	void core_scene::delete_component(core_entity* entity,core_component* component)
	{
		entity->m_components_list.erase(std::find(entity->m_components_list.begin(),
			entity->m_components_list.end(), component));
		delete component;
		m_rebuild_lights = true;
	}

	

	void core_scene::clear_entities()
	{		
		m_rebuild_lights = true;
		delete_all_entities();
		clear_selected_entities();
	}

	//--------------------------------------------------------- SERIALIZATION METHODS

	void core_scene::read_map_data(std::string file_path) {
		
		filedata::ZER read;
		read.read_file_cntx(file_path);
		current_scene = file_path;

		clear_entities();


		for (auto& entity : read.class_properties) {
			process_entity(*entity.second, nullptr, entity.first);
		}

	}


	void core_scene::save_map_data(std::string path) {
		

		std::function<void(core_entity*, filedata::ZER&)> process_entity = [&](core_entity* entity, filedata::ZER& read) {

			if (!entity) { return; }

			read["specs"].set_int("is_entity", { 1 });
			read["specs"].set_string("m_ent_name", { entity->m_ent_name });
			entity->save_data("", "", read);

			for (auto& child : entity->get_children()) {
				
				auto ent_child = dynamic_cast<core_entity*>(child);
				if (!ent_child) { continue; }
				process_entity(ent_child, read[ent_child->m_guid]);
			}
		};

		filedata::ZER read;

		for (const auto& e : m_entities) {
			

			if(!e->get_parent())
				process_entity(e, read[e->m_guid]);
			

			
		}

		if(current_scene.empty() || !path.empty())
		{
			read.save_file(read, path);
			current_scene = path;
		}
		else
			read.save_file(read, current_scene);
	}








	void core_scene::pre_update_light_components()
	{
		for (auto& i : m_entities)
		{
			for (auto& comp : i->m_components_list) {

				if (is_light_component(comp->m_component_type)) {
					comp->component_preupdate();
				}
			}
		}
	}

	void core_scene::update_light_components(float delta_time, std::shared_ptr<core_gl_shader>& shader)
	{
		for (auto& i : m_entities)
		{
			for (auto& comp : i->m_components_list) {
			
				if (is_light_component(comp->m_component_type)) {
					comp->component_update(delta_time, shader);				
				}
			}
		}
	}

	void core_scene::post_update_light_components()
	{
		for (auto& i : m_entities)
		{
			for (auto& comp : i->m_components_list) {

				if (is_light_component(comp->m_component_type)) {
					comp->component_postupdate();
				}
			}
		}
	}

}