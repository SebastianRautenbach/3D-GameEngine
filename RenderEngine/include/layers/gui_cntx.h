#pragma once
#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_glfw.h"
#include "imgui/backends/imgui_impl_opengl3.h"
#include "system/camera_manager.h"
#include "layer.h"
#include "ImGuizmo/ImGuizmo.h"
#include "system/asset_manager.h"


namespace wizm {
	class core_scene;

	class gui_layer : public core_layer
	{
	public:
		gui_layer(GLFWwindow* window, std::shared_ptr<camera_manager> camera_manager, asset_manager* p_asset_manager);
		~gui_layer();

		virtual void on_attach() override;
		virtual void on_detach() override;


		void begin();
		void end();

	private:
		ImVec4 rgbaToVec4(float r, float g, float b, float a);

	private:
		ImVec4 winCol = rgbaToVec4(0, 0, 0, 230),
			 bgCol = rgbaToVec4(31, 30, 31, 255),
			 childCol = rgbaToVec4(33, 34, 45, 255),
			 childCol1 = rgbaToVec4(38, 39, 50, 255),
			 notSelectedTextColor = rgbaToVec4(140, 140, 140, 255),
			 textCol = rgbaToVec4(255, 255, 255, 255),
			 btnActiveCol = rgbaToVec4(239, 73, 88, 255),
			 btnHoverCol = rgbaToVec4(173, 55, 65, 255),

			 frameCol = rgbaToVec4(44, 43, 44, 255),
			 hoverCol = rgbaToVec4(37, 36, 37, 255),
			 itemCol = rgbaToVec4(240, 74, 88, 255),
			 itemActiveCol = rgbaToVec4(240, 50, 66, 255),
			 resizeGripCol = rgbaToVec4(220, 50, 66, 120),
			 resizeGripHoverCol = rgbaToVec4(250, 50, 66, 140);
		
	
	private:
		bool show_demo_window = true;
		ImVec2 m_window_size;
		GLFWwindow* m_window;
		std::shared_ptr<camera_manager> m_camera_manager;
		asset_manager* m_asset_manager;
	};


}