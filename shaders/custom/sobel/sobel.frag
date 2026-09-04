#version 150

uniform sampler2D uTexture;
uniform vec2 uResolution;

in vec2 vTexCoord;

out vec4 fragColor;

void main()
{
    float kernelX[25] = float[25](
        0.0,  0.0,  0.0,  0.0,  0.0,
        0.0, -1.0,  0.0,  1.0,  0.0,
        0.0, -2.0,  0.0,  2.0,  0.0,
        0.0, -1.0,  0.0,  1.0,  0.0,
        0.0,  0.0,  0.0,  0.0,  0.0
    );

    float kernelY[25] = float[25](
        0.0,  0.0,  0.0,  0.0,  0.0,
        0.0, -1.0, -2.0, -1.0,  0.0,
        0.0,  0.0,  0.0,  0.0,  0.0,
        0.0,  1.0,  2.0,  1.0,  0.0,
        0.0,  0.0,  0.0,  0.0,  0.0
    );

    vec2 texelSize =
        1.0 / uResolution;

    vec3 gradientX = vec3(0.0);
    vec3 gradientY = vec3(0.0);

    int index = 0;

    for (int y = -2; y <= 2; y++)
    {
        for (int x = -2; x <= 2; x++)
        {
            vec2 offset =
                vec2(x, y) * texelSize;

            vec3 sampleColor =
                texture(
                    uTexture,
                    vTexCoord + offset
                ).rgb;

            gradientX +=
                sampleColor * kernelX[index];

            gradientY +=
                sampleColor * kernelY[index];

            index++;
        }
    }

    vec3 magnitude =
        sqrt(
            gradientX * gradientX +
            gradientY * gradientY
        );

    fragColor = vec4(
        magnitude,
        texture(uTexture, vTexCoord).a
    );
}
