#version 150

uniform sampler2D uTexture;

in vec2 vTexCoord;

out vec4 outputColor;

void main()
{
    vec4 color = texture(uTexture, vTexCoord);

    outputColor = vec4(
        1.0 - color.rgb,
        color.a
    );
}
