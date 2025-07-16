#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <array>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/rotate_vector.hpp>

namespace wizm
{
	class bounding_volume {
    public:
        glm::vec3 min_point = glm::vec3(), min_point_local = glm::vec3();
        glm::vec3 max_point = glm::vec3(), max_point_local = glm::vec3();
        glm::vec3 extents = glm::vec3(), init_extents = glm::vec3(), center = glm::vec3();
        glm::vec3 axes[3], init_axes[3];

        bounding_volume() : min_point(glm::vec3(std::numeric_limits<float>::max())),
            max_point(glm::vec3(std::numeric_limits<float>::lowest())) {}

        bounding_volume(const glm::vec3& min_point, const glm::vec3& max_point)
            : min_point(min_point), max_point(max_point) {}


        glm::vec3 get_center() const {
            return (min_point + max_point) / 2.0f;
        }

        glm::vec3 get_size() const {
            return max_point - min_point;
        }

        /*
            This function not only checks for a hit but also returns the intersection point if it did intersect
        */

        bool ray_intersect(glm::vec3 ray_dir, glm::vec3 ray_origin, glm::vec3& intersection_point) const {
            
            float tmin = -FLT_MAX;
            float tmax = FLT_MAX;
            bool is_inside = true;

            // check if they ray isnt inside the actual object
            for (int i = 0; i < 3; ++i) {
                float axis_dot_origin = glm::dot(axes[i], ray_origin - center);


                if (axis_dot_origin < -extents[i] || axis_dot_origin > extents[i]) {
                    is_inside = false;
                    break;
                }
            }


            if (is_inside) {
                return false;
            }



            for (int i = 0; i < 3; ++i) {
            
            
                float axis_dot_ray_dir = glm::dot(axes[i], ray_dir);
                float axis_dot_origin = glm::dot(axes[i], ray_origin - center);
            
                if (glm::epsilonNotEqual(axis_dot_ray_dir, 0.0f, glm::epsilon<float>())) {
            
                    float t1 = (-axis_dot_origin - extents[i]) / axis_dot_ray_dir;
                    float t2 = (-axis_dot_origin + extents[i]) / axis_dot_ray_dir;
            
            
                    if (t1 > t2) std::swap(t1, t2);
            
                    tmin = glm::max(tmin, t1);
                    tmax = glm::min(tmax, t2);
            
                    if (tmin > tmax) {
                        return false;
                    }
                }
                else {
            
                    if (axis_dot_origin < -extents[i] || axis_dot_origin > extents[i]) {
                        return false;
                    }
                }
            }
            
            if (tmin < 0.0f) {
                return false;
            }


            intersection_point = ray_origin + tmin * ray_dir;
            return true;





        }

        /*
            Handeling physics
        */

        bool intersects(const bounding_volume& other) const {
            return (min_point.x <= other.max_point.x && max_point.x >= other.min_point.x) &&
                (min_point.y <= other.max_point.y && max_point.y >= other.min_point.y) &&
                (min_point.z <= other.max_point.z && max_point.z >= other.min_point.z);
        }

	}; 
}