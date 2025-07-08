#include "system/billboard_manager.h"
#include "other utils/common.h"
#include "scene.h"

wizm::billboard_manager::billboard_manager(std::shared_ptr<core_gl_shader> shader, core_scene* scene)
	: m_shader(shader), global_scene(scene)
{
	m_billboard_core = new billboard_core(m_shader);
	m_shader->setVec3("tint", glm::vec3(1.0f));
}

void wizm::billboard_manager::render()
{
	if(engine_status == EDITOR_STATUS)
	{
		glDepthMask(GL_FALSE);
		for (const auto& ent : global_scene->m_entities) {
			for (const auto& comp : ent->m_components_list) {

				auto trans = comp->get_world_transform();

				if (comp->m_component_type == ePointLight) {
					m_billboard_core->draw(trans, eLightB);
				}
				else if (comp->m_component_type == eDirectionalLight)
				{
					m_billboard_core->draw(trans, eDirLightB);
				}
				else if (comp->m_component_type == eSpotLight)
				{
					m_billboard_core->draw(trans, eSpotLightB);
				}
				else if (comp->m_component_type == eCamera)
				{
					m_billboard_core->draw(trans, eCameraB);
				}
				else if (comp->m_component_type == eSound3D) 
				{
					m_billboard_core->draw(trans, eSoundB);
				}
			}
		}
		glDepthMask(GL_TRUE);
	}
}
