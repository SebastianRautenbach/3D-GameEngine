#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include "other utils/ZER.h"


namespace wizm {

	class save_node {

		virtual void save_data(std::string parent_name, std::string index, filedata::ZER& save_t) const = 0;
		virtual void read_saved_data(std::string parent_name, std::string index, filedata::ZER& save_t) = 0;
	};


	class core_node : public save_node {


	public:

		core_node();
		virtual ~core_node();


		//----------------------------------------------------------------------------------

		void add_parent(core_node* parent_node) {
			
			if(m_parent_node)
				m_parent_node->remove_child(this);

			m_parent_node = parent_node;
			m_parent_node->add_child(this);
		}

		void remove_parent() {
			if (m_parent_node)
			{
				m_parent_node->remove_child(this);
				m_parent_node = nullptr;
			}
		}

		void add_child(core_node* child_node) {
			m_child_nodes.emplace_back(child_node);
		}

		core_node* get_parent() { return m_parent_node; }

		std::vector<core_node*>& get_children() {
			return m_child_nodes;
		}

		void remove_child(core_node* entity) {
			if (m_child_nodes.empty()) { return; }
			m_child_nodes.erase(std::remove(m_child_nodes.begin(), m_child_nodes.end(), entity), m_child_nodes.end());
		}


		//---------------------------------------------------------------------------------- TRANSFORM

		// Setters
		void set_position(const glm::vec3& position);
		void add_position(const glm::vec3& offset);
		void set_rotation(const glm::vec3& rotation);
		void add_rotation(const glm::vec3& deltaRotation);
		void set_scale(const glm::vec3& scale);
		void add_scale(const glm::vec3& deltaScale);


		// Getters
		const glm::vec3& get_position() const { return m_translation; }		   // <<  These are local transforms
		const glm::vec3& get_rotation() const { return m_rotation; }		   // <<  These are local transforms
		const glm::vec3& get_scale() const { return m_scale; }				   // <<  These are local transforms
		const glm::mat4& get_transform() const { return m_transform; }		   // <<  These are local transforms

		glm::mat4 get_world_transform();

		glm::vec3 get_world_position() {
			if (m_parent_node)
				return glm::vec3(m_parent_node->get_world_transform() * glm::vec4(m_translation, 1.0f));
			else
				return m_translation;
		}

		glm::vec3 get_direction() {
			return get_world_rotation_quat() * glm::vec3(0.0f, 0.0f, -1.0f);
		}

		glm::mat4 get_rotation_matrix() {
			glm::quat localQuat = glm::quat(glm::radians(m_rotation));
			if (m_parent_node)
				return glm::mat4_cast(m_parent_node->get_world_rotation_quat() * localQuat);
			else
				return glm::mat4_cast(localQuat);
		}


		glm::vec3 get_world_rotation() {

			glm::vec3 euler_angles = glm::eulerAngles(get_world_rotation_quat());
			return glm::degrees(euler_angles);

		}


		glm::quat get_world_rotation_quat()
		{
			glm::mat4 world = get_world_transform();
			glm::mat3 R = glm::mat3(world);

			
			R[0] = glm::normalize(R[0]);
			R[1] = glm::normalize(R[1]);
			R[2] = glm::normalize(R[2]);

			
			if (glm::determinant(R) < 0.0f) {
				R[0] = -R[0];
			}

			return glm::quat_cast(R);
		}


		glm::vec3 get_world_scale() {
			if (m_parent_node) {
				return m_parent_node->get_scale() * get_scale();
			}
			else {
				return get_scale();
			}
		}




		void save_data(std::string parent_name, std::string index, filedata::ZER& save_t) const override {}
		void read_saved_data(std::string parent_name, std::string index, filedata::ZER& save_t) override {};


	private:
		glm::vec3 m_translation;
		glm::vec3 m_rotation;
		glm::vec3 m_scale;
		glm::mat4 m_transform;


	private:
		core_node* m_parent_node = nullptr;
		std::vector<core_node*> m_child_nodes;

	public:
		inline glm::vec3 rotate_direction(const glm::vec3& direction, const glm::vec3& axis, float angle_degrees) const {
			float angle_radians = glm::radians(angle_degrees);
			glm::mat4 rotation_matrix = glm::rotate(glm::mat4(1.0f), angle_radians, axis);
			glm::vec4 rotated_direction = rotation_matrix * glm::vec4(direction, 0.0f);
			return glm::vec3(rotated_direction);
		}


	};




}