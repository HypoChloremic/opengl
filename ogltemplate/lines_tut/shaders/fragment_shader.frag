#version 330 core

out vec4 color;

uniform vec3 uniformcolor;

void main(){
    color = vec4(uniformcolor, 1);
}
