#include "update_manager.h"
#include "system/mouse_picking.h"
#include "scene.h"
#include "gl core/engine_shader_types.h"

using namespace wizm;


//-----------------------------------------------------------------------


void update_manager::render_setup(int window_size_x, int window_size_y, const char* window_name)
{

	global_scene = new core_scene;

	m_camera_manager = std::make_shared<camera_manager>(global_scene);

	m_audio_manager = new audio_manager();
	m_listener_manager = new audio_listener(m_camera_manager, m_audio_manager);

	m_gl_renderer = new lowlevelsys::gl_renderer;

	m_gl_renderer->setup(window_size_x, window_size_y, window_name, m_camera_manager, global_scene);

	m_timer = new core_timer;

	framebuffer_spec spec;
	spec.Width = window_size_x;
	spec.Height = window_size_y;
	spec.attachment = { framebuffer_texture_format::RGBA8, framebuffer_texture_format::Depth };
	m_framebuffer = new core_framebuffer(spec);


	framebuffer_spec spec1;
	spec1.Width = SHADOW_WIDTH;
	spec1.Height = SHADOW_HEIGHT;
	spec1.attachment = {framebuffer_texture_format::Depth };
	m_shadowframebuffer = new core_framebuffer(spec1);

	compute_cluster_test = new compute_cluster(m_gl_renderer->m_shdrs, m_gl_renderer->m_shdrs[ENGINE_SHADER_CLUSTER_COMP],
		m_gl_renderer->m_shdrs[ENGINE_SHADER_CLUSTER_CULL], m_camera_manager, global_scene);


	m_asset_manager = new asset_manager(m_audio_manager, global_scene);

	base_layer = new gui_layer(m_gl_renderer->window, m_camera_manager, m_asset_manager, global_scene);

	m_layer_stack = new layer_stack();

	m_layer_stack->push_layer(base_layer);

	m_billboard_manager = new billboard_manager(m_gl_renderer->m_shdrs[ENGINE_SHADER_BILLBOARD], global_scene);

	m_layer_stack->push_layer(new script_debug_layer());
	m_layer_stack->push_layer(new viewport_layer(m_framebuffer, m_camera_manager, m_gl_renderer, m_asset_manager, global_scene));
	m_layer_stack->push_layer(new scene_ui_layer(m_gl_renderer, global_scene));
	m_layer_stack->push_layer(new performace_ui_layer(global_scene));
	m_layer_stack->push_layer(new properties_ui_layer(m_gl_renderer, m_asset_manager, global_scene));
	m_layer_stack->push_layer(new content_browser_layer(m_asset_manager));
	m_layer_stack->push_layer(new material_editor_layer(m_asset_manager));

	m_runtime_manager = std::make_unique<runtime_manager>(global_scene);



	m_gl_renderer->m_shdrs[ENGINE_SHADER_DEFUALT]->use_shader();
	m_gl_renderer->m_shdrs[ENGINE_SHADER_DEFUALT]->setInt("depthMap", 4);

	m_gl_renderer->m_shdrs[ENGINE_SHADER_SHADOW_DEBUG]->use_shader();
	m_gl_renderer->m_shdrs[ENGINE_SHADER_SHADOW_DEBUG]->setInt("depthMap", 4);

}



//-----------------------------------------------------------------------




void update_manager::pre_render()
{
	static bool do_once = false;

	if (!do_once) {
		do_once = true;

		global_scene->read_map_data("GAME/example.zer");
	}


	global_scene->scene_preupdate();
	m_asset_manager->assign_assets();
	m_gl_renderer->update_draw_data();
	compute_cluster_test->update_lights();
	m_gl_renderer->pre_render(is_running, m_timer->get_delta_time());


	// scene reset func required

	if (!global_scene->m_dirty_components.empty())
		global_scene->m_dirty_components.clear();
	global_scene->m_rebuild_lights = false;
}




//-----------------------------------------------------------------------



