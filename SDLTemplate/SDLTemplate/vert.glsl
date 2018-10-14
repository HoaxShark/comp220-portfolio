#version 330 core

// location matches the location in the vertex pointer from main.cpp
layout(location = 0) in vec3 vertexPos;

out vec3 vertColor;

uniform mat4 modelMatrix;

void main()
{
	
	gl_Position = modelMatrix*vec4(vertexPos,1.0);
	vertColor = vertexPos;
}
