#pragma once

#include <glm/glm.hpp> 
#include <glm/ext.hpp>
#include <string>
#include <vector>

#include "Vertex.h"
#include "Shader.h"
#include <GL/glew.h>

class Mesh {
public:

	// mesh data
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	GLuint texture_id{ 0 }; // texture id=0  means no texture
	GLenum primitive_type = GL_POINT;

	// mesh material
	glm::vec4 diffuse_color{1.0f}; //white, non-transparent 

	// indirect (indexed) draw 
	Mesh(GLenum primitive_type, std::vector<Vertex>& vertices, std::vector<GLuint>& indices, GLuint texture_id);

	void Draw(Shader& shader);

	void clear(void);

private:
	// OpenGL buffer IDs
	// ID = 0 is reserved (i.e. uninitalized)
	unsigned int VAO{0}, VBO{ 0 }, EBO{ 0 };
};
