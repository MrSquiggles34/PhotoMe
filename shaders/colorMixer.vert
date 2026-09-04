#version 150

uniform mat4 modelViewProjectionMatrix;
uniform vec2 uResolution;

in vec4 position;

out vec2 vTexCoord;

void main()
{
    gl_Position = modelViewProjectionMatrix * position;

    vTexCoord = position.xy / uResolution;
}
