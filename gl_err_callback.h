#ifndef GL_ERR_CALLBACK_H
#define GL_ERR_CALLBACK_H


#include <GL/glew.h>
#include <GLFW/glfw3.h>

void GLAPIENTRY MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);

#endif  GL_ERR_CALLBACK_H