void update_manager::render()
{

	static bool debug_shadow = true;


	m_timer->update_delta_time();

	m_gl_renderer->render(m_timer->get_delta_time());


	compute_cluster_test->update();


	if(global_scene->m_directional_light)
	{		
		glm::vec3 center_pos = glm::vec3(0.0f);
		lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
		lightView = glm::lookAt(lightPos, center_pos, glm::vec3(0.0f, 1.0f, 0.0f));
		lightSpaceMatrix = lightProjection * lightView;

		m_gl_renderer->m_shdrs[ENGINE_SHADER_SHADOW]->use_shader();
		m_gl_renderer->m_shdrs[ENGINE_SHADER_SHADOW]->setMat4("lightSpaceMatrix", lightSpaceMatrix);

				
		m_shadowframebuffer->bind_buffer();		
		global_scene->scene_update(m_timer->get_delta_time(), m_gl_renderer->m_shdrs[ENGINE_SHADER_SHADOW]);	
		m_shadowframebuffer->unbind_buffer();
		
	}


	if (debug_shadow) {
		glViewport(0, 0, 1920, 1080);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		m_gl_renderer->m_shdrs[ENGINE_SHADER_SHADOW_DEBUG]->use_shader();
		m_gl_renderer->m_shdrs[ENGINE_SHADER_SHADOW_DEBUG]->setFloat("near_plane", near_plane);
		m_gl_renderer->m_shdrs[ENGINE_SHADER_SHADOW_DEBUG]->setFloat("far_plane", far_plane);		
		glActiveTexture(GL_TEXTURE0+4);
		glBindTexture(GL_TEXTURE_2D, m_shadowframebuffer->get_depth_attachment_render_id());
		renderQuad(); 
	} 

	
	if(!debug_shadow)
	{

		// prepass
		{
			glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
			glDepthMask(GL_TRUE);
			glClear(GL_DEPTH_BUFFER_BIT);
			m_gl_renderer->m_shdrs[ENGINE_SHADER_PREPASS]->use_shader();
			global_scene->scene_update(m_timer->get_delta_time(), m_gl_renderer->m_shdrs[ENGINE_SHADER_PREPASS]);
			m_runtime_manager->render_runtime(m_timer->get_delta_time(), m_gl_renderer->m_shdrs[ENGINE_SHADER_PREPASS]);
			glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
		}

		// default
		{
			m_framebuffer->bind_buffer();

			m_gl_renderer->m_shdrs[ENGINE_SHADER_DEFUALT]->use_shader();
			m_gl_renderer->m_shdrs[ENGINE_SHADER_DEFUALT]->setMat4("lightSpaceMatrix", lightSpaceMatrix);

			glActiveTexture(GL_TEXTURE0+4);
			glBindTexture(GL_TEXTURE_2D, m_shadowframebuffer->get_depth_attachment_render_id());			
			
			global_scene->scene_update(m_timer->get_delta_time(), m_gl_renderer->m_shdrs[ENGINE_SHADER_DEFUALT]);
			m_runtime_manager->render_runtime(m_timer->get_delta_time(), m_gl_renderer->m_shdrs[ENGINE_SHADER_DEFUALT]);
			global_scene->update_light_components(m_timer->get_delta_time(), m_gl_renderer->m_shdrs[ENGINE_SHADER_DEFUALT]);
			m_billboard_manager->render();
			m_framebuffer->unbind_buffer();
		} 

		// I want to evolve this to a bigger system but this only handles UI so far
		base_layer->begin();
		for (auto layer = m_layer_stack->begin(); layer != m_layer_stack->end(); layer++)
			(*layer)->update(m_timer->get_delta_time());
		base_layer->end();
	}


	m_audio_manager->update();
	m_listener_manager->listener_on_update(m_timer->get_delta_time());

}


//-----------------------------------------------------------------------



void update_manager::post_render()
{

	global_scene->scene_postupdate();

	m_gl_renderer->post_render(m_timer->get_delta_time());

}

wizm::update_manager::~update_manager()
{
	m_gl_renderer->on_exit();
	delete m_layer_stack;
	delete m_billboard_manager;
	delete m_listener_manager;
	delete m_audio_manager;
	delete compute_cluster_test;
	delete m_gl_renderer;
	delete m_timer;
	delete m_framebuffer;
	delete m_asset_manager;
	delete global_scene;
}


//-----------------------------------------------------------------------