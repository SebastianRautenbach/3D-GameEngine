#pragma once 
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "gl core/vertex_buffer.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "system/camera_manager.h"
#include <memory>
#include <vector>
#include "system/compute_cluster_test.h"



namespace wizm {
	class camera_manager;
	class core_scene;
}

class core_gl_shader;

namespace lowlevelsys {

	class gl_renderer {
	public:
		void setup(int window_size_x, int window_size_y, const char* window_name, std::shared_ptr<wizm::camera_manager> camera_manager,
			wizm::core_scene* scene);
		
		void pre_render(bool& is_running, float deltaTime);
		void render(float deltaTime);
		void post_render(float deltaTime);
		void update_draw_data();
		void on_exit();

	public:

		core_scene* global_scene;

		GLFWwindow* window;
		
		std::map<int, std::shared_ptr<core_gl_shader>> m_shdrs;

		std::shared_ptr<camera_manager> m_camera_manager; 

	public:
		int w_width, w_height;

	private:
		unsigned int shader_count = 0;
	};
}
