#version 330

out vec4 colour;

uniform vec3 lightColour;

void main()
{
    colour = vec4(lightColour, 1.0f);
}