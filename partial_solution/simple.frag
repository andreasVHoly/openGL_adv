#version 330
/*out vec4 frag_color;
uniform vec4 fcolor;


void main() {
    frag_color = fcolor;
}

*/


uniform vec4 ambprod, diffprod, specprod;
uniform vec4 lpos;
uniform float shine;

in vec3 N;
in vec3 L;
in vec3 E;

void main(){
    vec3 NN = normalize(N);
    vec3 EE = normalize(E);
    vec3 LL = normalize(L);

    vec4 amb, diff, spec;

    vec3 H = normalize(LL+EE);

    float kd = max(dot(L,N), 0.0);
    float ks = pow(max(dot(N,H),0.0), shine);
    amb = ambprod;
    diff = kd*diffprod;
    spec = ks*specprod;
    gl_FragColor = vec4((amb+diff+spec).xyz, 1.0);

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

/*
in vec2 texCoord;
smooth in vec3 vNormal;
out vec4 outputColor;

uniform sampler2D gSampler;
uniform vec4 vColor;

struct SimpleDirectionalLight
{
   vec3 vColor;
   vec3 vDirection;
   float fAmbientIntensity;
};

uniform SimpleDirectionalLight sunLight;

void main()
{
   vec4 vTexColor = texture2D(gSampler, texCoord);
   float fDiffuseIntensity = max(0.0, dot(normalize(vNormal), -sunLight.vDirection));
   outputColor = vTexColor*vColor*vec4(sunLight.vColor*(sunLight.fAmbientIntensity+fDiffuseIntensity), 1.0);
}*/

