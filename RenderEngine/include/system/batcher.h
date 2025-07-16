#pragma once
#include "glm/glm.hpp"
#include <vector>
#include "scene_manager.h"


struct MeshGL {
	GLuint vao;
	GLuint vbo;
	GLuint ebo;
	GLsizei indexCount;
};


struct mesh_batch_data {
	std::vector<unsigned int> indices;
	std::vector<vertex_data> vertices;
	glm::mat4 mesh_transform = glm::mat4();
};



class batcher {
public:
	batcher(unsigned int max_vertices, wizm::core_scene* scene);
	~batcher();

	void render(std::shared_ptr<core_gl_shader> m_shader);

	void generate_unified_meshes();

	void generate_gl_mesh_context();

	void bacth();



private:
	std::unordered_map<std::shared_ptr<material_asset>, std::vector<mesh_batch_data>> unified_meshes;
	std::unordered_map<std::shared_ptr<material_asset>, std::vector<MeshGL>> meshGLs;
	wizm::core_scene* global_scene;
	unsigned int m_max_vertices;
};