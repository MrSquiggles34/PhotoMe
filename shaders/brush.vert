#version 150

uniform mat4 modelViewProjectionMatrix;

in vec4 position;

out vec2 vPosition;

void main()
{
    gl_Position = modelViewProjectionMatrix * position;

    vPosition = position.xy;
}
