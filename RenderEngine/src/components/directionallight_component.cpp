#include "entity sys/components/directionallight_component.h"

wizm::directionallight_component::directionallight_component( glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular)
	:light_component(ambient, diffuse, specular)
{
		m_component_type = eDirectionalLight;
}

wizm::directionallight_component::~directionallight_component()
{
	// the light doesnt actually get deleted
	// we fake it by blacking everything.

	if (!m_shader) { return; }

	m_shader->use_shader();
	m_shader->setVec3("dirLight.ambient", glm::vec3(0));
	m_shader->setVec3("dirLight.diffuse", glm::vec3(0));
	m_shader->setVec3("dirLight.specular", glm::vec3(0));
}

void wizm::directionallight_component::component_preupdate()
{

}

void wizm::directionallight_component::component_update(float delta_time, std::shared_ptr<core_gl_shader>& shader)
{
	(void)delta_time;


	
	
	
	
	
	
	


	if (m_shader != shader)
		m_shader = shader;


	glm::mat4 lightProjection, lightView;
	glm::mat4 light_matrix;

	glm::vec3 pos = get_world_position();

	light_matrix = lightProjection = lightView = glm::mat4(1.0);


	float near_plane = 1.0f, far_plane = 7.5f;
	lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
	lightView = glm::lookAt(pos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
	light_matrix = lightProjection * lightView;
	shader->setMat4("light_matrix", light_matrix);


	shader->setVec3("dirLight.direction", get_world_rotation());
	shader->setVec3("dirLight.ambient", m_ambient * glm::vec3(m_brightness));
	shader->setVec3("dirLight.diffuse", m_diffuse);
	shader->setVec3("dirLight.specular", m_specular);
}

void wizm::directionallight_component::component_postupdate()
{
}

core_component* wizm::directionallight_component::_copy() const
{
	auto new_dl_comp = new directionallight_component();

	new_dl_comp->set_position(this->get_position());
	new_dl_comp->set_rotation(this->get_rotation());
	new_dl_comp->set_scale(this->get_scale());
	
	new_dl_comp->m_ambient = this->m_ambient;
	new_dl_comp->m_diffuse = this->m_diffuse;
	new_dl_comp->m_is_active = this->m_is_active;
	new_dl_comp->m_is_visible = this->m_is_visible;
	new_dl_comp->m_specular = this->m_specular;

	return new_dl_comp;
}
