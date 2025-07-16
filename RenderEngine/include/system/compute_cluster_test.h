#pragma once
#include "gl core/core shader.h"
#include <memory>
#include "gl core/vertex_buffer.h"
#include "entity sys/components/all_components_list.h"
#include "system/scene_manager.h"
#include "camera_manager.h"

using namespace lowlevelsys;


namespace wizm {

	class core_scene;


	struct alignas(16) Cluster
	{
		glm::vec4 minPoint;
		glm::vec4 maxPoint;
		unsigned int pointcount;
		unsigned int spotcount;
		unsigned int pad1;
		unsigned int pad2;
		unsigned int pointLightIndices[100];
		unsigned int spotLightIndices[100];
	};


	struct alignas(16) PointLight {

		glm::vec4 position;
		glm::vec4 color;


		float intensity;  
		float radius;	  
		
		unsigned int pad1;
		unsigned int pad2;
	};


	struct alignas(16) SpotLight
	{
		glm::vec4 position;
		glm::vec4 direction;
		glm::vec4 ambient;
		glm::vec4 diffuse;
		glm::vec4 specular;

		float cutOff;
		float outerCutOff;
		float distance;
		float constant;

		float linear;
		float quadratic;
		unsigned int pad1;
		unsigned int pad2;
	};



	class compute_cluster {
	public:
		compute_cluster(std::map<int, std::shared_ptr<core_gl_shader>>& shaders, 
			std::shared_ptr<core_gl_shader>& com_shader_cluster, std::shared_ptr<core_gl_shader>& com_shader_cull, 
			std::shared_ptr<camera_manager> camera_manager, core_scene* scene);

	public:

		void update_lights();

		void update();

	private:

		core_scene* global_scene;

		std::map<int, std::shared_ptr<core_gl_shader>>& m_shaders;
		
		std::shared_ptr<core_gl_shader> m_shader_cull;
		std::shared_ptr<core_gl_shader> m_shader_cluster;
		std::shared_ptr<camera_manager> m_camera_manager;

		unsigned int clusterGridSSBO, pointLightSSBO, spotLightSSBO, shader_count = 0;


		
		unsigned int gridSizeX = 16;
		unsigned int gridSizeY = 8;
		unsigned int gridSizeZ = 24;
		unsigned int numClusters = gridSizeX * gridSizeY * gridSizeZ;


		



	};

}