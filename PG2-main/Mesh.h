#pragma once

#include <string>
#include <vector>

#include "Vertex.h"
#include "Shader.h"
#include <GL/glew.h>

#include <glm/glm.hpp> 
#include <glm/ext.hpp>
#include <opencv2\opencv.hpp>

class Mesh {
public:

	// mesh data
	std::vector<Vertex> vertices;
	std::vector< glm::vec4 > colors;
	std::vector<GLuint> indices;
	GLuint texture_id{ 0 }; // texture id=0  means no texture
	GLenum draw_mode = GL_TRIANGLES;

	// mesh material
	glm::vec4 diffuse_color{1.0f}; //white, non-transparent 

	// indirect (indexed) draw 
	Mesh(GLenum draw_mode, std::vector<Vertex>& vertices, std::vector<GLuint>& indices, GLuint texture_id);

	void Draw(Shader& shader);

	void clear(void);

private:
	// OpenGL buffer IDs
	// ID = 0 is reserved (i.e. uninitalized)
	unsigned int VAO{ 0 }, VBO{ 0 }, EBO{ 0 }, Tex_ID{ 0 };
};


