#include "Mesh.h"
#include "ShaderProgram.h"
#include "Model.h"

// indirect (indexed) draw 
std::filesystem::path VS_path = "/resources/basic/basic.vert";
std::filesystem::path FS_path = "/resources/basic/basic.frag";

Mesh::Mesh(GLenum primitive_type, std::vector<Vertex>& vertices, std::vector<GLuint>& indices, GLuint texture_id) :
    primitive_type(primitive_type),
    vertices(vertices),
    indices(indices),
    texture_id(texture_id)
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    // Bind VAO (set as the current)
    glBindVertexArray(VAO);
    // Bind the VBO, set type as GL_ARRAY_BUFFER
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // Fill-in data into the VBO
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
    // Bind EBO, set type GL_ELEMENT_ARRAY_BUFFER
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    // Fill-in data into the EBO
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

    //GLint attrib_location;
    //attrib_location = glGetAttribLocation(shader_prog_ID, "aPos"); //name in shader src
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(0 + offsetof(Vertex, Position)));
    // Enable the Vertex Attribute for position
    glEnableVertexAttribArray(0);
    // Set end enable Vertex Attribute for Normal
    //attrib_location = glGetAttribLocation(shader_prog_ID, "aNormal"); //name in shader src
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(0 + offsetof(Vertex, Normal)));
    glEnableVertexAttribArray(1);
    // Set end enable Vertex Attribute for Texture Coordinates
    //attrib_location = glGetAttribLocation(shader_prog_ID, "aTex"); //name in shader src
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(0 + offsetof(Vertex, UVs)));
    glEnableVertexAttribArray(2);



    // Bind VBO and VAO to 0 to prevent unintended modification of VAO/VBO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    //ShaderProgram my_shader= ShaderProgram(VS_path, FS_path);
    //glGenVertexArrays(1, &VAO);
    //glGenBuffers(1, &VBO);
    //glGenBuffers(1, &EBO);
    //// Bind VAO (set as the current)
    //glBindVertexArray(VAO);
    //// Bind the VBO, set type as GL_ARRAY_BUFFER
    //glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //// Fill-in data into the VBO
    //glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
    //// Bind EBO, set type GL_ELEMENT_ARRAY_BUFFER
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //// Fill-in data into the EBO
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);
    //// Set Vertex Attribute to explain OpenGL how to interpret the VBO
    //GLint attrib_location;
    //attrib_location = glGetAttribLocation(my_shader.getShaderID(), "aPos"); //name in shader src
    //glVertexAttribPointer(attrib_location, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(0 + offsetof(Vertex, Position)));
    //// Enable the Vertex Attribute for position
    //glEnableVertexAttribArray(attrib_location);
    //// Set end enable Vertex Attribute for Normal
    //attrib_location = glGetAttribLocation(my_shader.getShaderID(), "aNormal"); //name in shader src
    //glVertexAttribPointer(attrib_location, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(0 + offsetof(Vertex, Normal)));
    //glEnableVertexAttribArray(attrib_location);
    //// Set end enable Vertex Attribute forTexture Coordinates
    //attrib_location = glGetAttribLocation(my_shader.getShaderID(), "aTex"); //name in shader src
    //glVertexAttribPointer(attrib_location, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(0 + offsetof(Vertex, TexCoords)));
    //glEnableVertexAttribArray(attrib_location);
    //// Bind VBO and VAO to 0 to prevent unintended modification of VAO,VBO
    //glBindBuffer(GL_ARRAY_BUFFER, 0);
    //glBindVertexArray(0);
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    // USE
    //shader.activate();
    // TODO: create and initialize VAO, VBO, EBO and parameters
}

void Mesh::Draw(ShaderProgram& shader) {
    // TODO: set uniform variables: color
    //glm::vec3 color= { 1.0f, 0.0f, 0.0f};
    //shader.setUniform("color", color);

    //set texture id etc...
    //if (texture_id > 0) {
    //    glActiveTexture(GL_TEXTURE0);
    //    glBindTexture(GL_TEXTURE_2D, texture_id);
    //    shader.setUniform("mytexture", 0);
    //}

    //TODO: draw mesh: bind vertex array object, draw all elements with selected primitive type, unbind vertex array object 
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

}

void Mesh::clear(void) {
    vertices.clear();
    indices.clear();
    texture_id = 0;
    primitive_type = GL_POINT;

    // TODO: delete all allocations 
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &EBO);

}
