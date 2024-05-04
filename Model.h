#pragma once
#include "Mesh.h"
#include "Shader.h"
#include "OBJloader.h"

class Model
{
public:

	glm::vec3 position;
	glm::vec3 scale{};
	glm::vec4 rotation{}; // axes xyz + angle

	Model(const std::filesystem::path& filename, const std::filesystem::path& path_tex, glm::vec3 obj_position, bool is_height_map = false);
	glm::mat4 getTransMatrix(glm::mat4 trans);
	void Draw(Shader& shader);
	const unsigned int mesh_step_size = 10;
	float center_x;
	float height_y;
	float center_z;

private:
	std::vector<Mesh> meshes;
	std::string name;
	glm::vec3 rotation_axes{};


	void LoadOBJFile(const std::filesystem::path& filename);
	void LoadMTLFile(const std::filesystem::path& filename); //TODO: implement

	void HeightMap_Load(const std::filesystem::path& hm_file);
	glm::vec2 getSubtexST(const int x, const int y);
	glm::vec2 getSubtexByHeight(float height);

	std::vector<Vertex> mesh_vertexes{};
	std::vector<GLuint> mesh_vertex_indices{};
};