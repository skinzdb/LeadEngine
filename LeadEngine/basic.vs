#version 330 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 tex;
layout(location = 2) in vec3 normal;

out vec2 texCoord;

out vec3 surfaceNormal;
out vec3 toLightVector;
out vec3 toCamVector;

uniform mat4 projMatrix;
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;

uniform vec3 lightPos;

//uniform float numberOfRows;
//uniform vec2 offset;

void main()
{
	vec4 worldPos = modelMatrix * vec4(pos, 1.0);
	gl_Position = projMatrix * viewMatrix * worldPos;

	surfaceNormal = (modelMatrix * vec4(normal, 0.0)).xyz;
	toLightVector = lightPos - worldPos.xyz;
	toCamVector = (inverse(viewMatrix) * vec4(0.0, 0.0, 0.0, 1.0)).xyz - worldPos.xyz;

	texCoord = tex;
}