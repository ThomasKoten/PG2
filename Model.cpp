#include "Model.h"
#include "Texture.h"
#include "Mesh.h"

Model::Model(const std::filesystem::path& filename, const std::filesystem::path& path_tex, glm::vec3 obj_position, bool is_height_map) {
	std::filesystem::path file_ext = filename.extension();
	position = obj_position;
	if (!is_height_map) {
		LoadOBJFile(filename);
		GLuint texture_id = textureInit(path_tex.string().c_str());
		Mesh mesh = Mesh(GL_TRIANGLES, mesh_vertexes, mesh_vertex_indices, texture_id);
		meshes.push_back(mesh);
	}
	else {
		HeightMap_Load(filename);
		GLuint texture_id = textureInit(path_tex.string().c_str());
		Mesh mesh = Mesh(GL_TRIANGLES, mesh_vertexes, mesh_vertex_indices, texture_id);
		meshes.push_back(mesh);
	}
	// TODO: call LoadOBJFile, LoadMTLFile, process data, create mesh and set its properties 
}

void Model::Draw(Shader& shader) {
	// call Draw() on all meshes
	for (auto& mesh : meshes) {
		mesh.Draw(shader);
	}
}

glm::mat4 Model::getTransMatrix(glm::mat4 trans) {
	//glm::mat4 model = glm::identity<glm::mat4>();

	//model=glm::translate(trans, position);
	//model=glm::scale(trans, scale);
	//rotation_axes = glm::vec3(rotation.x, rotation.y, rotation.z);
	//model=glm::rotate(trans, glm::radians(rotation.w), rotation_axes);
	//return model;
	return glm::translate(trans, position);
}



void Model::LoadOBJFile(const std::filesystem::path& filename)
{

	std::vector< glm::vec3 > vertices, normals;
	std::vector< glm::vec2 > uvs;
	bool res = loadOBJ(filename.string().c_str(), vertices, uvs, normals);
	if (res) {
		mesh_vertexes = make_Vertexes(vertices, normals, uvs);
		for (unsigned int i = 0; i < vertices.size(); ++i) {
			mesh_vertex_indices.push_back(i);
		}
	}
	else {
		std::cout << "Nacteni souboru OBJ se nepovedlo." << '\n';
	}
}

void Model::HeightMap_Load(const std::filesystem::path& hm_file)
{

	mesh_vertexes.clear();
	mesh_vertex_indices.clear();

	cv::Mat hmap = cv::imread(hm_file.u8string(), cv::IMREAD_GRAYSCALE);


	glm::vec3 v;
	glm::vec4 c;
	const unsigned int mesh_step_size = 10;

	if (hmap.empty())
	{
		std::cerr << "ERR: Height map empty? File:" << hm_file << std::endl;
	}

	if (hmap.channels() != 1)
	{
		std::cerr << "WARN: requested 1 channel, got: " << hmap.channels() << std::endl;
	}

	// Create heightmap mesh from TRIANGLES in XZ plane, Y is UP (right hand rule)
	//
	//   3-----2
	//   |    /|
	//   |  /  |
	//   |/    |
	//   0-----1
	//
	//   012,023
	//

	glm::vec3 normal{};
	unsigned int indices_counter = 0;

	for (unsigned int x_coord = 0; x_coord < (hmap.cols - mesh_step_size); x_coord += mesh_step_size)
	{
		for (unsigned int z_coord = 0; z_coord < (hmap.rows - mesh_step_size); z_coord += mesh_step_size)
		{
			// Get The (X, Y, Z) Value For The Bottom Left Vertex = 0
			glm::vec3 p0(x_coord, hmap.at<uchar>(cv::Point(x_coord, z_coord)), z_coord);
			// Get The (X, Y, Z) Value For The Bottom Right Vertex = 1
			glm::vec3 p1(x_coord + mesh_step_size, hmap.at<uchar>(cv::Point(x_coord + mesh_step_size, z_coord)), z_coord);
			// Get The (X, Y, Z) Value For The Top Right Vertex = 2
			glm::vec3 p2(x_coord + mesh_step_size, hmap.at<uchar>(cv::Point(x_coord + mesh_step_size, z_coord + mesh_step_size)), z_coord + mesh_step_size);
			// Get The (X, Y, Z) Value For The Top Left Vertex = 3
			glm::vec3 p3(x_coord, hmap.at<uchar>(cv::Point(x_coord, z_coord + mesh_step_size)), z_coord + mesh_step_size);

			// Get max normalized height for tile, set texture accordingly
			// Grayscale image returns 0..256, normalize to 0.0f..1.0f by dividing by 256
			float max_h = std::max(hmap.at<uchar>(cv::Point(x_coord, z_coord)) / 256.0f,
				std::max(hmap.at<uchar>(cv::Point(x_coord, z_coord + mesh_step_size)) / 256.0f,
					std::max(hmap.at<uchar>(cv::Point(x_coord + mesh_step_size, z_coord + mesh_step_size)) / 256.0f,
						hmap.at<uchar>(cv::Point(x_coord + mesh_step_size, z_coord)) / 256.0f
					)));

			// Get texture coords in vertices, bottom left of geometry == bottom left of texture
			glm::vec2 tc0 = getSubtexByHeight(max_h);
			glm::vec2 tc1 = tc0 + glm::vec2(1.0f / 16, 0.0f);			//add offset for bottom right corner
			glm::vec2 tc2 = tc0 + glm::vec2(1.0f / 16, 1.0f / 16);  //add offset for top right corner
			glm::vec2 tc3 = tc0 + glm::vec2(0.0f, 1.0f / 16);       //add offset for bottom leftcorner

			normal = glm::normalize(glm::cross(p1 - p0, p2 - p0));

			//place vertices and ST to mesh
			mesh_vertexes.emplace_back(Vertex{ p0,normal, tc0 });
			mesh_vertexes.emplace_back(Vertex{ p1,normal, tc1 });
			mesh_vertexes.emplace_back(Vertex{ p2,normal, tc2 });
			mesh_vertexes.emplace_back(Vertex{ p3, normal,tc3 });

			// place indices
			//mesh_vertex_indices.emplace_back(0, 1, 2, 0, 2, 3);
			// place indices
			indices_counter += 4;
			mesh_vertex_indices.emplace_back(indices_counter - 4);
			mesh_vertex_indices.emplace_back(indices_counter - 3);
			mesh_vertex_indices.emplace_back(indices_counter - 2);
			mesh_vertex_indices.emplace_back(indices_counter - 4);
			mesh_vertex_indices.emplace_back(indices_counter - 2);
			mesh_vertex_indices.emplace_back(indices_counter - 1);

		}
	}
	std::cout << "Heightmap size:" << hmap.size << std::endl;

}


//return bottom left ST of subtexture
glm::vec2 Model::getSubtexST(const int x, const int y)
{
	return glm::vec2(x * 1.0f / 16, y * 1.0f / 16);
}

glm::vec2 Model::getSubtexByHeight(float height)
{
	if (height > 0.9)
		return getSubtexST(2, 11); //snow
	else if (height > 0.8)
		return getSubtexST(3, 11); //ice
	else if (height > 0.5)
		return getSubtexST(0, 14); //rock
	else if (height > 0.3)
		return getSubtexST(2, 15); //soil
	else
		return getSubtexST(0, 11); //grass
}

