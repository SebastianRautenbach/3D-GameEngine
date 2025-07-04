#pragma once
#include "layer.h"
#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_glfw.h"
#include "imgui/backends/imgui_impl_opengl3.h"
#include <memory>
#include "other utils/ZER.h"
#include "system/assets/material_asset.h"
#include "system/assets/mesh_asset.h"
#include "entity sys/components/staticmesh_component.h"
#include "entity sys/components/pointlight_component.h"
#include "gl core/vertex_buffer.h"
#include "gl renderer/gl_renderer.h"
#include "system/camera_3d.h"

namespace wizm {

	class asset_manager;

	class material_editor_layer : public core_layer {
	public:
		material_editor_layer(asset_manager* asset_manager);
		~material_editor_layer();

		virtual void on_attach() override;
		virtual void on_detach() override;
		virtual void update(float delta_time) override;


	private:

		float shininess = 1.0;
		float roughness = 1.0;

		std::shared_ptr<material_asset> m_material;
		std::string mat_path;
		asset_manager* m_asset_manager;

		staticmesh_asset sphere;
		staticmesh_component* sm_comp;

		camera_core_3d* m_camera;
		std::shared_ptr<core_gl_shader> m_shader;
		std::string m_mat_ID;

		core_gl_texture* diffuse_image;
		core_gl_texture* sepcular_image;
		core_framebuffer* material_fbo;

		directionallight_component* light;
	};

}