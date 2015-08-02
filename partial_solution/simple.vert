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
    gl_Position = Projection * View * Model * vert/*vec4(vertex, 1.0)*/;
}



/*
#include_part

struct PointLight
{
   vec3 vColor; // Color of that point light
   vec3 vPosition;

   float fAmbient;

   float fConstantAtt;
   float fLinearAtt;
   float fExpAtt;
};

vec4 getPointLightColor(const PointLight ptLight, vec3 vWorldPos, vec3 vNormal);

#definition_part

vec4 getPointLightColor(const PointLight ptLight, vec3 vWorldPos, vec3 vNormal)
{
   vec3 vPosToLight = vWorldPos-ptLight.vPosition;
   float fDist = length(vPosToLight);
   vPosToLight = normalize(vPosToLight);

   float fDiffuse = max(0.0, dot(vNormal, -vPosToLight));

   float fAttTotal = ptLight.fConstantAtt + ptLight.fLinearAtt*fDist + ptLight.fExpAtt*fDist*fDist;

   return vec4(ptLight.vColor, 1.0)*(ptLight.fAmbient+fDiffuse)/fAttTotal;
}



/*uniform mat4 projectionMatrix;
uniform mat4 modelViewMatrix;
uniform mat4 normalMatrix;

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec2 inCoord;
layout (location = 2) in vec3 inNormal;

out vec2 texCoord;

smooth out vec3 vNormal;

void main()
{
   gl_Position = projectionMatrix*modelViewMatrix*vec4(inPosition, 1.0);
   texCoord = inCoord;
   vec4 vRes = normalMatrix*vec4(inNormal, 0.0);
   vNormal = vRes.xyz;
}
*/


