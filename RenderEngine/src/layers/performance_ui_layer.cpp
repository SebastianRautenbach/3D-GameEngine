#include "layers/performance_ui_layer.h"
#include "scene.h"

wizm::performace_ui_layer::performace_ui_layer(core_scene* scene, core_framebuffer* shadow_fbo)
	:core_layer("performance layer"), global_scene(scene), m_shadow_fbo(shadow_fbo)
{
}

wizm::performace_ui_layer::~performace_ui_layer()
{
}

void wizm::performace_ui_layer::on_attach()
{
}

void wizm::performace_ui_layer::on_detach()
{
}

void wizm::performace_ui_layer::update(float delta_time)
{
	ImGui::Begin("Performance");
	ImGuiIO& io = ImGui::GetIO();
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
	
	
	static unsigned int amm_verts = 0;
	if(ImGui::Button("Get total triangles in scene"))
	{
		amm_verts = 0;
		for (const auto& ent : global_scene->m_entities)
		{
			for (const auto& comp : ent->m_components_list)
			{
				auto staticmesh = dynamic_cast<staticmesh_component*>(comp);
				if (staticmesh)
					amm_verts += staticmesh->m_model->get_triangles();
			}
		}
	}






	std::string tot_vert_str = "total triangles:" + std::to_string(amm_verts);
	ImGui::Text(tot_vert_str.c_str());
	

	static ImVec2 mSize = { 200.0, 200.0 };
	
	ImGui::Image(reinterpret_cast<void*>(m_shadow_fbo->get_depth_attachment_render_id()), ImVec2{ mSize.x, mSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });



	ImGui::End();
}
