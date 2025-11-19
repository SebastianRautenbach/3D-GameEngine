#include "entity sys/entity.h"
#include "other utils/IDGEN.h"
#include "scene.h"

	//////////////////////////////////////////////////////////////////////////
	// DEFAULT CONSTRUCTOR
	//////////////////////////////////////////////////////////////////////////

	//-----------------------------------------------------------------------

wizm::core_entity::core_entity(std::string ent_name, std::string ent_guid, core_scene* scene)
	:m_ent_name(ent_name), global_scene(scene)
{
	if (ent_guid.empty()) {
		ent_guid = generate_unique_id();
	}
	
	m_guid = ent_guid;
	
	entity_tags = core_tag();
}

	//-----------------------------------------------------------------------

wizm::core_entity::~core_entity()
{
	destroy_entity();
}

void wizm::core_entity::remame_entity(std::string name)
{
	m_ent_name = name;
}

void wizm::core_entity::destroy_entity()
{
	
}


//////////////////////////////////////////////////////////////////////////
// COPY
//////////////////////////////////////////////////////////////////////////


//-----------------------------------------------------------------------

core_entity* wizm::core_entity::copy_(std::string name) const
{
	std::string new_ent_ID = name;
	core_entity* new_entity = new core_entity(new_ent_ID);

	new_entity->global_scene = global_scene;

	
	new_entity->set_position(get_position());
	new_entity->set_rotation(get_rotation());
	new_entity->set_scale(get_scale());

	
	new_entity->entity_tags.tags = this->entity_tags.tags;
	



	for (const auto& component : m_components_list)
	{
		const auto& comp = component->_copy();
		global_scene->m_dirty_components.emplace_back(comp);
		new_entity->add_component(comp);
	}


	return new_entity;
}


	//////////////////////////////////////////////////////////////////////////
	// COMPONENTS
	//////////////////////////////////////////////////////////////////////////


	//-----------------------------------------------------------------------

core_component* wizm::core_entity::add_component(core_component* component)
{
	component->add_parent(this);
	m_components_list.push_back(component);
	global_scene->m_dirty_components.emplace_back(component);
	global_scene->m_rebuild_lights = true;
	
	if(component->m_component_type == eDirectionalLight)
		global_scene->m_directional_light = dynamic_cast<directionallight_component*>(component);
	
	return component;
}


core_component* wizm::core_entity::get_component(eCompType comp_type)
{

	for (auto& i: m_components_list)
	{
		if (i->m_component_type == comp_type)
		{
			return i;
		}
	}


	return nullptr;
}

int wizm::core_entity::get_component_index(eCompType comp_type)
{
	for (int i = 0; i < m_components_list.size(); i++)
	{
		if (m_components_list[i]->m_component_type == comp_type)
			return i;
	}
	return -1;
}


void wizm::core_entity::remove_component(unsigned int index)
{
	m_components_list.erase(m_components_list.begin() + index);
}


void wizm::core_entity::set_component(unsigned int index, core_component* component)
{
	m_components_list[index] = component;
}

void wizm::core_entity::read_saved_data(std::string parent_name, std::string index, filedata::ZER& save_t)
{

	set_position(glm::vec3(
		save_t["transform"].get_float("position")[0],
		save_t["transform"].get_float("position")[1],
		save_t["transform"].get_float("position")[2]));

	set_rotation(glm::vec3(
		save_t["transform"].get_float("rotation")[0],
		save_t["transform"].get_float("rotation")[1],
		save_t["transform"].get_float("rotation")[2]));

	set_scale(glm::vec3(
		save_t["transform"].get_float("scale")[0],
		save_t["transform"].get_float("scale")[1],
		save_t["transform"].get_float("scale")[2]));



	//-------------------------------------------------------------------------------- COMPONENTS
	for (auto& i : save_t.class_properties) {

		//--- POINT LIGHT

		if (i.first.find("pointlight") != -1) {
			auto c = add_component(new pointlight_component());
			c->read_saved_data(m_guid, i.first, save_t);
		}

		//--- DIRECTIONAL LIGHT

		if (i.first.find("directionallight") != -1) {
			auto c = add_component(new directionallight_component());
			c->read_saved_data(m_guid, i.first, save_t);
		}

		//--- MESH COMPONENT

		if (i.first.find("staticmesh") != -1) {
			auto c = add_component(new staticmesh_component());
			c->read_saved_data(m_guid, i.first, save_t);
		}


		//--- SPOT LIGHT

		if (i.first.find("spotlight") != -1) {
			auto c = add_component(new spotlight_component());
			c->read_saved_data(m_guid, i.first, save_t);
		}

		//--- CAMERA COMPONENT

		if (i.first.find("cameracomponent") != -1) {
			auto c = add_component(new camera_component());
			c->read_saved_data(m_guid, i.first, save_t);
		}

		//--- SCRIPTING COMP

		if (i.first.find("ScriptingComponent") != -1) {
			auto c = add_component(new scripting_component());
			c->read_saved_data(m_guid, i.first, save_t);
		}

		//--- SCRIPTING SOUND

		if (i.first.find("sound_component") != -1) {
			auto c = add_component(new sound_component());
			c->read_saved_data(m_guid, i.first, save_t);
		}

	}
}

void wizm::core_entity::save_data(std::string parent_name, std::string index, filedata::ZER& save_t) const
{

	save_t["specs"].set_int("is_entity", { true });

	save_t["transform"].set_float("position", { get_position().x, get_position().y, get_position().z });
	save_t["transform"].set_float("rotation", { get_rotation().x, get_rotation().y, get_rotation().z });
	save_t["transform"].set_float("scale", { get_scale().x, get_scale().y, get_scale().z });



	save_t["tags"].set_string("tags", entity_tags.tags);

	for (int i = 0; i < m_components_list.size(); i++) {
		m_components_list[i]->save_data(m_guid, std::to_string(i), save_t);
	}
}


	//-----------------------------------------------------------------------