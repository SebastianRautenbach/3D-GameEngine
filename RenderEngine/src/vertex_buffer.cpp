#include "gl core/vertex_buffer.h"
#include <iostream>
static const unsigned int max_framebuffer_size = 8192;




//-----------------------------------------------------------------------


void core_vertex_buffer::gen_buffer()
{
	glGenBuffers(1, &this->buffer_id);
}



//-----------------------------------------------------------------------


void core_vertex_buffer::bind_buffer()
{
	glBindBuffer(GL_ARRAY_BUFFER, this->buffer_id);
	glBufferData(GL_ARRAY_BUFFER, this->mvertices.size() * sizeof(vertex_data), &this->mvertices[0], GL_STATIC_DRAW);
}

//-----------------------------------------------------------------------

void core_vertex_buffer::unbind_buffer()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}


//-----------------------------------------------------------------------

void core_vertex_buffer::destroy_buffer()
{
	glDeleteBuffers(1, &this->buffer_id);
}




//-----------------------------------------------------------------------

void core_index_buffer::gen_buffer()
{
	glGenBuffers(1, &this->buffer_id);
}


//-----------------------------------------------------------------------

void core_index_buffer::bind_buffer()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->buffer_id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->mindices.size() * sizeof(unsigned int), this->mindices.data(), GL_STATIC_DRAW);
}




//-----------------------------------------------------------------------

void core_index_buffer::unbind_buffer()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}


//-----------------------------------------------------------------------





void core_index_buffer::destroy_buffer()
{
	glDeleteBuffers(1, &this->buffer_id);
}



//////////////////////////////////////////////////////////////////////////
// DEFAULT CONSTRUCTOR FOR VAO
//////////////////////////////////////////////////////////////////////////


core_arr_vertex_buffer::core_arr_vertex_buffer(std::vector<vertex_data>& vertices, std::vector<unsigned int>& indices)
{

	glGenVertexArrays(1, &this->buffer_id);
	vbo = new core_vertex_buffer(vertices);
	ibo = new core_index_buffer(indices);


}

core_arr_vertex_buffer::~core_arr_vertex_buffer()
{
}




//-----------------------------------------------------------------------


void core_arr_vertex_buffer::create_buffer() {
	bind_buffer();

	vbo->bind_buffer();
	ibo->bind_buffer();

	for (const auto& i : vertex_attribs)
	{
		glVertexAttribPointer(i.layout, i.size, GL_FLOAT, GL_FALSE, i.stride, (void*)(intptr_t)i.attrib_length);
		glEnableVertexAttribArray(i.layout);
	}

	vbo->unbind_buffer();
	unbind_buffer();
}


//-----------------------------------------------------------------------

void core_arr_vertex_buffer::draw_buffer(unsigned int size_of_row)
{
	bind_buffer();
	glDrawElements(GL_TRIANGLES, size_of_row, GL_UNSIGNED_INT, 0);
	//unbind_buffer();
}





//-----------------------------------------------------------------------

void core_arr_vertex_buffer::bind_buffer()
{
	glBindVertexArray(this->buffer_id);
}




//-----------------------------------------------------------------------

void core_arr_vertex_buffer::unbind_buffer()
{
	glBindVertexArray(0);
}





//-----------------------------------------------------------------------

void core_arr_vertex_buffer::destroy_buffer()
{
	vbo->destroy_buffer();
	ibo->destroy_buffer();
	glDeleteBuffers(1, &this->buffer_id);
}




//-----------------------------------------------------------------------


void core_arr_vertex_buffer::create_attrib_arr(unsigned int layout, int size, int stride, int attrib_length)
{
	vertex_attribs.emplace_back(layout, size, stride, attrib_length);
}



//-----------------------------------------------------------------------

core_framebuffer::core_framebuffer(framebuffer_spec spec)
	:m_spec(spec)
{
	for (auto format : m_spec.attachment.attachments) {
		if (!lowlevelsys::is_depth_format(format.texture_format))
			m_color_attachment_spec.emplace_back(format);
		else
			m_depth_attachment_spec = format.texture_format;
	}

	create_fbuffer();
}

core_framebuffer::~core_framebuffer()
{
	destroy_buffer();
}

int core_framebuffer::read_pixel(unsigned int color_attachement_index, int x, int y)
{
	glReadBuffer(GL_COLOR_ATTACHMENT0 + color_attachement_index);

	float r, g, b;
	float pixels[3];
	glReadPixels(x, y, 1, 1, GL_RGB, GL_FLOAT, pixels);

	r = pixels[0];
	g = pixels[1];
	b = pixels[2];

	int red = static_cast<int>(r * 255.0f);
	int green = static_cast<int>(g * 255.0f);
	int blue = static_cast<int>(b * 255.0f);

	int pick_color = (red << 16) | (green << 8) | blue;

	return pick_color;
}

