#pragma once
#include "Mesh.h"
#include "Shader.h"

class Model
{
public:
	Model(const std::filesystem::path& filename, const std::filesystem::path& path_tex, glm::vec3 obj_position);
	glm::mat4 getTransMatrix(glm::mat4 trans);
	void Draw(Shader& shader);

private:
	std::vector<Mesh> meshes;
	std::string name;
	glm::vec3 position;


	void LoadOBJFile(const std::filesystem::path& filename);
	void LoadMTLFile(const std::filesystem::path& filename); //TODO: implement


	std::vector<Vertex> vertexes{};
	std::vector<GLuint> vertex_indices{};
};