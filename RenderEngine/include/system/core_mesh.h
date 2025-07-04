#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <string>
#include <gl core/core shader.h>
#include "gl core/vertex_buffer.h"



namespace lowlevelsys {
	class core_mesh {
	
	public:
		core_mesh(std::vector<vertex_data> vertices, std::vector<unsigned int> indices, std::vector<core_gl_texture> textures, int material_index);
		core_mesh(core_mesh&& other) noexcept;
		
		void draw_mesh();
		~core_mesh();


		core_mesh& operator=(core_mesh&& other) noexcept;

	
	public:
		std::vector<vertex_data> vertices;
		std::vector<unsigned int> indices;		
		int m_material_index;

	private:
		core_arr_vertex_buffer vertex_arr;
		void setup_mesh();
	};
}