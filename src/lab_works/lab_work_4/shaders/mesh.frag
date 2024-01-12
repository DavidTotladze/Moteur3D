#version 450

layout( location = 0 ) out vec4 fragColor;

uniform vec3 ambient;
uniform vec3 diffuse;
uniform vec3 specular;
uniform float shininess;

in vec3 normal;

in vec3 position;

void main()
{

	vec3 diffuseFrag = max(dot(normal , position ), 0.f) * diffuse;

	vec3 specularFrag = pow(max(dot(normal, normalize(position)), 0.f), shininess) * specular;

	fragColor = vec4( ambient + diffuseFrag + specularFrag, 1.f );
}
