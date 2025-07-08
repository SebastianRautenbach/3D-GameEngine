#include "system/camera_manager.h"
#include "system/camera_3d.h"
#include "scene.h"

wizm::camera_manager::camera_manager(core_scene* scene)
	: global_scene(scene)
{
}

wizm::camera_manager::~camera_manager()
{
}

bool wizm::camera_manager::update_crnt_camera(bool simulate)
{
	did_change_cam = false;
	if(simulate)
	{
		for (const auto& ent : global_scene->m_entities)
		{
			for (const auto& comp : ent->m_components_list) {

				auto m_cam_comp = dynamic_cast<camera_component*>(comp);
				if (m_cam_comp) {

					if (m_cam_comp->is_main_camera) {

						m_crnt_camera = m_cam_comp->m_camera;
						did_change_cam = true;
					}
				}
			}
		}
	}
	else
	{
		m_crnt_camera = m_viewport_camera;
		did_change_cam = true;
	}

	return did_change_cam;
}

void wizm::camera_manager::save_viewport_camera(std::string crnt_save_path)
{
	filedata::ZER save_t;
	save_t.read_file_cntx(crnt_save_path);
	m_viewport_camera->save_data("viewport_camera", "", save_t);
	save_t.save_file(save_t, crnt_save_path);
}

void wizm::camera_manager::load_save_viewport_camera(std::string crnt_save_path)
{
	filedata::ZER save_t;
	save_t.read_file_cntx(crnt_save_path);
	m_viewport_camera->read_saved_data("viewport_camera", "", save_t);
}
