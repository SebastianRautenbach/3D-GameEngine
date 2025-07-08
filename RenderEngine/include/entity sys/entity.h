#pragma once
#include "system/node.h"
#include <string>
#include <vector>
#include "component.h"
#include "system/tag.h"
#include <memory>
#include "components/all_components_list.h"

using namespace lowlevelsys;

namespace wizm {

	class core_scene;

	class core_entity : public core_node
	{

	public:
	
		core_entity(std::string ent_ID, std::string ent_guid = "", core_scene* scene = nullptr);
		~core_entity();
	
		
		// entity
		void remame_entity(std::string name);
		void destroy_entity();
		core_entity* get_entity() { return this; }


		// duplication
		
		core_entity* copy_(std::string name) const;



		// component
		
		core_component* add_component(core_component* component);
		core_component* get_component(eCompType comp_type);
		const std::vector<core_component*>& get_components() const { return m_components_list; }
		int get_component_index(eCompType comp_type);
		void remove_component(unsigned int index);
		void set_component(unsigned int index, core_component* component);
		
		core_component* get_selected_comp() { return m_selected_comp; }
		void set_selected_comp(core_component* component) { m_selected_comp = component; }


		//////////////////////////////////////////////////
		//////////  SERIALIZATION METHOD /////////////////
		//////////////////////////////////////////////////

		void read_saved_data(std::string parent_name, std::string index, filedata::ZER& save_t) override;

		void save_data(std::string parent_name, std::string index, filedata::ZER& save_t) const override;

	public:

		std::string m_ent_name;
		std::string m_guid;

		int entity_color_id;

		core_tag* entity_tags;
		std::vector<core_component*> m_components_list;
		core_scene* global_scene;

	private:
		core_component* m_selected_comp;

	};


}