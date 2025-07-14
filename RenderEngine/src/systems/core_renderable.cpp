#include "system/core_renderable.h"
#include <array>
#include <iostream>



void wizm::core_renderable::init_boundingvolume(std::vector<vertex_data> vertices)
{          
    this->has_boundvolume = true;

    min_point_local = vertices[0].Position;
    max_point_local = vertices[0].Position;

    for (const auto& vertex : vertices) {
        min_point_local = glm::min(min_point_local, vertex.Position);
        max_point_local = glm::max(max_point_local, vertex.Position);
    }
    center = get_center();
    init_extents = extents = (max_point_local - min_point_local) * 1.f;
    init_axes[0] = axes[0] = glm::vec3(1, 0, 0);
    init_axes[1] = axes[1] = glm::vec3(0, 1, 0);
    init_axes[2] = axes[2] = glm::vec3(0, 0, 1);

}


void wizm::core_renderable::update_boundingvolume(const glm::vec3& world_pos, const glm::vec3& world_rot, const glm::vec3& world_scale)
{
    glm::vec3 scaled_extents = init_extents * world_scale;

   
    glm::vec3 axis0 = init_axes[0];
    glm::vec3 axis1 = init_axes[1];
    glm::vec3 axis2 = init_axes[2];

    
    glm::mat4 rotation_matrix = glm::rotate(world_rot.z, glm::vec3(0, 0, 1)) *  
        glm::rotate(world_rot.y, glm::vec3(0, 1, 0)) *  
        glm::rotate(world_rot.x, glm::vec3(1, 0, 0)); 

    axes[0] = glm::vec3(rotation_matrix * glm::vec4(axis0, 0.0));
    axes[1] = glm::vec3(rotation_matrix * glm::vec4(axis1, 0.0));
    axes[2] = glm::vec3(rotation_matrix * glm::vec4(axis2, 0.0));


    center = world_pos;
    extents = scaled_extents;

}


/*
    Might aproximate if there will be collision at all between two
    bounding volumes
*/

void wizm::core_renderable::update_boundingvolume(const glm::mat4& model_matrix)
{
   glm::mat4 transform = model_matrix;
   
   glm::vec3 corners[8] = {
        glm::vec3(min_point_local.x, min_point_local.y, min_point_local.z),
        glm::vec3(min_point_local.x, min_point_local.y, max_point_local.z),
        glm::vec3(min_point_local.x, max_point_local.y, min_point_local.z),
        glm::vec3(min_point_local.x, max_point_local.y, max_point_local.z),
        glm::vec3(max_point_local.x, min_point_local.y, min_point_local.z),
        glm::vec3(max_point_local.x, min_point_local.y, max_point_local.z),
        glm::vec3(max_point_local.x, max_point_local.y, min_point_local.z),
        glm::vec3(max_point_local.x, max_point_local.y, max_point_local.z)
   };

   
   min_point = glm::vec3(std::numeric_limits<float>::max());
   max_point = glm::vec3(std::numeric_limits<float>::lowest());
   
   
   for (const auto& corner : corners) {
       glm::vec3 trans_corner = glm::vec3(transform * glm::vec4(corner, 1.0f));
       min_point.x = std::min(min_point.x, trans_corner.x);
       min_point.y = std::min(min_point.y, trans_corner.y);
       min_point.z = std::min(min_point.z, trans_corner.z);
   
       max_point.x = std::max(max_point.x, trans_corner.x);
       max_point.y = std::max(max_point.y, trans_corner.y);
       max_point.z = std::max(max_point.z, trans_corner.z);
   }
}

