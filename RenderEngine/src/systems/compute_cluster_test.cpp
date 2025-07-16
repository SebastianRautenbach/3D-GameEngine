#include "system/compute_cluster_test.h"
#include "system/camera_3d.h"
#include "scene.h"

wizm::compute_cluster::compute_cluster(std::map<int, std::shared_ptr<core_gl_shader>>& shaders, std::shared_ptr<core_gl_shader>& com_shader_cluster, 
	std::shared_ptr<core_gl_shader>& com_shader_cull, std::shared_ptr<camera_manager> camera_manager, core_scene* scene)
	: m_shader_cluster(com_shader_cluster), m_shader_cull(com_shader_cull), m_camera_manager(camera_manager), m_shaders(shaders), global_scene(scene)
{
	glGenBuffers(1, &clusterGridSSBO);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, clusterGridSSBO);

	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(Cluster) * numClusters, nullptr, GL_STATIC_COPY);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, clusterGridSSBO);
}

void wizm::compute_cluster::update_lights()
{	

	static std::vector<PointLight> point_lights_list;
	static std::vector<SpotLight> spot_lights_list;


	if(global_scene->m_rebuild_lights) {
		m_point_lights.clear();
		m_spot_lights.clear();
		point_lights_list.clear();
		spot_lights_list.clear();


		if (pointLightSSBO || spotLightSSBO) {
			glDeleteBuffers(1, &pointLightSSBO);
			pointLightSSBO = 0;
			glDeleteBuffers(1, &spotLightSSBO);
			spotLightSSBO = 0;
		}

		// Rebuild the list of light indexes (n^2) very slow operatation
		for (unsigned int ent_i = 0; ent_i < global_scene->m_entities.size(); ent_i++) {
			for (unsigned int comp_i = 0; comp_i < global_scene->m_entities[ent_i]->m_components_list.size(); comp_i++) {
				if (global_scene->m_entities[ent_i]->m_components_list[comp_i]->m_component_type == ePointLight) {					
					auto component = dynamic_cast<pointlight_component*>(global_scene->m_entities[ent_i]->m_components_list[comp_i]);
					m_point_lights.emplace_back(component);
				}
				else if (global_scene->m_entities[ent_i]->m_components_list[comp_i]->m_component_type == eSpotLight) {
					auto component = dynamic_cast<spotlight_component*>(global_scene->m_entities[ent_i]->m_components_list[comp_i]);
					m_spot_lights.emplace_back(component);
				}
			}
		}

		point_lights_list.reserve(m_point_lights.size());
		spot_lights_list.reserve(m_spot_lights.size());

		for (auto& pointlight : m_point_lights) {
			if (pointlight) {
				PointLight temp;
				temp.position = glm::vec4(pointlight->get_world_position(), 1.0f);
				temp.color = glm::vec4(pointlight->m_diffuse, 1.0f);
				temp.intensity = pointlight->m_intensity;
				temp.radius = pointlight->m_radius;
				temp.pad1 = temp.pad2 = 0;
				point_lights_list.emplace_back(temp);
			}
		}

		for (auto& spotlight : m_spot_lights) {
			if (spotlight) {
				SpotLight temp;
				temp.position = glm::vec4(spotlight->get_world_position(), 1.0f);

				glm::vec3 world_rotation = spotlight->get_world_rotation();
				glm::quat rotation_quat = glm::quat(glm::vec3(glm::radians(world_rotation.x),
					glm::radians(world_rotation.y), glm::radians(world_rotation.z)));
				glm::vec3 rotated_direction = glm::normalize(rotation_quat * glm::vec3(0.0f, -1.0f, 0.0f));

				temp.direction = glm::vec4(rotated_direction, 1.0f);
				temp.cutOff = spotlight->m_cutOff;
				temp.outerCutOff = spotlight->m_outerCutOff;
				temp.distance = spotlight->m_distance;
				temp.constant = spotlight->m_constant;
				temp.quadratic = spotlight->m_quadratic;
				temp.ambient = glm::vec4(spotlight->m_ambient, 1.0f);
				temp.diffuse = glm::vec4(spotlight->m_diffuse, 1.0f);
				temp.specular = glm::vec4(spotlight->m_specular, 1.0f);
				temp.linear = spotlight->m_linear;
				spot_lights_list.emplace_back(temp);
			}
		}

		m_shader_cull->use_shader();		
		m_shader_cull->setUInt("pointlightCount", m_point_lights.size());
		m_shader_cull->setUInt("spotlightCount", m_spot_lights.size());

		current_point_light_buffer_size = 0;
		current_spot_light_buffer_size = 0;
	}

	

	// Gather point light data

	for (int i = 0; i < point_lights_list.size(); i++) {
		point_lights_list[i].position = glm::vec4(m_point_lights[i]->get_world_position(), 1.0f);
		point_lights_list[i].color = glm::vec4(m_point_lights[i]->m_diffuse, 1.0f);
		point_lights_list[i].intensity = m_point_lights[i]->m_intensity;
		point_lights_list[i].radius = m_point_lights[i]->m_radius;
	}

	
	/*
		We want to update all shaders with updated light info (if the camera moved or any light state change), this is done per frame
	*/

	for (int i = 0; i < spot_lights_list.size(); i++) {
		spot_lights_list[i].position = glm::vec4(m_spot_lights[i]->get_world_position(), 1.0f);

		glm::vec3 world_rotation = m_spot_lights[i]->get_world_rotation();
		glm::quat rotation_quat = glm::quat(glm::vec3(glm::radians(world_rotation.x),
			glm::radians(world_rotation.y), glm::radians(world_rotation.z)));
		glm::vec3 rotated_direction = glm::normalize(rotation_quat * glm::vec3(0.0f, -1.0f, 0.0f));

		spot_lights_list[i].direction = glm::vec4(rotated_direction, 1.0f);
		spot_lights_list[i].cutOff = m_spot_lights[i]->m_cutOff;
		spot_lights_list[i].outerCutOff = m_spot_lights[i]->m_outerCutOff;
		spot_lights_list[i].distance = m_spot_lights[i]->m_distance;
		spot_lights_list[i].constant = m_spot_lights[i]->m_constant;
		spot_lights_list[i].quadratic = m_spot_lights[i]->m_quadratic;
		spot_lights_list[i].ambient = glm::vec4(m_spot_lights[i]->m_ambient, 1.0f);
		spot_lights_list[i].diffuse = glm::vec4(m_spot_lights[i]->m_diffuse, 1.0f);
		spot_lights_list[i].specular = glm::vec4(m_spot_lights[i]->m_specular, 1.0f);
		spot_lights_list[i].linear = m_spot_lights[i]->m_linear;
	}


	int point_light_buffer_size = point_lights_list.size() * sizeof(PointLight);
	if (pointLightSSBO == 0) {
		glGenBuffers(1, &pointLightSSBO);
	}

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, pointLightSSBO);

	

	if (current_point_light_buffer_size != static_cast<GLint>(point_light_buffer_size) || point_lights_list.empty()) {
		if (point_lights_list.empty()) {			
			glGetBufferParameteriv(GL_SHADER_STORAGE_BUFFER, GL_BUFFER_SIZE, &current_point_light_buffer_size);
			glBufferData(GL_SHADER_STORAGE_BUFFER, 0, nullptr, GL_DYNAMIC_DRAW);
		}
		else {
			glGetBufferParameteriv(GL_SHADER_STORAGE_BUFFER, GL_BUFFER_SIZE, &current_point_light_buffer_size);
			glBufferData(GL_SHADER_STORAGE_BUFFER, point_light_buffer_size, point_lights_list.data(), GL_DYNAMIC_DRAW);
		}
	}
	else {
		glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, point_light_buffer_size, point_lights_list.data());
	}

	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, pointLightSSBO);


	//--------------------------------------------------------------------------------------------------------------------------------- SPOTLIGHT

	int spot_light_buffer_size = spot_lights_list.size() * sizeof(SpotLight);
	if (spotLightSSBO == 0) {
		glGenBuffers(1, &spotLightSSBO);
	}
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, spotLightSSBO);
	



	if (current_spot_light_buffer_size != static_cast<GLint>(spot_light_buffer_size) || spot_lights_list.empty()) {
		if (spot_lights_list.empty()) {
			glGetBufferParameteriv(GL_SHADER_STORAGE_BUFFER, GL_BUFFER_SIZE, &current_spot_light_buffer_size);
			glBufferData(GL_SHADER_STORAGE_BUFFER, 0, nullptr, GL_DYNAMIC_DRAW);
		}
		else {
			glGetBufferParameteriv(GL_SHADER_STORAGE_BUFFER, GL_BUFFER_SIZE, &current_spot_light_buffer_size);
			glBufferData(GL_SHADER_STORAGE_BUFFER, spot_light_buffer_size, spot_lights_list.data(), GL_DYNAMIC_DRAW);
		}
	}
	else {
		glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, spot_light_buffer_size, spot_lights_list.data());
	}

	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, spotLightSSBO);


	//---------------------------------------------------------------------------------------------------------------------------------

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

