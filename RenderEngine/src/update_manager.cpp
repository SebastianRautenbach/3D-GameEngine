#include "update_manager.h"
#include "system/mouse_picking.h"
#include "system/scene_manager.h"
#include "gl core/engine_shader_types.h"

using namespace wizm;


//-----------------------------------------------------------------------


void update_manager::render_setup(int window_size_x, int window_size_y, const char* window_name)
{
	
	global_scene = new core_scene;

	m_camera_manager = std::make_shared<camera_manager>();

	m_audio_manager = new audio_manager();
	m_listener_manager = new audio_listener(m_camera_manager, m_audio_manager);

	m_gl_renderer = new lowlevelsys::gl_renderer;

	m_gl_renderer->setup(window_size_x, window_size_y, window_name, m_camera_manager);

	m_timer = new core_timer;

	framebuffer_spec spec;
	spec.Width = window_size_x;
	spec.Height = window_size_y;
	spec.attachment = { framebuffer_texture_format::RGBA8, framebuffer_texture_format::Depth };
	m_framebuffer = new core_framebuffer(spec);

	compute_cluster_test = new compute_cluster(m_gl_renderer->m_shdrs, m_gl_renderer->m_shdrs[ENGINE_SHADER_CLUSTER_COMP], m_gl_renderer->m_shdrs[ENGINE_SHADER_CLUSTER_CULL],  m_camera_manager);
	

	m_asset_manager = new asset_manager(m_audio_manager);

	base_layer = new gui_layer(m_gl_renderer->window, m_camera_manager, m_asset_manager);

	m_layer_stack = new layer_stack();

	m_layer_stack->push_layer(base_layer);

	m_billboard_manager = new billboard_manager(m_gl_renderer->m_shdrs[ENGINE_SHADER_BILLBOARD]);

	m_layer_stack->push_layer(new script_debug_layer());
	m_layer_stack->push_layer(new viewport_layer(m_framebuffer, m_camera_manager, m_gl_renderer, m_asset_manager));
	m_layer_stack->push_layer(new scene_ui_layer(m_gl_renderer));
	m_layer_stack->push_layer(new performace_ui_layer());
	m_layer_stack->push_layer(new properties_ui_layer(m_gl_renderer, m_asset_manager));
	m_layer_stack->push_layer(new content_browser_layer(m_asset_manager));
	m_layer_stack->push_layer(new material_editor_layer(m_asset_manager));

	m_runtime_manager = std::make_unique<runtime_manager>();
	
}



//-----------------------------------------------------------------------




void update_manager::pre_render()
{
	global_scene->scene_preupdate();
	m_asset_manager->assign_assets();
	m_gl_renderer->update_draw_data();
	compute_cluster_test->update_lights();
	m_gl_renderer->pre_render(is_running, m_timer->get_delta_time());
}




//-----------------------------------------------------------------------



void update_manager::render()
{

	m_timer->update_delta_time();

    m_gl_renderer->render(m_timer->get_delta_time());
	
	compute_cluster_test->update();


	
	{
		m_framebuffer->bind_buffer();
		m_gl_renderer->m_shdrs[ENGINE_SHADER_DEFUALT]->use_shader();
		global_scene->scene_update(m_timer->get_delta_time(), m_gl_renderer->m_shdrs[ENGINE_SHADER_DEFUALT]);		
		m_runtime_manager->render_runtime(m_timer->get_delta_time(), m_gl_renderer->m_shdrs[ENGINE_SHADER_DEFUALT]);
		global_scene->update_light_components(m_timer->get_delta_time(), m_gl_renderer->m_shdrs[ENGINE_SHADER_DEFUALT]);
		m_billboard_manager->render();
		m_framebuffer->unbind_buffer();
	}

	

	m_audio_manager->update();
	m_listener_manager->listener_on_update(m_timer->get_delta_time());

	// I want to evolve this to a bigger system but this only handles UI so far
	base_layer->begin();
	for (auto layer = m_layer_stack->begin(); layer != m_layer_stack->end(); layer++)
		(*layer)->update(m_timer->get_delta_time());
	base_layer->end();
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