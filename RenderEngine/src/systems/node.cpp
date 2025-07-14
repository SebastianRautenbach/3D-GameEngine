#include "system/node.h"



//////////////////////////////////////////////////////////////////////////
// DEFAULT CONSTRUCTOR
//////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------




//----------------------------------------------------------------------------------------------


//----------------------------------------------------------------------------------------------




//----------------------------------------------------------------------------------------------

wizm::core_node::core_node()
	:m_translation(0.0f), m_rotation(0.0f), m_scale(1.0f)
{
}

wizm::core_node::~core_node()
{
    while (!m_child_nodes.empty()) {
        auto ent = m_child_nodes.back();

        m_child_nodes.pop_back();

        delete ent;
    }

   if (m_parent_node) {
       auto& siblings = m_parent_node->m_child_nodes;
       siblings.erase(std::remove(siblings.begin(), siblings.end(), this), siblings.end());
   }
}

void wizm::core_node::set_position(const glm::vec3& position)
{

    m_translation = position;
}

void wizm::core_node::add_position(const glm::vec3& offset)
{
    m_translation += offset;
}

void wizm::core_node::set_rotation(const glm::vec3& rotation)
{

    m_rotation = rotation;
}

void wizm::core_node::add_rotation(const glm::vec3& deltaRotation)
{

    m_rotation += deltaRotation;
}

void wizm::core_node::set_scale(const glm::vec3& scale)
{
    m_scale = scale;
}

void wizm::core_node::add_scale(const glm::vec3& deltaScale)
{
    m_scale += deltaScale;
}

glm::mat4 wizm::core_node::get_world_transform()
{

    static const glm::mat4 identity = glm::mat4(1.0f);

    glm::mat4 rotation = glm::mat4_cast(glm::quat(m_rotation));
    glm::mat4 localTransform = glm::translate(identity, m_translation) *
        rotation *
        glm::scale(identity, m_scale);

    if (!m_parent_node) {
        return localTransform;
    }

    return m_parent_node->get_world_transform() * localTransform;

}
