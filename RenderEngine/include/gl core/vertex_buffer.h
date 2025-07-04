#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

enum class framebuffer_texture_format {
	None = 0,
	
	// color
	RGBA8,
	RGBA16,
	RED_INTEGER,

	// depth
	DEPTH24STENCIL8,
	Depth = DEPTH24STENCIL8
};

struct framebuffer_texture_sepc {
	framebuffer_texture_sepc() = default;
	framebuffer_texture_sepc(framebuffer_texture_format format)
		: texture_format(format) {}

	framebuffer_texture_format texture_format;

};

struct framebuffer_attachment_spec {
	framebuffer_attachment_spec() = default;
	framebuffer_attachment_spec(const std::initializer_list<framebuffer_texture_sepc> specs)
		: attachments(specs) {}
	std::vector<framebuffer_texture_sepc> attachments;
};

struct framebuffer_spec
{
	uint32_t Width = 0, Height = 0;
	framebuffer_attachment_spec attachment;
	uint32_t Samples = 1;

	bool SwapChainTarget = false;
};


namespace lowlevelsys {

	/*
		These function were created by Cherno
	*/

	static bool is_depth_format(framebuffer_texture_format format)
	{
		switch (format)
		{
		case framebuffer_texture_format::DEPTH24STENCIL8:  return true;
		}

		return false;
	}

	static GLenum texture_target(bool multisampled)
	{
		return multisampled ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
	}


	static void create_texture(bool multisampled, uint32_t* outID, uint32_t count)
	{
		glCreateTextures(texture_target(multisampled), count, outID);
	}

	static void bind_texture(bool multisampled, uint32_t id)
	{
		glBindTexture(texture_target(multisampled), id);
	}

	static GLenum framebuffer_texture_format_to_gl(framebuffer_texture_format format)
	{
		switch (format)
		{
		case framebuffer_texture_format::RGBA8:       return GL_RGBA8;
		case framebuffer_texture_format::RED_INTEGER: return GL_RED_INTEGER;
		}

		return 0;
	}



	static void attach_color_texture(uint32_t id, int samples, GLenum internalFormat, GLenum format, uint32_t width, uint32_t height, int index)
	{
		bool multisampled = samples > 1;
		if (multisampled)
		{
			glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, internalFormat, width, height, GL_FALSE);
		}
		else
		{
			glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, nullptr);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		}

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, texture_target(multisampled), id, 0);
	}



	static void attach_depth_texture(uint32_t id, int samples, GLenum format, GLenum attachmentType, uint32_t width, uint32_t height)
	{
		bool multisampled = samples > 1;
		if (multisampled)
		{
			glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, format, width, height, GL_FALSE);
		}
		else
		{
			glTexStorage2D(GL_TEXTURE_2D, 1, format, width, height);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		}

		glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentType, texture_target(multisampled), id, 0);
	}

}


struct attrib_info {
	unsigned int layout;
	int size;
	int stride;
	int attrib_length;
};


struct vertex_data {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
	glm::vec3 Tangent;
	glm::vec3 Bitangent;
	int m_BoneIDs[4];
	float m_Weights[4];
};


class core_buffer_object {
public:
	virtual void gen_buffer() {};
	virtual void bind_buffer() {};
	virtual void unbind_buffer() {};
	virtual void destroy_buffer() {};

public:
	unsigned int buffer_id = 0;
};


class core_vertex_buffer : public core_buffer_object {
public:
	void gen_buffer() override;
	void bind_buffer() override;
	void unbind_buffer() override;
	void destroy_buffer() override;

	// default constructor 
	core_vertex_buffer(std::vector<vertex_data>& vertices) : mvertices(vertices) {gen_buffer();}
	
public:
	std::vector<vertex_data> mvertices;

};


class core_index_buffer : public core_buffer_object {
public:
	void gen_buffer() override;
	void bind_buffer() override;
	void unbind_buffer() override;
	void destroy_buffer() override;

	// default constructor 
	core_index_buffer(std::vector<unsigned int>& indices) : mindices(indices) {gen_buffer();}

public:
	std::vector<unsigned int> mindices;
};



class core_arr_vertex_buffer : public core_buffer_object {
	
public:
	// default constructor
	core_arr_vertex_buffer(std::vector<vertex_data>& vertices, std::vector<unsigned int>& indices);
	core_arr_vertex_buffer() {}
	~core_arr_vertex_buffer();
	
	void create_buffer();
	void draw_buffer(unsigned int size_of_row);
	void bind_buffer() override;
	void unbind_buffer() override;
	void destroy_buffer() override;
	void create_attrib_arr(unsigned int layout, int size, int stride, int attrib_length);

public:
	core_index_buffer* ibo;
	core_vertex_buffer* vbo;
	std::vector<attrib_info> vertex_attribs;
	
};

class core_newframebuffer : public core_buffer_object {

public:
	core_newframebuffer(int pwidth, int pheight);

	void bind_buffer() override;
	void unbind_buffer() override;
	void destroy_buffer() override;
	void resize(unsigned int new_width, unsigned int new_height);

	void initialize(GLenum color_format = GL_RGB32F, GLenum depth_format = GL_DEPTH_COMPONENT, bool use_depth_buffer = true);
	unsigned int read_pixel(int x, int y);

public:         
	GLuint color_texture = 0; 
	GLuint depth_buffer = 0;  
	int width;
	int height;
};

class core_framebuffer : public core_buffer_object {
public:
	core_framebuffer(framebuffer_spec spec);
	~core_framebuffer();

	unsigned int get_tex_id() { return m_tex_id; }

	int read_pixel(unsigned int color_attachement_index, int x, int y);
	void clear_attachment(unsigned int attachment_index, int value);

	void create_fbuffer();
	void resize(unsigned int width, unsigned int height);
	void invalidate();

	framebuffer_spec& get_specs() { return m_spec; }

	void gen_buffer() override;
	void bind_buffer() override;
	void unbind_buffer() override;
	void destroy_buffer() override;
	unsigned int get_color_attachment_render_id(int index = 0) {
		if(index > m_color_attachments.size())
			throw std::invalid_argument("index out of range for fbo");
		return m_color_attachments[index];
	}

private:
	unsigned int m_tex_id;
	unsigned int m_depth_id;

	/*
		I like how Cherno did his
	*/
	framebuffer_spec m_spec;
	std::vector<framebuffer_texture_sepc> m_color_attachment_spec;
	framebuffer_texture_sepc m_depth_attachment_spec = framebuffer_texture_format::None;

	std::vector<unsigned int> m_color_attachments;
	unsigned int m_depth_attachment = 0;

};