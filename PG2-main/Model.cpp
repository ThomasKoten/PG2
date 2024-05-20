#include <numeric>
#include "Model.h"
#include "Texture.h"
#include "Mesh.h"

Model::Model(const std::filesystem::path& path_obj, const std::filesystem::path& path_tex, glm::vec3 obj_position, float obj_scale, glm::vec4 obj_rotation, bool is_height_map)
	:scale(obj_scale),
	rotation(obj_rotation),
	position(obj_position) {
	if (!is_height_map) {
		LoadOBJFile(path_obj, path_tex);
	}
	else {
		HeightMap_Load(path_obj);
		GLuint texture_id = textureInit(path_tex.string().c_str());
		position = glm::vec3(-center_x, position.y, -center_z);
		Mesh mesh = Mesh(GL_TRIANGLES, mesh_vertexes, mesh_indices, texture_id);
		meshes.push_back(mesh);
	}
}

void Model::Draw(Shader& shader) {
	// call Draw() on all meshes
	for (auto& mesh : meshes) {
		mesh.Draw(shader);
	}
}

glm::mat4 Model::getTransMatrix() {
	glm::mat4 model = glm::identity<glm::mat4>();

	model = glm::translate(model, position);
	model = glm::scale(model, glm::vec3(scale));
	rotation_axes = glm::vec3(rotation.x, rotation.y, rotation.z);
	model = glm::rotate(model, glm::radians(rotation.w), rotation_axes);
	return model;

}



void Model::LoadOBJFile(const std::filesystem::path& path_obj, const std::filesystem::path& path_tex)
{

	std::vector< glm::vec3 > vertices, normals;
	std::vector< glm::vec2 > uvs;

	MTLloader mtlLoader;
	bool hasMaterials = (path_tex.extension() == ".mtl");
	if (hasMaterials) {
		materials = mtlLoader.loadMTL(path_tex.string().c_str());
	}

	bool res = loadOBJ(path_obj.string().c_str(), vertices, uvs, normals, materials);

	if (!res) {
		std::cout << "Loading OBJ file failed." << std::endl;
		return;
	}
	if (hasMaterials) {
		for (const auto& material : materials) {
			auto mesh_vertexes = make_Vertexes(material.mat_vertices, material.mat_normals, material.mat_uvs);
			std::vector<unsigned int> mesh_indices(material.mat_vertices.size());
			std::iota(mesh_indices.begin(), mesh_indices.end(), 0);

			GLuint texture_id;
			// Create a solid color texture or use a default texture
			if (material.texturePath.empty()) {
				texture_id = createSolidColorTexture(material.diffuseColor);
			}
			else {
				texture_id = textureInit(material.texturePath.c_str());
			}

			Mesh mesh = Mesh(GL_TRIANGLES, mesh_vertexes, mesh_indices, texture_id);
			meshes.push_back(mesh);
		}

	}
	else {
		auto mesh_vertexes = make_Vertexes(vertices, normals, uvs);
		std::vector<unsigned int> mesh_indices(vertices.size());
		std::iota(mesh_indices.begin(), mesh_indices.end(), 0);

		GLuint texture_id = textureInit(path_tex.string().c_str());
		Mesh mesh = Mesh(GL_TRIANGLES, mesh_vertexes, mesh_indices, texture_id);
		meshes.push_back(mesh);
	}
}

void Model::HeightMap_Load(const std::filesystem::path& hm_file)
{

	mesh_vertexes.clear();
	mesh_indices.clear();

	hmap = cv::imread(hm_file.u8string(), cv::IMREAD_GRAYSCALE);

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

			normal = glm::normalize(glm::cross(p1 - p0, p2 - p0));

			// Get max normalized height for tile, set texture accordingly
			// Grayscale image returns 0..256, normalize to 0.0f..1.0f by dividing by 256
			float max_h = std::max(hmap.at<uchar>(cv::Point(x_coord, z_coord)) / 256.0f,
				std::max(hmap.at<uchar>(cv::Point(x_coord, z_coord + mesh_step_size)) / 256.0f,
					std::max(hmap.at<uchar>(cv::Point(x_coord + mesh_step_size, z_coord + mesh_step_size)) / 256.0f,
						hmap.at<uchar>(cv::Point(x_coord + mesh_step_size, z_coord)) / 256.0f
					)));

			// Get texture coords in vertices, bottom left of geometry == bottom left of texture
			glm::vec2 tc0 = getSubtexByHeight(max_h);
			glm::vec2 tc1 = tc0 + glm::vec2(1.0f / 16, 0.0f);		//add offset for bottom right corner
			glm::vec2 tc2 = tc0 + glm::vec2(1.0f / 16, 1.0f / 16);  //add offset for top right corner
			glm::vec2 tc3 = tc0 + glm::vec2(0.0f, 1.0f / 16);       //add offset for bottom leftcorner



			//place vertices and ST to mesh
			mesh_vertexes.emplace_back(Vertex{ p0,normal, tc0 });
			mesh_vertexes.emplace_back(Vertex{ p3, normal,tc3 });
			mesh_vertexes.emplace_back(Vertex{ p2,normal, tc2 });
			mesh_vertexes.emplace_back(Vertex{ p1,normal, tc1 });

			// place indices

			indices_counter += 4;
			mesh_indices.emplace_back(indices_counter - 4);
			mesh_indices.emplace_back(indices_counter - 3);
			mesh_indices.emplace_back(indices_counter - 2);
			mesh_indices.emplace_back(indices_counter - 4);
			mesh_indices.emplace_back(indices_counter - 2);
			mesh_indices.emplace_back(indices_counter - 1);

		}
	}
	center_x = (hmap.cols - mesh_step_size) / 2.0;
	center_z = (hmap.rows - mesh_step_size) / 2.0;

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
		return getSubtexST(2, 4); //snow
	else if (height > 0.8)
		return getSubtexST(2, 3); //diamond
	else if (height > 0.5)
		return getSubtexST(10, 6); //rock
	else if (height > 0.3)
		return getSubtexST(10, 4); //soil
	else
		return getSubtexST(6, 4); //grass
}
float Model::GetHeightAtPosition(float x, float z) const
{
	// Adjust for terrain position and scale if necessary
	x += center_x;
	z += center_z;

	int x_index = static_cast<int>(x);
	int z_index = static_cast<int>(z);

	// Ensure x and z are within valid terrain bounds
	if (x_index < 0 || x_index >= hmap.cols - 1 || z_index < 0 || z_index >= hmap.rows - 1) {
		return 0.0f;
	}

	float x_frac = x - static_cast<float>(x_index);
	float z_frac = z - static_cast<float>(z_index);

	// Sample heights of four surrounding points
	float height00 = static_cast<float>(hmap.at<uchar>(z_index, x_index));
	float height10 = static_cast<float>(hmap.at<uchar>(z_index, x_index + 1));
	float height01 = static_cast<float>(hmap.at<uchar>(z_index + 1, x_index));
	float height11 = static_cast<float>(hmap.at<uchar>(z_index + 1, x_index + 1));

	// Bilinear interpolation
	float height_interp =
		height00 * (1.0f - x_frac) * (1.0f - z_frac)
		+ height10 * x_frac * (1.0f - z_frac)
		+ height01 * (1.0f - x_frac) * z_frac
		+ height11 * x_frac * z_frac;

	return height_interp;
}
