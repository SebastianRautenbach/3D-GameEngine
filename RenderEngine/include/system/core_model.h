#pragma once
#include "gl core/core shader.h"
#include "core_mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace lowlevelsys {
	class core_model {
	public:
		core_model(const char* file_path);
		void render_model();
		void loadModel(std::string path);

	public:
		std::vector<core_gl_texture> textures_loaded;
		std::vector<core_mesh>    meshes;
		std::string directory;
		bool gammaCorrection;
		unsigned int total_triangles = 0;
		unsigned int total_vertices = 0;
		int m_num_materials = 0;

	private:

		void processNode(aiNode* node, const aiScene* scene);
		core_mesh processMesh(aiMesh* mesh, const aiScene* scene);
		std::vector<core_gl_texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type,
			std::string typeName);
	};
}