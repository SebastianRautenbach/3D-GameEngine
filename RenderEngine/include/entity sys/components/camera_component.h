#pragma once
#include "entity sys/component.h"
#include "system/core_renderable.h"


using namespace lowlevelsys;

namespace wizm {

	class camera_core_3d;

	class camera_component : public core_component, public core_renderable {

	public:
		camera_component();
		~camera_component();

	public:
		void component_preupdate() override;
		void component_update(float delta_time, std::shared_ptr<core_gl_shader>& shader) override;
		void component_postupdate() override;
		core_component* _copy() const;


		void read_saved_data(std::string parent_name, std::string index, filedata::ZER& save_t) override {
			set_position(glm::vec3(
				save_t[index]["transform"].get_float("position")[0],
				save_t[index]["transform"].get_float("position")[1],
				save_t[index]["transform"].get_float("position")[2]
			));
			set_rotation(glm::vec3(
				save_t[index]["transform"].get_float("rotation")[0],
				save_t[index]["transform"].get_float("rotation")[1],
				save_t[index]["transform"].get_float("rotation")[2]
			));
			set_scale(glm::vec3(
				save_t[index]["transform"].get_float("scale")[0],
				save_t[index]["transform"].get_float("scale")[1],
				save_t[index]["transform"].get_float("scale")[2]
			));

			is_main_camera = save_t[index]["settings"].get_int("is_main_camera")[0];
		
		}
		void save_data(std::string parent_name, std::string index, filedata::ZER& save_t) const override {
			(void)parent_name;
			save_t["cameracomponent" + index]["transform"].set_float("position", { get_position().x, get_position().y, get_position().z });
			save_t["cameracomponent" + index]["transform"].set_float("rotation", { get_rotation().x, get_rotation().y, get_rotation().z });
			save_t["cameracomponent" + index]["transform"].set_float("scale", { get_scale().x, get_scale().y, get_scale().z });
			save_t["cameracomponent" + index]["settings"].set_int("is_main_camera", { is_main_camera });
		}


	public:
		std::shared_ptr<camera_core_3d> m_camera;
		bool is_main_camera = false;
	};
}
