#include "OBJloader.h"
#include "Model.h"

Model::Model(const std::filesystem::path& filename) {
	std::filesystem::path file_ext = filename.extension();
	if (file_ext == ".obj") {
		LoadOBJFile(filename);
		Mesh mesh = Mesh(GL_TRIANGLES, vertexes, vertex_indices, 0);
		meshes.push_back(mesh);
	}
	else if (file_ext == ".mtl") {
		//LoadMTLFile(filename);
	}
	// TODO: call LoadOBJFile, LoadMTLFile, process data, create mesh and set its properties 
}

void Model::Draw(Shader& shader) {
	// call Draw() on all meshes
	for (auto& mesh : meshes) {
		mesh.Draw(shader);
	}
}

void Model::LoadOBJFile(const std::filesystem::path& filename)
{

	std::vector< glm::vec3 > vertices, normals;
	std::vector< glm::vec2 > uvs;
	bool res = loadOBJ(filename.string().c_str(), vertices, uvs, normals);
	if (res) {
		vertexes = make_Vertexes(vertices, normals, uvs);
		for (unsigned int i = 0; i < vertices.size(); ++i) {
			vertex_indices.push_back(i);
		}
	}
	else {
		std::cout << "Nacteni souboru OBJ se nepovedlo." << '\n';
	}
}
