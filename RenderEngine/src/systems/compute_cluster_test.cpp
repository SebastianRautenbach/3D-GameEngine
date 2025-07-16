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
	static std::vector<std::pair<unsigned int, unsigned int>> pointlight_indexes, spotlight_indexes;
	static size_t previous_shader_count = 0;


	if(global_scene->m_rebuild_lights) {
		pointlight_indexes.clear();
		spotlight_indexes.clear();

		previous_shader_count = global_scene->total_component_count();

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
					pointlight_indexes.emplace_back(ent_i, comp_i);
				}
				else if (global_scene->m_entities[ent_i]->m_components_list[comp_i]->m_component_type == eSpotLight) {
					spotlight_indexes.emplace_back(ent_i, comp_i);
				}
			}
		}
	}


	std::vector<PointLight> point_lights_list;
	std::vector<SpotLight> spot_lights_list;

	point_lights_list.reserve(pointlight_indexes.size());
	spot_lights_list.reserve(spotlight_indexes.size());

	// Gather point light data
	for (auto& index_pair : pointlight_indexes) {
		
		const auto& [ent_i, comp_i] = index_pair;
		
		auto light_comps = dynamic_cast<pointlight_component*>(
			global_scene->m_entities[ent_i]->m_components_list[comp_i]);

		if (light_comps) {
			PointLight temp;
			temp.position = glm::vec4(light_comps->get_world_position(), 1.0f);
			temp.color = glm::vec4(light_comps->m_diffuse, 1.0f);
			temp.intensity = light_comps->m_intensity;
			temp.radius = light_comps->m_radius;
			temp.pad1 = temp.pad2 = 0;
			point_lights_list.emplace_back(temp);
		}


	}
	
	/*
		We want to update all shaders with updated light info (if the camera moved or any light state change), this is done per frame
	*/


	for (auto& index_pair : spotlight_indexes) {

		const auto& [ent_i, comp_i] = index_pair;
	
		auto spotlight_comp = dynamic_cast<spotlight_component*>(
			global_scene->m_entities[ent_i]->m_components_list[comp_i]);

		if (spotlight_comp) {
			SpotLight temp;
			temp.position = glm::vec4(spotlight_comp->get_world_position(), 1.0f);

			glm::vec3 world_rotation = spotlight_comp->get_world_rotation();
			glm::quat rotation_quat = glm::quat(glm::vec3(glm::radians(world_rotation.x), 
				glm::radians(world_rotation.y), glm::radians(world_rotation.z)));
			glm::vec3 rotated_direction = glm::normalize(rotation_quat * glm::vec3(0.0f, -1.0f, 0.0f));
			
			temp.direction = glm::vec4(rotated_direction, 1.0f);
			temp.cutOff = spotlight_comp->m_cutOff;
			temp.outerCutOff = spotlight_comp->m_outerCutOff;
			temp.distance = spotlight_comp->m_distance;
			temp.constant = spotlight_comp->m_constant;
			temp.quadratic = spotlight_comp->m_quadratic;
			temp.ambient = glm::vec4(spotlight_comp->m_ambient, 1.0f);
			temp.diffuse = glm::vec4(spotlight_comp->m_diffuse, 1.0f);
			temp.specular = glm::vec4(spotlight_comp->m_specular, 1.0f);
			temp.linear = spotlight_comp->m_linear;
			spot_lights_list.emplace_back(temp);
		}
	}


	int buffer_size = point_lights_list.size() * sizeof(PointLight);
	if (pointLightSSBO == 0) {
		glGenBuffers(1, &pointLightSSBO);
	}

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, pointLightSSBO);


	GLint current_buffer_size = 0;
	glGetBufferParameteriv(GL_SHADER_STORAGE_BUFFER, GL_BUFFER_SIZE, &current_buffer_size);

	if (current_buffer_size != static_cast<GLint>(buffer_size) || point_lights_list.empty()) {
		if (point_lights_list.empty()) {			
			glBufferData(GL_SHADER_STORAGE_BUFFER, 0, nullptr, GL_DYNAMIC_DRAW);
		}
		else {
			glBufferData(GL_SHADER_STORAGE_BUFFER, buffer_size, point_lights_list.data(), GL_DYNAMIC_DRAW);
		}
	}
	else {
		glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, buffer_size, point_lights_list.data());
	}

	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, pointLightSSBO);

	auto packet_test = point_lights_list.data();

	//--------------------------------------------------------------------------------------------------------------------------------- SPOTLIGHT

	int buffer_sizespt = spot_lights_list.size() * sizeof(SpotLight);
	if (spotLightSSBO == 0) {
		glGenBuffers(1, &spotLightSSBO);
	}
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, spotLightSSBO);
	
	GLint current_buffer_sizespt = 0;

	glGetBufferParameteriv(GL_SHADER_STORAGE_BUFFER, GL_BUFFER_SIZE, &current_buffer_sizespt);


	if (current_buffer_sizespt != static_cast<GLint>(buffer_sizespt) || spot_lights_list.empty()) {
		if (spot_lights_list.empty()) {
			glBufferData(GL_SHADER_STORAGE_BUFFER, 1, nullptr, GL_DYNAMIC_DRAW);
		}
		else {
			glBufferData(GL_SHADER_STORAGE_BUFFER, buffer_sizespt, spot_lights_list.data(), GL_DYNAMIC_DRAW);
		}
	}
	else {
		glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, buffer_sizespt, spot_lights_list.data());
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
