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

	Model(const std::filesystem::path& filename, const std::filesystem::path& path_tex, glm::vec3 obj_position, float scale = 1.0f, glm::vec4 rotation = { 0.0f, 1.0f, 0.0f, 0.0f }, bool is_height_map = false);
	glm::mat4 getTransMatrix();
	void Draw(Shader& shader);
	float GetHeightAtPosition(float x, float z) const;
	const unsigned int mesh_step_size = 10;
	float center_x;
	float center_z;
	cv::Mat hmap;

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