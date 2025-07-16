#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "node.h"

namespace wizm {

	class camera_core_3d : public core_node {

	public:
		camera_core_3d(int window_width, int window_height);
		~camera_core_3d();

		const glm::vec3& get_position() const {
			return m_position;
		}

		const glm::mat4& get_view_matrix() const {
			return m_view_matrix;
		}
		const glm::mat4& get_projection_matrix() const {
			return m_projection_matrix;
		}

		glm::vec3 get_forward_vector() const {
			return -glm::normalize(glm::vec3(m_rotation_matrix[2]));
		}

		glm::vec3 get_right_vector() const {
			return glm::normalize(glm::vec3(m_rotation_matrix[0]));
		}

		glm::vec3 get_up_vector() const {
			return glm::normalize(glm::vec3(m_rotation_matrix[1]));
		}


		void set_position(const glm::vec3& position);
		void set_rotation(glm::vec3& rotation);
		void set_rotation(float pitch, float yaw, float roll);
		void set_rotation_matrix(const glm::mat4& rotation_matrix);
		void set_window_size(int width, int height);
		void set_aspect_ratio(int aspect);


		void add_yaw(float yaw);
		void add_pitch(float pitch);
		void add_roll(float roll);

		glm::vec2 get_window_size() { return glm::vec2(m_window_width, m_window_height); }


		void move_up(float distance);
		void move_right(float distance);
		void move_forward(float distance);

		void set_speed(float speed) { m_speed = speed; }
		void add_speed(float speed) { m_speed += speed; }

		float get_near() { return m_near_plane; }
		float get_far() { return m_far_plane; }
		float get_speed() { return m_speed; }

		/*
			Updates
		*/

		void update_rotation_matrix();
		void update_aspect_ratio();
		void update_projection_matrix();
		void update_view_matrix();


		void read_saved_data(std::string parent_name, std::string index, filedata::ZER& save_t) {
			

			if (save_t[parent_name]["transform"].get_float("position")[0]) {

				m_position.x = save_t[parent_name]["transform"].get_float("position")[0];
				m_position.y = save_t[parent_name]["transform"].get_float("position")[1];
				m_position.z = save_t[parent_name]["transform"].get_float("position")[2];

				m_pitch = save_t[parent_name]["transform"].get_float("rotation")[0];
				m_yaw = save_t[parent_name]["transform"].get_float("rotation")[1];
				m_roll = save_t[parent_name]["transform"].get_float("rotation")[2];


				update_rotation_matrix();

			}

			
							
		
		};
			
		void save_data(std::string parent_name, std::string index, filedata::ZER& save_t) {
			(void)index;
			save_t[parent_name]["transform"].set_float("position", {m_position.x, m_position.y, m_position.z});
			save_t[parent_name]["transform"].set_float("rotation", { m_pitch, m_yaw, m_roll });
			save_t[parent_name]["transform"].set_float("scale", { 1, 1, 1});

		}

	private:

		int m_window_width, m_window_height;

		glm::vec3 m_position = glm::vec3(0.0);

		float m_pitch = 0.0, m_yaw = 0.0, m_roll = 0.0;

		float m_fov;
		float m_aspect_ratio;
		float m_near_plane;
		float m_far_plane;

		float m_speed = 0.01f;

		glm::mat4 m_projection_matrix;
		glm::mat4 m_rotation_matrix = glm::mat4(1.0f);
		glm::mat4 m_view_matrix;


	};

}