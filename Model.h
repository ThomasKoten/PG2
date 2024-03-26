#pragma once
#include "Mesh.h"
#include "Shader.h"

class Model
{
public:
	Model(const std::filesystem::path& filename);

	void Draw(Shader& shader);

private:
	std::vector<Mesh> meshes;
	std::string name;


	void LoadOBJFile(const std::filesystem::path& filename);
	void LoadMTLFile(const std::filesystem::path& filename); //TODO: implement


	std::vector<Vertex> vertexes{};
	std::vector<GLuint> vertex_indices{};
};