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
	if (m_shader != shader)
		m_shader = shader;

	
	shader->setVec3("dirLight.ambient", m_ambient * m_brightness);
	shader->setVec3("dirLight.diffuse", m_diffuse);
	shader->setVec3("dirLight.specular", m_specular);
	shader->setVec3("dirLight.direction", glm::normalize(get_direction()));
}

void wizm::directionallight_component::component_postupdate()
{
}

glm::mat4 wizm::directionallight_component::computeLightSpaceMatrix(const glm::vec3& scene_center, float scene_radius, float ortho_size, float near_plane, float far_plane)
{

	glm::vec3 light_dir = glm::normalize(this->get_direction());

	float distance = std::max(scene_radius * 2.0f, ortho_size);
	glm::vec3 light_pos = scene_center - light_dir * distance;

	
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	if (glm::abs(glm::dot(up, light_dir)) > 0.99f)
		up = glm::vec3(0.0f, 0.0f, 1.0f); 

	glm::mat4 light_view = glm::lookAt(light_pos, scene_center, up);

	
	float half = ortho_size * 0.5f;
	glm::mat4 light_proj = glm::ortho(-half, half, -half, half, near_plane, far_plane);


	return light_proj * light_view;
	
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
