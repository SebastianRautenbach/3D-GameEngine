#include "gl renderer/gl_renderer.h"
#include <vector>
#include "system/camera_3d.h"
#include "system/scene_manager.h"
#include "system/input_manager.h"
#include "gl core/engine_shader_types.h"


void lowlevelsys::gl_renderer::setup(int window_size_x, int window_size_y, const char* window_name,
	std::shared_ptr<camera_manager> camera_manager, core_scene* scene)
{
	global_scene = scene;

	w_width = window_size_x;
	w_height = window_size_y;


	m_camera_manager = camera_manager;


	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(window_size_x, window_size_y, window_name, NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return;
	}


	glfwMakeContextCurrent(window);



	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return;
	}




	// manage input --------------------------------------------------------------------- /
	global_input_manager = new input_manager(window, static_cast<float>(w_width), static_cast<float>(w_height));


	m_camera_manager->m_viewport_camera = std::make_shared<camera_core_3d>(w_width, w_height);
	m_camera_manager->m_viewport_camera->set_position(glm::vec3(-1.76043, 1.11876, 1.69863));
	m_camera_manager->m_viewport_camera->set_rotation(-0.438943, -0.769122, 0.0);
	m_camera_manager->m_crnt_camera = m_camera_manager->m_viewport_camera;




	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);


	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	glEnable(GL_MULTISAMPLE);


	/*
	*	I should consider making a shader manager
	*	but for now this will do
	*/

	m_shdrs.emplace(ENGINE_SHADER_DEFUALT,new core_gl_shader("shaders/default_vrtx_shdr.glsl", "shaders/default_frgmnt_shdr_new.glsl"));
	m_shdrs.emplace(ENGINE_SHADER_BILLBOARD,new core_gl_shader("shaders/billboard_vrtx.glsl", "shaders/billboard_frgment.glsl"));
	m_shdrs.emplace(ENGINE_SHADER_PREPASS,new core_gl_shader("shaders/Z Pre-pass_vrtx_shdr.glsl", "shaders/Z Pre-pass_frgmnt_shdr.glsl"));
	m_shdrs.emplace(ENGINE_SHADER_CLUSTER_COMP,new core_gl_shader("shaders/cluster_comp_shdr.glsl"));
	m_shdrs.emplace(ENGINE_SHADER_CLUSTER_CULL,new core_gl_shader("shaders/cluster_cull_comp_shdr.glsl"));
	m_shdrs.emplace(ENGINE_SHADER_MOUSEPICK,new core_gl_shader("shaders/mouse_pick_vrtx.glsl", "shaders/mouse_pick_frgmnt.glsl"));
	
}


//-----------------------------------------------------------------------

