#version 450

layout( location = 0 ) in vec4 aVertexPosition;

layout( location = 1 ) in vec4 aVertexColor;

out vec4 color;

uniform float uTranslationX;

void main() {

	gl_Position = aVertexPosition + vec4(uTranslationX, 0.f, 0.f, 0.f);

	color = aVertexColor;

}