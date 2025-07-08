#include "system/batcher.h"
#include <thread>
#include <future>
#include <mutex>

batcher::batcher(unsigned int max_vertices, wizm::core_scene* scene)
: m_max_vertices(max_vertices * 2), global_scene(scene) {}


void batcher::render(std::shared_ptr<core_gl_shader> m_shader)
{

    if(this)
    {
        m_shader->setMat4("model", glm::mat4(1.0));
        for (auto& [matPtr, glMeshes] : meshGLs) {

        

            if (!matPtr) continue;

            matPtr->apply(m_shader);

            for (auto& glMesh : glMeshes) {
                glBindVertexArray(glMesh.vao);
                glDrawElements(GL_TRIANGLES, glMesh.indexCount, GL_UNSIGNED_INT, 0);
            }

            matPtr->unbind();
        }
    }


}

void batcher::generate_unified_meshes()
{
    std::unordered_map<std::shared_ptr<material_asset>, std::vector<mesh_batch_data>> material_batches;

    for (const auto& entity : global_scene->m_entities) {
        // Store components to be removed later to avoid iterator invalidation
        std::vector<core_component*> components_to_remove;

        for (auto* component : entity->get_components()) {
            auto* sm_comp = dynamic_cast<staticmesh_component*>(component);

            if (sm_comp && sm_comp->is_static) {
                const glm::mat4 applied_trans = sm_comp->get_world_transform();
                const auto& meshes_in_model = sm_comp->m_model->get_mesh()->meshes;
                const auto& material_per_mesh = sm_comp->m_materials;

                for (const auto& mesh : meshes_in_model) {
                    mesh_batch_data mbd;
                    mbd.vertices.reserve(mesh.vertices.size());
                    mbd.indices = mesh.indices;

                    for (const auto& v : mesh.vertices) {
                        vertex_data tv = v;

                        // Apply world transform
                        glm::vec4 worldPos = applied_trans * glm::vec4(v.Position, 1.0f);
                        tv.Position = glm::vec3(worldPos);

                        // Transform normals and tangents
                        glm::mat3 normalMat = glm::transpose(glm::inverse(glm::mat3(applied_trans)));
                        tv.Normal = glm::normalize(normalMat * v.Normal);
                        tv.Tangent = glm::normalize(normalMat * v.Tangent);
                        tv.Bitangent = glm::normalize(normalMat * v.Bitangent);

                        mbd.vertices.push_back(tv);
                    }

                    // Material association
                    if (mesh.m_material_index < material_per_mesh.size()) {
                        const auto& material = material_per_mesh[mesh.m_material_index];
                        material_batches[material].emplace_back(std::move(mbd));
                    }
                }

                components_to_remove.push_back(component);
            }
        }

        // Remove and delete static mesh components after iteration
        for (auto* comp : components_to_remove) {
            auto& components = entity->m_components_list;
            auto it = std::find(components.begin(), components.end(), comp);
            if (it != components.end()) {
                components.erase(it);
                delete comp;
            }
        }
    }





    // Clear unified_meshes and start fresh
    unified_meshes.clear();

    for (auto& [matPtr, batches] : material_batches) {
        std::vector<mesh_batch_data> current_group;
        mesh_batch_data current_mesh;
        unsigned int current_vertex_count = 0;

        for (auto& batch : batches) {
            if (current_vertex_count + batch.vertices.size() > m_max_vertices) {
                // Commit current mesh and reset
                unified_meshes[matPtr].emplace_back(std::move(current_mesh));
                current_mesh = mesh_batch_data();
                current_vertex_count = 0;
            }

            unsigned int baseIndex = static_cast<unsigned int>(current_mesh.vertices.size());

            // Append vertices
            current_mesh.vertices.insert(current_mesh.vertices.end(),
                batch.vertices.begin(), batch.vertices.end());

            // Append indices with offset
            for (auto idx : batch.indices) {
                current_mesh.indices.push_back(idx + baseIndex);
            }

            current_vertex_count += static_cast<unsigned int>(batch.vertices.size());
        }

        // Add any remaining mesh
        if (!current_mesh.vertices.empty()) {
            unified_meshes[matPtr].emplace_back(std::move(current_mesh));
        }
    }
    
}

void batcher::generate_gl_mesh_context()
{
    for (auto& [matPtr, meshes] : unified_meshes) {
        std::vector<MeshGL> glMeshList;

        for (auto& mesh : meshes) {
            MeshGL glMesh;

            glGenVertexArrays(1, &glMesh.vao);
            glGenBuffers(1, &glMesh.vbo);
            glGenBuffers(1, &glMesh.ebo);

            glBindVertexArray(glMesh.vao);

            glBindBuffer(GL_ARRAY_BUFFER, glMesh.vbo);
            glBufferData(
                GL_ARRAY_BUFFER,
                mesh.vertices.size() * sizeof(vertex_data),
                mesh.vertices.data(),
                GL_STATIC_DRAW
            );

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, glMesh.ebo);
            glBufferData(
                GL_ELEMENT_ARRAY_BUFFER,
                mesh.indices.size() * sizeof(unsigned int),
                mesh.indices.data(),
                GL_STATIC_DRAW
            );

            using VD = vertex_data;
            GLsizei stride = sizeof(VD);

            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(VD, Position));

            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(VD, Normal));

            glEnableVertexAttribArray(2);
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(VD, TexCoords));

            glEnableVertexAttribArray(3);
            glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(VD, Tangent));

            glEnableVertexAttribArray(4);
            glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(VD, Bitangent));

            glEnableVertexAttribArray(5);
            glVertexAttribIPointer(5, 4, GL_INT, stride, (void*)offsetof(VD, m_BoneIDs));

            glEnableVertexAttribArray(6);
            glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(VD, m_Weights));

            glBindVertexArray(0);

            glMesh.indexCount = static_cast<GLsizei>(mesh.indices.size());
            glMeshList.push_back(glMesh);
        }

        meshGLs[matPtr] = std::move(glMeshList);
    }
}


void batcher::bacth()
{
    unified_meshes.clear();
    meshGLs.clear();

    generate_unified_meshes();
    generate_gl_mesh_context();

}

batcher::~batcher()
{
    for (auto& [mat, meshes] : meshGLs) {
        for (auto& m : meshes) {
            glDeleteVertexArrays(1, &m.vao);
            glDeleteBuffers(1, &m.vbo);
            glDeleteBuffers(1, &m.ebo);
        }
    }

}

