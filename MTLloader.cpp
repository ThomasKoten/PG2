#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>

#include <GL/glew.h> 
#include <glm/glm.hpp>
#include "MTLloader.h"


std::vector<Material> MTLloader::loadMTL(const std::string& path) {
	std::ifstream file(path);
	std::vector<Material> materials;
	Material currentMaterial;

	if (!file.is_open()) {
		std::cerr << "Failed to open MTL file: " << path << std::endl;
		return materials;
	}

	std::string line;
	while (std::getline(file, line)) {
		std::istringstream iss(line);
		std::string type;
		iss >> type;

		if (type == "newmtl") {
			if (!currentMaterial.name.empty()) {
				materials.push_back(currentMaterial);
			}
			currentMaterial = Material();
			iss >> currentMaterial.name;
		}
		else if (type == "Ka") {
			iss >> currentMaterial.ambientColor.r >> currentMaterial.ambientColor.g >> currentMaterial.ambientColor.b;
		}
		else if (type == "Kd") {
			iss >> currentMaterial.diffuseColor.r >> currentMaterial.diffuseColor.g >> currentMaterial.diffuseColor.b;
		}
		else if (type == "Ks") {
			iss >> currentMaterial.specularColor.r >> currentMaterial.specularColor.g >> currentMaterial.specularColor.b;
		}
		else if (type == "illum") {
			iss >> currentMaterial.shininess;
		}
		else if (type == "map_Kd") {
			iss >> currentMaterial.texturePath;
		}
	}

	if (!currentMaterial.name.empty()) {
		materials.push_back(currentMaterial);
	}

	file.close();
	return materials;
}