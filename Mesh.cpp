#pragma once
#include "Mesh.h"
#include "Shader.h"

// indirect (indexed) draw 

Mesh::Mesh(GLenum primitive_type, std::vector<Vertex>& vertices, std::vector<GLuint>& indices, GLuint texture_id) :
	primitive_type(primitive_type),
	vertices(vertices),
	indices(indices),
	Tex_ID(texture_id)
{

	Shader my_shader = Shader(VS_PATH, FS_PATH);
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	// Bind VAO (set as the current)
	glBindVertexArray(VAO);
	// Bind the VBO, set type as GL_ARRAY_BUFFER
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// Fill-in data into the VBO
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
	// Bind EBO, set type GL_ELEMENT_ARRAY_BUFFER
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	// Fill-in data into the EBO
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);
	// Set Vertex Attribute to explain OpenGL how to interpret the VBO
	GLint pos_location, normals_location, uvs_location;
	pos_location = glGetAttribLocation(my_shader.getShaderID(), "aPos"); //name in shader src
	if (pos_location < 0) pos_location = 0;
	glVertexAttribPointer(pos_location, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(0 + offsetof(Vertex, Position)));
	// Enable the Vertex Attribute for position
	glEnableVertexAttribArray(pos_location);
	// Set end enable Vertex Attribute for Normal
	normals_location = glGetAttribLocation(my_shader.getShaderID(), "aNormal"); //name in shader src
	if (normals_location < 0) normals_location = 1;
	glVertexAttribPointer(normals_location, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(0 + offsetof(Vertex, Normal)));
	glEnableVertexAttribArray(normals_location);
	// Set end enable Vertex Attribute forTexture Coordinates
	uvs_location = glGetAttribLocation(my_shader.getShaderID(), "aTexCoord"); //name in shader src
	if (uvs_location < 0) uvs_location = 2;
	glVertexAttribPointer(uvs_location, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(0 + offsetof(Vertex, UVs)));
	glEnableVertexAttribArray(uvs_location);
	// Bind VBO and VAO to 0 to prevent unintended modification of VAO,VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	// USE
	my_shader.activate();
	// TODO: create and initialize VAO, VBO, EBO and parameters
}

void Mesh::Draw(Shader& shader) {
	// TODO: set uniform variables: color
	//glm::vec3 color= { 1.0f, 0.0f, 0.0f};
	//shader.setUniform("color", color);
	if (Tex_ID > 0) {
		//set texture id etc...
		glActiveTexture(GL_TEXTURE0);
		//if (texture_id > 0) {
		glBindTexture(GL_TEXTURE_2D, Tex_ID);
		//    glActiveTexture(GL_TEXTURE0);
		//shader.setUniform("uTexture", 0);
		//    glBindTexture(GL_TEXTURE_2D, texture_id);
	}
	shader.activate();
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

}

void Mesh::clear(void) {
	vertices.clear();
	indices.clear();
	Tex_ID = 0;
	primitive_type = GL_POINT;

	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &EBO);

}
