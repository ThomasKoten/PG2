#pragma once
#include <vector>
#include <fstream>
#include <sstream>

#include <GL/glew.h>
#include <glm/ext.hpp>

#include "Shader.h"



Shader::Shader(const std::filesystem::path& VS_file, const std::filesystem::path& FS_file) {
	std::vector<GLuint> shader_ids;
	GLuint VS_h, FS_h;
	VS_h = compile_shader(VS_file, GL_VERTEX_SHADER);
	FS_h = compile_shader(FS_file, GL_FRAGMENT_SHADER);
	shader_ids.push_back(VS_h);
	shader_ids.push_back(FS_h);

	ID = link_shader(shader_ids);

}
// TODO: load, compile, and link shader

void Shader::setUniform(const std::string& name, const float val) {
	auto loc = glGetUniformLocation(ID, name.c_str());
	if (loc == -1) {
		std::cerr << "no uniform with name:" << name << '\n';
		return;
	}
	glUniform1f(loc, val);
}

void Shader::setUniform(const std::string& name, const int val) {
	auto loc = glGetUniformLocation(ID, name.c_str());
	if (loc == -1) {
		std::cerr << "no uniform with name:" << name << '\n';
		return;
	}
	glUniform1f(loc, val);
}

void Shader::setUniform(const std::string& name, const glm::vec3 val)
{
	auto loc = glGetUniformLocation(ID, name.c_str());
	if (loc == -1) {
		std::cerr << "no uniform with name:" << name << '\n';
		return;
	}
	glUniform3fv(loc, 1, glm::value_ptr(val));
}

void Shader::setUniform(const std::string& name, const glm::vec4 val) {
	auto loc = glGetUniformLocation(ID, name.c_str());
	if (loc == -1) {
		std::cerr << "no uniform with name:" << name << '\n';
		return;
	}
	glUniform4fv(loc, 1, glm::value_ptr(val));
}

void Shader::setUniform(const std::string& name, const glm::mat3 val) {
	auto loc = glGetUniformLocation(ID, name.c_str());
	if (loc == -1) {
		std::cout << "no uniform with name:" << name << '\n';
		return;
	}
	glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(val));

}

void Shader::setUniform(const std::string& name, const glm::mat4 val) {
	auto loc = glGetUniformLocation(ID, name.c_str());
	if (loc == -1) {
		std::cout << "no uniform with name:" << name << '\n';
		return;
	}
	glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(val));
}

std::string Shader::getShaderInfoLog(const GLuint obj)
{
	int infologLength = 0;
	std::string s;
	glGetShaderiv(obj, GL_INFO_LOG_LENGTH, &infologLength);
	if (infologLength > 0) {
		std::vector<char> v(infologLength);
		glGetShaderInfoLog(obj, infologLength, NULL, v.data());
		s.assign(begin(v), end(v));
	}
	return s;
}

std::string Shader::getProgramInfoLog(const GLuint obj)
{
	int infologLength = 0;
	std::string s;
	glGetProgramiv(obj, GL_INFO_LOG_LENGTH, &infologLength);
	if (infologLength > 0) {
		std::vector<char> v(infologLength);
		glGetProgramInfoLog(obj, infologLength, NULL, v.data());
		s.assign(begin(v), end(v));
	}
	return s;
}

GLuint Shader::compile_shader(const std::filesystem::path& source_file, const GLenum type)
{
	GLuint shader_ID = glCreateShader(type);
	std::string str = textFileRead(source_file);
	const char* shader_string = str.c_str();

	glShaderSource(shader_ID, 1, &shader_string, NULL);
	glCompileShader(shader_ID);

	{
		GLint cmpl_status;
		glGetShaderiv(shader_ID, GL_COMPILE_STATUS, &cmpl_status);
		if (cmpl_status == GL_FALSE) {
			std::cerr << getShaderInfoLog(shader_ID);
			throw std::exception("Shader compilation err.\n");
		}
	}
	return shader_ID;
}

GLuint Shader::link_shader(const std::vector<GLuint> shader_ids)
{
	GLuint prog_h = glCreateProgram();
	for (GLuint id : shader_ids)
	{
		glAttachShader(prog_h, id);
	}

	glLinkProgram(prog_h);
	{ // check link result, display error (if any)
		GLint status;
		glGetProgramiv(prog_h, GL_LINK_STATUS, &status);
		if (status == GL_FALSE) {
			std::cerr << getProgramInfoLog(prog_h);
			throw std::exception("Link err.\n");
		}
	}
	return prog_h;
}

std::string Shader::textFileRead(const std::filesystem::path& filename)
{
	std::ifstream file(filename);
	if (!file.is_open())
		throw std::exception("Error opening file.\n");
	std::stringstream ss;
	ss << file.rdbuf();
	return ss.str();
}
