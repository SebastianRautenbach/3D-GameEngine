#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

namespace ray {
	inline glm::vec3 ray_cast(glm::vec2 mouse_pos, glm::vec2 screen_size, glm::mat4 projection_mtx, glm::mat4 view_mtx) {
        
        float x = (2.0f * mouse_pos.x) / screen_size.x - 1.0f;
        float y = 1.0f - (2.0f * mouse_pos.y) / screen_size.y;
        float z = 1.0f;

        glm::vec3 ray_nds = glm::vec3(x, y, z);
        
        glm::vec4 ray_clip = glm::vec4(ray_nds.x, ray_nds.y, -1.0, 1.0);
        
        glm::vec4 ray_eye = glm::inverse(projection_mtx) * ray_clip;
        ray_eye = glm::vec4(ray_eye.x, ray_eye.y, -1.0, 0.0);
        
        glm::vec4 ray_w = glm::inverse(view_mtx) * ray_eye;
        glm::vec3 ray_world = glm::normalize(glm::vec3(ray_w));

        return ray_world;

	}

    inline glm::vec3 ray_origin(glm::mat4 view_mtx) {
        return glm::vec3(glm::inverse(view_mtx) * glm::vec4(0, 0, 0, 1));
	}
}