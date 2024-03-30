#version 460 core

// input vertex attributes

in vec3 aPos;   // position: MUST exist
//in vec3 aColor; // any additional attributes are optional, any data type, etc.

uniform mat4 transform = mat4(1.0);
uniform mat4 projection = mat4(1.0);
uniform mat4 view      = mat4(1.0);

uniform vec4 my_color;

out vec4 color; // optional output attribute

void main()
{
    // Outputs the positions/coordinates of all vertices, MUST WRITE
    gl_Position = projection * view * transform * vec4(aPos, 1.0f);
    
    //color = aColor; // copy color to output
    color = my_color;
}