void wizm::compute_cluster::update()
{	
	const auto& crnt_camera = m_camera_manager->m_crnt_camera;
	glm::mat4 view = crnt_camera->get_view_matrix();
	glm::mat4 projection = crnt_camera->get_projection_matrix();
	glm::mat4 inverseProjection = glm::inverse(projection);
	glm::uvec2 screenDimensions = crnt_camera->get_window_size();
	float zNear = crnt_camera->get_near();
	float zFar = crnt_camera->get_far();
	glm::vec3 camPos = crnt_camera->get_position();
	glm::vec3 camFront = crnt_camera->get_forward_vector();

	// Configure non-compute shaders
	for (auto& shader : m_shaders) {
		if (!shader.second->is_compute) {
			shader.second->use_shader();
			shader.second->setMat4("view", view);
			shader.second->setMat4("projection", projection);
			shader.second->setVec3("camPos", camPos);
			shader.second->setVec3("camFront", camFront);

			shader.second->setFloat("zNear", zNear);
			shader.second->setFloat("zFar", zFar);

			shader.second->setUVec3("gridSize", { gridSizeX, gridSizeY, gridSizeZ });
			shader.second->setUVec2("screenDimensions", screenDimensions);
		}
	}

	// Configure and dispatch cluster update shader
	m_shader_cluster->use_shader();
	m_shader_cluster->setFloat("zNear", zNear);
	m_shader_cluster->setFloat("zFar", zFar);
	m_shader_cluster->setMat4("inverseProjection", inverseProjection);
	m_shader_cluster->setUVec3("gridSize", { gridSizeX, gridSizeY, gridSizeZ });
	m_shader_cluster->setUVec2("screenDimensions", screenDimensions);

	glDispatchCompute(gridSizeX, gridSizeY, gridSizeZ);
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

	// Configure and dispatch culling shader
	m_shader_cull->use_shader();
	m_shader_cull->setMat4("viewMatrix", view);

	glDispatchCompute(27, 1, 1);
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
}
