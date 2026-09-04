#version 150

uniform sampler2D uTexture;
uniform vec2 uResolution;

uniform float uHistogramMap[256];

in vec2 vTexCoord;
out vec4 fragColor;

void main()
{
    vec4 source =
        texture(uTexture, vTexCoord);

    float luminance =
        0.299 * source.r +
        0.587 * source.g +
        0.114 * source.b;

    int bin =
        int(clamp(
            luminance * 255.0,
            0.0,
            255.0
        ));

    float equalized =
        uHistogramMap[bin];

    fragColor =
        vec4(
            equalized,
            equalized,
            equalized,
            source.a
        );
}