void core_framebuffer::clear_attachment(unsigned int attachment_index, int value)
{
	if (attachment_index > m_color_attachment_spec.size())
		throw std::invalid_argument("index out of range for fbo");

	auto& spec = m_color_attachment_spec[attachment_index];
	glClearTexImage(m_color_attachments[attachment_index], 0, lowlevelsys::framebuffer_texture_format_to_gl(spec.texture_format), GL_INT, &value);
}

void core_framebuffer::create_fbuffer()
{
	if (this->buffer_id)
	{
		destroy_buffer();
	}

	glGenFramebuffers(1, &this->buffer_id);
	glBindFramebuffer(GL_FRAMEBUFFER, this->buffer_id);

	bool multisample = m_spec.Samples > 1;

	if (m_color_attachment_spec.size()) {

		m_color_attachments.resize(m_color_attachment_spec.size());
		lowlevelsys::create_texture(multisample, m_color_attachments.data(), (uint32_t)m_color_attachments.size());


		for (int i = 0; i < m_color_attachments.size(); i++) {
			lowlevelsys::bind_texture(multisample, m_color_attachments[i]);
			switch (m_color_attachment_spec[i].texture_format) {
			case framebuffer_texture_format::RGBA8:
				lowlevelsys::attach_color_texture(m_color_attachments[i], m_spec.Samples, GL_RGBA8, GL_RGBA, m_spec.Width, m_spec.Height, i);
				break;

			case framebuffer_texture_format::RED_INTEGER:
				lowlevelsys::attach_color_texture(m_color_attachments[i], m_spec.Samples, GL_R32I, GL_RED_INTEGER, m_spec.Width, m_spec.Height, i);
				break;

			case framebuffer_texture_format::RGBA16:
				lowlevelsys::attach_color_texture(m_color_attachments[i], m_spec.Samples, GL_RGBA16F, GL_RGBA, m_spec.Width, m_spec.Height, i);
				break;
			}
		}
	}
	if (m_depth_attachment_spec.texture_format != framebuffer_texture_format::None) {
		lowlevelsys::create_texture(multisample, &m_depth_attachment, 1);
		lowlevelsys::bind_texture(multisample, m_depth_attachment);
		switch (m_depth_attachment_spec.texture_format) {
		case framebuffer_texture_format::DEPTH24STENCIL8:
			lowlevelsys::attach_depth_texture(m_depth_attachment, m_spec.Samples, GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL_ATTACHMENT, m_spec.Width, m_spec.Height);
			break;
		}
	}
	if (m_color_attachments.size() > 1) {
		GLenum buffers[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
		glDrawBuffers((GLsizei)m_color_attachments.size(), buffers);
	}
	else if (m_color_attachments.empty()) {			

		GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if (status != GL_FRAMEBUFFER_COMPLETE) {
			std::cerr << "Framebuffer not complete!" << std::endl;
		}

		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
	}
	unbind_buffer();
}

void core_framebuffer::resize(unsigned int width, unsigned int height)
{
	if (width == 0 || height == 0 || width > max_framebuffer_size || height > max_framebuffer_size)
	{
		return;
	}

	m_spec.Width = width;
	m_spec.Height = height;

	invalidate();
}

void core_framebuffer::invalidate()
{
	if (buffer_id)
	{
		glDeleteFramebuffers(1, &buffer_id);

		glDeleteTextures((GLsizei)m_color_attachments.size(), m_color_attachments.data());
		glDeleteTextures(1, &m_depth_attachment);

		m_color_attachments.clear();
		m_depth_attachment = 0;
	}

	glCreateFramebuffers(1, &buffer_id);
	glBindFramebuffer(GL_FRAMEBUFFER, buffer_id);

	bool multisample = m_spec.Samples > 1;

	if (m_color_attachment_spec.size())
	{
		m_color_attachments.resize(m_color_attachment_spec.size());
		lowlevelsys::create_texture(multisample, m_color_attachments.data(), (uint32_t)m_color_attachments.size());

		for (int i = 0; i < m_color_attachments.size(); i++)
		{
			lowlevelsys::bind_texture(multisample, m_color_attachments[i]);
			switch (m_color_attachment_spec[i].texture_format)
			{
			case framebuffer_texture_format::RGBA8:
				lowlevelsys::attach_color_texture(m_color_attachments[i], m_spec.Samples, GL_RGBA8, GL_RGBA, m_spec.Width, m_spec.Height, i);
				break;
			case framebuffer_texture_format::RED_INTEGER:
				lowlevelsys::attach_color_texture(m_color_attachments[i], m_spec.Samples, GL_R32I, GL_RED_INTEGER, m_spec.Width, m_spec.Height, i);
				break;
			}
		}
	}

	if (m_depth_attachment_spec.texture_format != framebuffer_texture_format::None)
	{
		lowlevelsys::create_texture(multisample, &m_depth_attachment, 1);
		lowlevelsys::bind_texture(multisample, m_depth_attachment);
		switch (m_depth_attachment_spec.texture_format)
		{
		case framebuffer_texture_format::DEPTH24STENCIL8:
			lowlevelsys::attach_depth_texture(m_depth_attachment, m_spec.Samples, GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL_ATTACHMENT, m_spec.Width, m_spec.Height);
			break;
		}
	}

	if (m_color_attachments.size() > 1)
	{
		if (!(m_color_attachments.size() <= 4))
			throw std::invalid_argument("m_color_attachments.size() <= 4");

		GLenum buffers[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
		glDrawBuffers((GLsizei)m_color_attachments.size(), buffers);
	}
	else if (m_color_attachments.empty())
	{
		glDrawBuffer(GL_NONE);
	}

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		throw std::invalid_argument("framebuffer isnt built");
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void core_framebuffer::gen_buffer()
{
}

void core_framebuffer::bind_buffer()
{
	glBindFramebuffer(GL_FRAMEBUFFER, this->buffer_id);
	glViewport(0, 0, m_spec.Width, m_spec.Height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void core_framebuffer::unbind_buffer()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void core_framebuffer::destroy_buffer()
{
	if (this->buffer_id)
	{
		glDeleteFramebuffers(GL_FRAMEBUFFER, &this->buffer_id);
		for (auto attachment : m_color_attachments) {
			glDeleteTextures(1, &attachment);
		}
		m_color_attachments.clear();
		
		glDeleteTextures(1, &m_depth_attachment);		
	}
}

core_newframebuffer::core_newframebuffer(int pwidth, int pheight)
	:width(pwidth), height(pheight)
{
	glGenFramebuffers(1, &buffer_id);
}



void core_newframebuffer::bind_buffer()
{
	glBindFramebuffer(GL_FRAMEBUFFER, this->buffer_id);
	glViewport(0, 0, width, height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void core_newframebuffer::unbind_buffer()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void core_newframebuffer::destroy_buffer()
{
	glDeleteFramebuffers(1, &buffer_id);
	glDeleteTextures(1, &color_texture);
	if (depth_buffer != 0) {
		glDeleteRenderbuffers(1, &depth_buffer);
	}
}

void core_newframebuffer::resize(unsigned int new_width, unsigned int new_height)
{
	width = new_width;
	height = new_height;

	glBindFramebuffer(GL_FRAMEBUFFER, buffer_id);
	glBindTexture(GL_TEXTURE_2D, color_texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);

	if (depth_buffer != 0) {
		glBindRenderbuffer(GL_RENDERBUFFER, depth_buffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

unsigned int core_newframebuffer::read_pixel(int x, int y) {

	glBindFramebuffer(GL_FRAMEBUFFER, buffer_id);

	glReadBuffer(GL_COLOR_ATTACHMENT0);

	unsigned char pixels[3];
	glReadPixels(x, height - y, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, &pixels);


	GLuint pickID = (static_cast<GLuint>(pixels[0]) << 16) |
		(static_cast<GLuint>(pixels[1]) << 8) |
		(static_cast<GLuint>(pixels[2]));

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return pickID;
}


void core_newframebuffer::initialize(GLenum color_format, GLenum depth_format, bool use_depth_buffer)
{
	glGenFramebuffers(1, &buffer_id);
	glBindFramebuffer(GL_FRAMEBUFFER, buffer_id);

	glGenTextures(1, &color_texture);
	glBindTexture(GL_TEXTURE_2D, color_texture);
	glTexImage2D(GL_TEXTURE_2D, 0, color_format, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, color_texture, 0);


	if (use_depth_buffer) {
		glGenRenderbuffers(1, &depth_buffer);
		glBindRenderbuffer(GL_RENDERBUFFER, depth_buffer);
		glRenderbufferStorage(GL_RENDERBUFFER, depth_format, width, height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, depth_buffer);
	}


	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		std::cerr << "Error: Framebuffer is not complete!\n";
	}


	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}