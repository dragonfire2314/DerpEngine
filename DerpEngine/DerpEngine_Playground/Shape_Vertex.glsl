#version 330 core

layout (location = 0) in vec3 vertexPosition_modelspace;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

void main(){
    gl_Position = Projection * View * vec4(vertexPosition_modelspace, 1.0);
}