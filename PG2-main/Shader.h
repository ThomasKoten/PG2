#pragma once
#include <string>
#include <filesystem>

#include <GL/glew.h> 
#include <glm/ext.hpp>

#define  VS_PATH std::filesystem::path("./resources/shaders/FragRT.vert")
#define FS_PATH std::filesystem::path("./resources/shaders/FragRT.frag")
class Shader {
public:
	Shader(void) = default; //does nothing
	Shader(const std::filesystem::path& VS_file, const std::filesystem::path& FS_file);

	void activate(void) { glUseProgram(ID); }
	void deactivate(void) { glUseProgram(0); }
	void clear(void) {
		deactivate();
		glUseProgram(ID);
		ID = 0;
	}

	GLuint getShaderID() {
		return ID;
	}

	void setUniform(const std::string& name, const int val);

	void setUniform(const std::string& name, const float val);

	void setUniform(const std::string& name, const glm::vec3 val);
	void setUniform(const std::string& name, const glm::vec4 val);

	void setUniform(const std::string& name, const glm::mat3 val);
	void setUniform(const std::string& name, const glm::mat4 val);

	void setUniformArray(const std::string& name, const std::vector<glm::vec3>& val);
	void setUniformArray(const std::string& name, const std::vector<float>& val);
private:
	GLuint ID{ 0 }; // default = 0, empty shader
	std::string getShaderInfoLog(const GLuint obj);
	std::string getProgramInfoLog(const GLuint obj);

	GLuint compile_shader(const std::filesystem::path& source_file, const GLenum type);
	GLuint link_shader(const std::vector<GLuint> shader_ids);
	std::string textFileRead(const std::filesystem::path& filename);
};

