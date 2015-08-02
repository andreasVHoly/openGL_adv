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

out vec4 frag_color;

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
    //gl_FragColor = vec4((amb+diff+spec).xyz, 1.0);
    frag_color = vec4((amb+diff+spec).xyz, 1.0);

}




