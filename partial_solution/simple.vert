#version 330

/*
void main( void )
{
    gl_Position = Projection * View * Model * vec4(vertex, 1.0);
}*/


//old
//attribute vec3 vertex;
//attribute vec3 normal;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
//old



//new
in vec4 vert;
in vec4 norm;
uniform vec4 lpos;


out vec3 N;
out vec3 L;
out vec3 E;
//new
void main(){
    //new
    N = norm.xyz;
    L = lpos.xyz - vert.xyz;
    E = -vert.xyz;
    //old
    gl_Position = Projection * View * vert/*vec4(vertex, 1.0)*/;
}




