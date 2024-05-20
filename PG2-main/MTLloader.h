#pragma once

struct Material {
	std::string name;
	glm::vec3 ambientColor;
	glm::vec3 diffuseColor;
	glm::vec3 specularColor;
	float shininess;
	std::string texturePath;
	std::vector<glm::vec3> mat_vertices;
	std::vector<glm::vec3> mat_normals;
	std::vector<glm::vec2> mat_uvs;
};

class MTLloader
{
public:
	std::vector<Material> loadMTL(const std::string& path);
};

