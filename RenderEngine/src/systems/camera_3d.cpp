#include "system/camera_3d.h"
#include <algorithm>
#include <iostream>

wizm::camera_core_3d::camera_core_3d(int window_width, int window_height)
    :m_window_width(window_width), m_window_height(window_height)
{
    m_fov = 60.f;
    m_far_plane = 1000.0f;
    m_near_plane = 0.1f;

    update_aspect_ratio();
    update_projection_matrix();
    update_view_matrix();
}

wizm::camera_core_3d::~camera_core_3d()
{
}

void wizm::camera_core_3d::set_position(const glm::vec3& position)
{
    m_position = position;
    update_view_matrix();
}

void wizm::camera_core_3d::set_rotation(glm::vec3& rotation) {
    m_pitch = rotation.x;
    m_yaw = rotation.y;
    m_roll = rotation.z;
    update_view_matrix();
}

void wizm::camera_core_3d::set_rotation(float pitch, float yaw, float roll)
{
    m_pitch = pitch;
    m_yaw = yaw;
    m_roll = roll;
    update_view_matrix();

}

void wizm::camera_core_3d::set_rotation_matrix(const glm::mat4& rotation_matrix)
{
    glm::mat3 rot3x3 = glm::mat3(rotation_matrix);
    glm::vec3 euler_angles = glm::eulerAngles(glm::quat_cast(rot3x3));
    m_pitch = glm::degrees(euler_angles.x);
    m_yaw = glm::degrees(euler_angles.y);
    m_roll = glm::degrees(euler_angles.z);
    m_rotation_matrix = rotation_matrix;
    update_view_matrix();
}



void wizm::camera_core_3d::set_window_size(int width, int height)
{
    m_window_width = width;
    m_window_height = height;
    update_aspect_ratio();
    update_projection_matrix();
}

void wizm::camera_core_3d::set_aspect_ratio(int aspect)
{
    m_aspect_ratio = (float)aspect;
    update_projection_matrix();
}

void wizm::camera_core_3d::add_yaw(float yaw)
{
    m_yaw -= yaw;
    update_rotation_matrix();
}

void wizm::camera_core_3d::add_pitch(float pitch)
{
    m_pitch = std::clamp(m_pitch + pitch, -89.0f, 89.0f);
    update_rotation_matrix();
}

void wizm::camera_core_3d::add_roll(float roll)
{
    m_roll += roll;
    update_rotation_matrix();
}

void wizm::camera_core_3d::move_up(float distance)
{
    m_position += distance * get_up_vector();
    update_view_matrix();
}

void wizm::camera_core_3d::move_right(float distance)
{
    m_position += distance * get_right_vector();
    update_view_matrix();
}

void wizm::camera_core_3d::move_forward(float distance)
{
    m_position += distance * get_forward_vector();
    update_view_matrix();
}

void wizm::camera_core_3d::update_rotation_matrix()
{
    glm::quat quaternion = glm::quat(glm::vec3(glm::radians(m_pitch), glm::radians(m_yaw), glm::radians(m_roll)));
    m_rotation_matrix = glm::toMat4(quaternion);
    update_view_matrix();
}

void wizm::camera_core_3d::update_aspect_ratio()
{
    m_aspect_ratio = static_cast<float>(m_window_width) / static_cast<float>(m_window_height);
}

void wizm::camera_core_3d::update_projection_matrix()
{
    m_projection_matrix = glm::perspective(glm::radians(m_fov), m_aspect_ratio, m_near_plane, m_far_plane);
}

void wizm::camera_core_3d::update_view_matrix()
{

    glm::vec3 forward = get_forward_vector();
    glm::vec3 up = get_up_vector();

    m_view_matrix = glm::lookAt(m_position, m_position + forward, up);

}
