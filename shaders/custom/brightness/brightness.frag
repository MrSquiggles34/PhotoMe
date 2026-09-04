#version 150

uniform sampler2D uTexture;
uniform vec2 uResolution;

uniform float uAmount;
uniform int uSteps;
uniform bool uInvert;
uniform vec4 uTint;

in vec2 vTexCoord;

out vec4 fragColor;

void main()
{
    vec4 color = texture(
        uTexture,
        vTexCoord
    );

    // Float
    color.rgb *= uAmount;

    // Int
    float steps = float(max(uSteps, 1));
    color.rgb = floor(color.rgb * steps) / steps;

    // Color
    color.rgb *= uTint.rgb;

    // Bool
    if (uInvert)
    {
        color.rgb = 1.0 - color.rgb;
    }

    fragColor = color;
}
