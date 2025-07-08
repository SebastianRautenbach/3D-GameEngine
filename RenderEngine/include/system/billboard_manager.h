#pragma once
#include "billboard.h"


namespace wizm {

	class core_scene;

	class billboard_manager {
	public:
		
		billboard_manager( std::shared_ptr<core_gl_shader> shader, core_scene* scene);
		void render();


	public:
		glm::vec3 color_tint = glm::vec3(1.0);
		billboard_core* m_billboard_core;
		std::shared_ptr<core_gl_shader> m_shader;
		core_scene* global_scene;
	};

}