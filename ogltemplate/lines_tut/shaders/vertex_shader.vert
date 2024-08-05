#version 330 core

layout(location=0) in vec3 aPosition;
uniform mat4 MVP;

void main(){
	gl_Position = MVP * vec4(aPosition, 1);
    gl_PointSize = 20.0;  // Set the point size here
}

