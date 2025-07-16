#include "entity sys/components/pointlight_component.h"

wizm::pointlight_component::pointlight_component(float constant, float linear, float quadratic, float radius, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular)
    :light_component(ambient, diffuse, specular)
{
    m_component_type = ePointLight;

    m_constant = constant;
    m_linear = linear;
    m_quadratic = quadratic;

    m_radius = radius;
}

wizm::pointlight_component::~pointlight_component()
{
   
}

void wizm::pointlight_component::component_preupdate()
{

}

void wizm::pointlight_component::component_update(float delta_time, std::shared_ptr<core_gl_shader>& shader)
{
    (void)shader;
    (void)delta_time;
}

void wizm::pointlight_component::component_postupdate()
{
}

lowlevelsys::core_component* wizm::pointlight_component::_copy() const
{
    auto new_pc_comp = new pointlight_component();

    new_pc_comp->set_position(this->get_position());
    new_pc_comp->set_rotation(this->get_rotation());
    new_pc_comp->set_scale(this->get_scale());

    new_pc_comp->light_index = this->light_index + 1;
    new_pc_comp->m_ambient = this->m_ambient;
    new_pc_comp->m_constant = this->m_constant;
    new_pc_comp->m_diffuse = this->m_diffuse;
    new_pc_comp->m_is_active = this->m_is_active;
    new_pc_comp->m_is_visible = this->m_is_visible;
    new_pc_comp->m_linear = this->m_linear;
    new_pc_comp->m_quadratic = this->m_quadratic;
    new_pc_comp->m_radius = this->m_radius;
    new_pc_comp->m_specular = this->m_specular;
    new_pc_comp->m_intensity = this->m_intensity;

    return new_pc_comp;
}
