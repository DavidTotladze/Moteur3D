#version 450

layout( location = 0 ) out vec4 fragColor;

in vec4 color;

uniform float luminosity;


void main() {
	
	fragColor = color + vec4(luminosity, luminosity, luminosity, 0.f);

}