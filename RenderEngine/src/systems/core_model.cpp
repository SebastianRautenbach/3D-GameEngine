#include "system/core_model.h"

lowlevelsys::core_model::core_model(const char* file_path)
{
    loadModel(file_path);
}

void lowlevelsys::core_model::render_model()
{
	for (unsigned int i = 0; i < meshes.size(); i++)
		meshes[i].draw_mesh();
}


void lowlevelsys::core_model::loadModel(std::string path)
{
    Assimp::Importer import;
    const aiScene * scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
        return;
    }
    directory = path.substr(0, path.find_last_of('/'));

    processNode(scene->mRootNode, scene);
    m_num_materials = scene->mNumMaterials;
    
}

void lowlevelsys::core_model::processNode(aiNode* node, const aiScene* scene)
{
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(mesh, scene));
    }
  
    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene);
    }
}

lowlevelsys::core_mesh lowlevelsys::core_model::processMesh(aiMesh* mesh, const aiScene* scene)
{
    std::vector<vertex_data> vertices;
    std::vector<unsigned int> indices;
    std::vector<core_gl_texture> textures;

    total_triangles += mesh->mNumFaces;
    total_vertices += mesh->mNumVertices;

    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        vertex_data vertex;
        glm::vec3 vector; 
        // positions
        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.Position = vector;
        // normals
        if (mesh->HasNormals())
        {
            vector.x = mesh->mNormals[i].x;
            vector.y = mesh->mNormals[i].y;
            vector.z = mesh->mNormals[i].z;
            vertex.Normal = vector;
        }
        // texture coordinates
        if (mesh->mTextureCoords[0]) 
        {
            glm::vec2 vec;
            
            vec.x = mesh->mTextureCoords[0][i].x;
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.TexCoords = vec;
     

            if(mesh->HasTangentsAndBitangents())
            {
                vector.x = mesh->mTangents[i].x;
                vector.y = mesh->mTangents[i].y;
                vector.z = mesh->mTangents[i].z;
                vertex.Tangent = vector;
             
                vector.x = mesh->mBitangents[i].x;
                vector.y = mesh->mBitangents[i].y;
                vector.z = mesh->mBitangents[i].z;
                vertex.Bitangent = vector;
            }
        }
        else
            vertex.TexCoords = glm::vec2(0.0f, 0.0f);
        
        vertices.push_back(vertex);
    }
 
    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        // retrieve all indices of the face and store them in the indices vector
        for (unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }


     
    for (unsigned int i = 0; i < scene->mNumMaterials; ++i) {
        int textureChannelCount = 0;
        aiMaterial* material = scene->mMaterials[i];
        for (int type = aiTextureType_NONE; type <= aiTextureType_UNKNOWN; ++type) {
            if (material->GetTextureCount(static_cast<aiTextureType>(type)) > 0) {
                textureChannelCount++;
            }
        }
        // ----- mat updates
    }


    return core_mesh(vertices, indices, mesh->mMaterialIndex);
}



std::vector<core_gl_texture> lowlevelsys::core_model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
{
    std::vector<core_gl_texture> textures;
    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
        std::cout << "texture count:" << typeName << "\n";
    }
    //for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
    //{
    //    aiString str;
    //    mat->GetTexture(type, i, &str);
    //    // check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
    //    bool skip = false;
    //    for (unsigned int j = 0; j < textures_loaded.size(); j++)
    //    {
    //        if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0)
    //        {
    //            textures.push_back(textures_loaded[j]);
    //            skip = true; // a texture with the same filepath has already been loaded, continue to next one. (optimization)
    //            break;
    //        }
    //    }
    //    if (!skip)
    //    {   // if texture hasn't been loaded already, load it
    //        core_gl_texture texture(str.C_Str(), this->directory);
    //        texture.type = typeName;
    //        texture.path = str.C_Str();
    //        textures.push_back(texture);
    //        textures_loaded.push_back(texture);  // store it as texture loaded for entire model, to ensure we won't unnecessary load duplicate textures.
    //    }
    //}
    return textures;
}
