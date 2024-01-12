#version 450

layout( location = 0 ) in vec4 aVertexPosition;

layout( location = 1 ) in vec4 aVertexColor;

out vec4 color;

uniform mat4 model;

uniform mat4 view;

uniform mat4 projection;


void main() {

	gl_Position = projection * view * model * aVertexPosition;

	color = aVertexColor;

}
