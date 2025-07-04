#pragma once
#include "layer.h"
#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_glfw.h"
#include "imgui/backends/imgui_impl_opengl3.h"
#include "system/asset_manager.h"

namespace wizm {

	class asset_manager;

	class asset_viewer_layer : public core_layer {
	public:
		asset_viewer_layer(asset_manager* p_asset_manager);
		~asset_viewer_layer();

		virtual void on_attach() override;
		virtual void on_detach() override;
		virtual void update(float delta_time) override;

	private:
		std::vector<asset_details> m_assets;
		asset_manager* m_asset_manager;
	};

}