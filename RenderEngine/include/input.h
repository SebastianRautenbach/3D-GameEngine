#pragma once
#include <GLFW/glfw3.h>
#include <unordered_set>
//#include "input_types.h"
#include <unordered_map>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


struct mouse_info {
	double x_offset, y_offset;
	bool mousemoved;
	float scroll_offset;
};



class input_manager {
public:
	
	input_manager(GLFWwindow* window, float width, float height);
	
	static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

	static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

	bool has_key_been_pressed(int key);

	mouse_info get_mouse_offset() { return _mouse_info; }
	
	mouse_info get_mouse_offset_new();
	
	void set_mouse_offset(float x, float y) { _mouse_info.x_offset = x; _mouse_info.y_offset = y; }
	
	void get_mouse_pos(double& mouse_x, double& mouse_y) { glfwGetCursorPos(m_window, &mouse_x, &mouse_y); }

	void set_mouse_cursor_pos(double mouse_x, double mouse_y) { (void)mouse_x; (void)mouse_y;  glfwSetCursorPos(m_window, (w_width / 2), (w_height / 2)); }

	void set_hide_mouse_cursor(bool hide) { hide ? glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN) : glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL); }

	void mouse_stop_move() { _mouse_info.mousemoved = false; }

	float get_mouse_scroll_offset() { return _mouse_info.scroll_offset; }
	void set_mouse_scroll_offset(float offset = 0) { _mouse_info.scroll_offset = offset; }

private:
	float w_width, w_height;
	static glm::vec2 last_mouse_pos;
	static mouse_info _mouse_info;
	static std::unordered_map<int, bool> keys;
	GLFWwindow* m_window = nullptr;
};