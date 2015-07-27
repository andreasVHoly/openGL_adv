#version 330

attribute vec3 vertex;
attribute vec3 normal;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

void main( void )
{
    gl_Position = Projection * View * Model * vec4(vertex, 1.0);
}
