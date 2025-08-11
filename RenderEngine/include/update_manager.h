#pragma once
#include <iostream>
#include "gl renderer/gl_renderer.h"
#include "system/timer.h"
#include "input.h"
#include "system/asset_manager.h"
#include "layers/gui_cntx.h"
#include "layers/layerstack.h"
#include "layers/layers_define.h"
#include "system/billboard_manager.h"
#include "system/camera_manager.h"
#include "system/compute_cluster_test.h"
#include "system/audio_listener.h"
#include "system/batcher.h"
#include "system/runtimemanager.h"


// DELETE
#include "system/draw_ray.h"


namespace wizm {

	class core_scene;

	class update_manager {
	public:

		void render_setup(int window_size_x, int window_size_y, const char* window_name);
		void pre_render();
		void render();
		void post_render();

		~update_manager();


	public:
		bool is_running = true;
		lowlevelsys::gl_renderer* m_gl_renderer;
		core_timer* m_timer;
		gui_layer* base_layer;

		core_framebuffer* m_framebuffer;

		compute_cluster* compute_cluster_test;
		
		std::unique_ptr<runtime_manager> m_runtime_manager;

		layer_stack* m_layer_stack;
		billboard_manager* m_billboard_manager;
		std::shared_ptr<camera_manager> m_camera_manager;
		asset_manager* m_asset_manager;

		// SOUND

		audio_listener* m_listener_manager;
		audio_manager* m_audio_manager;

		// SCENE
		core_scene* global_scene;

		// DELETE
		core_framebuffer* m_shadowframebuffer;
		const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
		unsigned int depthMapFBO;
		unsigned int depthMap;

		glm::vec3 lightPos = glm::vec3(-2.0f, 4.0f, -1.0f);
		glm::mat4 lightProjection, lightView;
		glm::mat4 lightSpaceMatrix;

		float near_plane = 1.0f, far_plane = 200.5f;

		unsigned int quadVAO = 0;
		unsigned int quadVBO;

		void renderQuad()
		{
			if (quadVAO == 0)
			{
				float quadVertices[] = {
					// positions        // texture Coords
					-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
					-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
					 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
					 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
				};
				// setup plane VAO
				glGenVertexArrays(1, &quadVAO);
				glGenBuffers(1, &quadVBO);
				glBindVertexArray(quadVAO);
				glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
				glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
				glEnableVertexAttribArray(0);
				glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
				glEnableVertexAttribArray(1);
				glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
			}
			glBindVertexArray(quadVAO);
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
			glBindVertexArray(0);
		}



		
	};


}