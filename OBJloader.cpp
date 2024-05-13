#pragma once
#include <iostream>
#include <fstream>
#include <GL/glew.h> 
#include <glm/glm.hpp>

#include "OBJloader.h"
#include "StringUtils.h"


#define MAX_LINE_SIZE 255

bool loadOBJ(const char* path, std::vector < glm::vec3 >& out_vertices, std::vector < glm::vec2 >& out_uvs, std::vector < glm::vec3 >& out_normals)
{
	auto file_lines = FillFileLines(path);

	std::vector< unsigned int > vertexIndices, uvIndices, normalIndices;
	std::vector< glm::vec3 > temp_vertices;
	std::vector< glm::vec2 > temp_uvs;
	std::vector< glm::vec3 > temp_normals;

	std::string first_two_chars, first_three_chars;
	bool line_success;

	out_vertices.clear();
	out_uvs.clear();
	out_normals.clear();



	for (const std::string& line : file_lines) {
		if (!line.empty()) {
			line_success = true;
			first_two_chars = line.substr(0, 2);
			first_three_chars = line.substr(0, 3);


			if (first_two_chars == "v ") {
				glm::vec3 vertex;
				sscanf_s(line.c_str(), "v %f %f %f", &vertex.x, &vertex.y, &vertex.z);
				temp_vertices.push_back(vertex);
			}
			else if (first_three_chars == "vt ") {
				glm::vec2 uv;
				sscanf_s(line.c_str(), "vt %f %f", &uv.y, &uv.x);
				temp_uvs.push_back(uv);
			}
			else if (first_three_chars == "vn ") {
				glm::vec3 normal;
				sscanf_s(line.c_str(), "vn %f %f %f", &normal.x, &normal.y, &normal.z);
				temp_normals.push_back(normal);
			}
			else if (first_two_chars == "f ") {
				auto slashes = std::count(line.begin(), line.end(), '/');

				// f 1 2 3
				if (slashes == 0) {
					unsigned int indices_temp[3]{};
					sscanf_s(line.c_str(), "f %d %d %d", &indices_temp[0], &indices_temp[1], &indices_temp[2]);
					vertexIndices.insert(vertexIndices.end(), { indices_temp[0], indices_temp[1], indices_temp[2] });
				}

				// f 1/4 2/5 3/1
				else if (slashes == 3) {
					unsigned int indices_temp[6]{};
					sscanf_s(line.c_str(), "f %d/%d %d/%d %d/%d", &indices_temp[0], &indices_temp[3], &indices_temp[1], &indices_temp[4], &indices_temp[2], &indices_temp[5]);
					vertexIndices.insert(vertexIndices.end(), { indices_temp[0], indices_temp[1], indices_temp[2] });
					uvIndices.insert(uvIndices.end(), { indices_temp[3], indices_temp[4], indices_temp[5] });


				}
				else if (slashes == 6) {
					// f 7//1 8//2 9//3
					if (line.find("//") != std::string::npos) {
						unsigned int indices_temp[6]{};
						sscanf_s(line.c_str(), "f %d//%d %d//%d %d//%d", &indices_temp[0], &indices_temp[3], &indices_temp[1], &indices_temp[4], &indices_temp[2], &indices_temp[5]);
						vertexIndices.insert(vertexIndices.end(), { indices_temp[0], indices_temp[1], indices_temp[2] });
						normalIndices.insert(normalIndices.end(), { indices_temp[3], indices_temp[4], indices_temp[5] });
					}
					//f 6/4/1 3/5/3 7/6/5
					else {
						unsigned int indices_temp[9]{};
						sscanf_s(line.c_str(), "f %d/%d/%d %d/%d/%d %d/%d/%d",
							&indices_temp[0], &indices_temp[3], &indices_temp[6], &indices_temp[1], &indices_temp[4], &indices_temp[7], &indices_temp[2], &indices_temp[5], &indices_temp[8]);
						vertexIndices.insert(vertexIndices.end(), { indices_temp[0], indices_temp[1], indices_temp[2] });
						uvIndices.insert(uvIndices.end(), { indices_temp[3], indices_temp[4], indices_temp[5] });
						normalIndices.insert(normalIndices.end(), { indices_temp[6], indices_temp[7], indices_temp[8] });


					}

				}
				// f 1/1/1 2/2/2 22/23/3 21/22/4
				else if (slashes == 8) {
					unsigned int vertexIndex[4]{}, uvIndex[4]{}, normalIndex[4]{};
					sscanf_s(line.c_str(), "f %d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d\n",
						&vertexIndex[0], &uvIndex[0], &normalIndex[0],
						&vertexIndex[1], &uvIndex[1], &normalIndex[1],
						&vertexIndex[2], &uvIndex[2], &normalIndex[2],
						&vertexIndex[3], &uvIndex[3], &normalIndex[3]);

					vertexIndices.insert(vertexIndices.end(), { vertexIndex[0], vertexIndex[1], vertexIndex[2], vertexIndex[0], vertexIndex[2],  vertexIndex[3] });
					uvIndices.insert(uvIndices.end(), { uvIndex[0], uvIndex[1], uvIndex[2], vertexIndex[0], uvIndex[2],  uvIndex[3] });
					normalIndices.insert(normalIndices.end(), { normalIndex[0], normalIndex[1], normalIndex[2] , normalIndex[0], normalIndex[2],  normalIndex[3] });
				}
				else {
					line_success = false;
				}
			}
			else {
				line_success = false;
			}

			if (!line_success && first_two_chars != "# ") {
				std::cout << "LoadOBJFile: Ignoring line '" << line << "' in file" << path << std::endl;
			}
		}
	}

	// unroll from indirect to direct vertex specification
	// sometimes not necessary, definitely not optimal

	for (unsigned int u = 0; u < vertexIndices.size(); u++) {
		unsigned int vertexIndex = vertexIndices[u];
		glm::vec3 vertex = temp_vertices[vertexIndex - 1];
		out_vertices.push_back(vertex);
	}

	if (!temp_uvs.empty()) {
		for (unsigned int u = 0; u < uvIndices.size(); u++) {
			unsigned int uvIndex = uvIndices[u];
			glm::vec2 uv = temp_uvs[uvIndex - 1];
			out_uvs.push_back(uv);
		}
	}
	if (!temp_normals.empty()) {
		for (unsigned int u = 0; u < normalIndices.size(); u++) {
			unsigned int normalIndex = normalIndices[u];
			glm::vec3 normal = temp_normals[normalIndex - 1];
			out_normals.push_back(normal);
		}
	}
	return true;
}

std::vector<Vertex> make_Vertexes(std::vector<glm::vec3> vertices, std::vector<glm::vec3> normals, std::vector<glm::vec2> uvs) {
	std::vector<Vertex> out_vertex;
	for (unsigned int i = 0; i < vertices.size(); i++) {
		Vertex vertex;
		vertex.Position = vertices[i];
		vertex.Normal = i < normals.size() ? normals[i] : glm::vec3{ 0, 0, 0 };
		vertex.UVs = i < uvs.size() ? uvs[i] : glm::vec2{ 0.5, 0.5 };

		out_vertex.push_back(vertex);
	}
	return out_vertex;
}

std::vector<std::string> FillFileLines(const std::filesystem::path& file_name)
{
	std::string file_line;
	std::vector<std::string> file_lines;

	file_lines.clear();
	std::ifstream file_reader(file_name);
	while (getline(file_reader, file_line)) {
		file_lines.push_back(file_line);
	}
	file_reader.close();

	return file_lines;
}
