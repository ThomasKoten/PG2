#pragma once
#ifndef OBJloader_H
#define OBJloader_H

#include <vector>
#include <glm/fwd.hpp>
#include "Vertex.h"

bool loadOBJ(
	const char* path,
	std::vector < glm::vec3 >& out_vertices,
	std::vector < glm::vec2 >& out_uvs,
	std::vector < glm::vec3 >& out_normals
);
std::vector<Vertex> make_Vertexes(std::vector<glm::vec3> vertices, std::vector<glm::vec3>  normals, std::vector<glm::vec2>  uvs);

#endif