void lowlevelsys::gl_renderer::pre_render(bool& is_running, float deltaTime)
{
	is_running = !glfwWindowShouldClose(window);


	glfwGetFramebufferSize(window, &w_width, &w_height);
	glViewport(0, 0, w_width, w_height);
	glClearColor(0.77f, 0.839f, 0.968f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

}




//-----------------------------------------------------------------------

void lowlevelsys::gl_renderer::render(float deltaTime)
{
	if (global_input_manager->has_key_been_pressed(GLFW_KEY_LEFT_ALT))
	{
		global_input_manager->set_hide_mouse_cursor(true);

		m_camera_manager->m_viewport_camera->add_yaw(global_input_manager->get_mouse_offset_new().x_offset * .02);
		m_camera_manager->m_viewport_camera->add_pitch(global_input_manager->get_mouse_offset_new().y_offset * .02);

		m_camera_manager->m_viewport_camera->add_speed(global_input_manager->get_mouse_scroll_offset() / 50);
		m_camera_manager->m_viewport_camera->set_speed(glm::clamp(m_camera_manager->m_viewport_camera->get_speed(), 0.01f, 2.0f));
		global_input_manager->set_mouse_scroll_offset();

		float speed = m_camera_manager->m_viewport_camera->get_speed();

		if (global_input_manager->has_key_been_pressed(GLFW_KEY_W))
			m_camera_manager->m_viewport_camera->move_forward(speed);

		if (global_input_manager->has_key_been_pressed(GLFW_KEY_S))
			m_camera_manager->m_viewport_camera->move_forward(-speed);

		if (global_input_manager->has_key_been_pressed(GLFW_KEY_D))
			m_camera_manager->m_viewport_camera->move_right(speed);

		if (global_input_manager->has_key_been_pressed(GLFW_KEY_A))
			m_camera_manager->m_viewport_camera->move_right(-speed);

		if (global_input_manager->has_key_been_pressed(GLFW_KEY_E))
			m_camera_manager->m_viewport_camera->move_up(speed);

		if (global_input_manager->has_key_been_pressed(GLFW_KEY_Q))
			m_camera_manager->m_viewport_camera->move_up(-speed);


	}
	else
	{
		global_input_manager->set_hide_mouse_cursor(false);
		global_input_manager->mouse_stop_move();
	}

	if (global_input_manager->has_key_been_pressed(GLFW_KEY_LEFT_CONTROL)) {
		if (global_input_manager->has_key_been_pressed(GLFW_KEY_F)) {
			m_camera_manager->m_viewport_camera->set_position(glm::vec3(0.0));
		}
	}
}




void lowlevelsys::gl_renderer::post_render(float deltaTime)
{
	glfwPollEvents();
	glfwSwapBuffers(window);
}



//-----------------------------------------------------------------------


//-----------------------------------------------------------------------
// I fucking HATE this way of doing this hollyyyy shit
// please someone with a brain help me

void lowlevelsys::gl_renderer::update_draw_data()
{

	if (global_scene->total_component_count() != shader_count || global_scene->m_reloaded) {

		std::vector<pointlight_component*> point_lights;
		std::vector<directionallight_component*> directional_lights;
		std::vector<spotlight_component*> spot_lights;
		std::vector<light_component*> all_lights;
		std::vector<staticmesh_component*> meshes;


		for (auto& i : global_scene->m_entities) {
			for (auto& per_ent : i->m_components_list)
			{
				auto mesh_comps = dynamic_cast<staticmesh_component*>(per_ent);
				if (mesh_comps)
					meshes.push_back(mesh_comps);

				auto light_comps = dynamic_cast<pointlight_component*>(per_ent);
				if (light_comps)
				{
					point_lights.push_back(light_comps);
					all_lights.push_back(light_comps);
				}

				auto directional_comps = dynamic_cast<directionallight_component*>(per_ent);
				if (directional_comps)
				{
					directional_lights.push_back(directional_comps);
					all_lights.push_back(directional_comps);
				}

				auto spotlight_comps = dynamic_cast<spotlight_component*>(per_ent);
				if (spotlight_comps) {

					spot_lights.push_back(spotlight_comps);
					all_lights.push_back(spotlight_comps);
				}


				auto renderable = dynamic_cast<core_renderable*>(per_ent);
				if (renderable) {
					std::vector<vertex_data> cube = {
						vertex_data(glm::vec3(-.4,-.4,-.4)),
						vertex_data(glm::vec3(-.4,-.4,.4)),
						vertex_data(glm::vec3(-.4,.4,-.4)),
						vertex_data(glm::vec3(-.4,.4,.4)),
						vertex_data(glm::vec3(.4,-.4,-.4)),
						vertex_data(glm::vec3(.4,-.4,.4)),
						vertex_data(glm::vec3(.4,.4,-.4)),
						vertex_data(glm::vec3(.4,.4,.4))
					};					
					renderable->init_boundingvolume(cube);
				}

			}
		}


		for (auto& i : meshes) {

			//i->m_material->on_change_material();

			if (i->m_model) {
				i->m_model->m_camera = m_camera_manager->m_crnt_camera;
				if (!i->m_model->has_boundvolume)
				{
					i->m_model->init_boundingvolume(i->m_model->retrieve_all_vertices());
				}
			}
		}
		/*
		* I am a IDIOT
		* This will be updated because luckly for now only one object can be copied so only the last mesh will be updated
		* this is bad because if later there is a way to paste multiple meshes this will only update the last one
		*/
		if (!meshes.empty() && meshes[meshes.size() - 1]->m_model)
			meshes[meshes.size() - 1]->m_model->init_boundingvolume(meshes[meshes.size() - 1]->m_model->retrieve_all_vertices());


		shader_count = global_scene->total_component_count();

	}
}



//-----------------------------------------------------------------------


void lowlevelsys::gl_renderer::on_exit()
{
	m_shdrs.clear();
	glfwTerminate();

}

//-----------------------------------------------------------------------