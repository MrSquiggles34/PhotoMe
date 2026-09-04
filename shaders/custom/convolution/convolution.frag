#version 150

uniform sampler2D uTexture;
uniform vec2 uResolution;

in vec2 vTexCoord;

out vec4 fragColor;

void main()
{
    float kernel[25] = float[25](
        1.0, 1.0, 1.0, 1.0, 1.0,
        1.0, 1.0, 1.0, 1.0, 1.0,
        1.0, 1.0, 1.0, 1.0, 1.0,
		1.0, 1.0, 1.0, 1.0, 1.0,
		1.0, 1.0, 1.0, 1.0, 1.0
    );

    float kernelSum = 25.0;

    vec3 color = vec3(0.0);

    vec2 texelSize = 1.0 / uResolution;

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

            color +=
                sampleColor * kernel[index];

            index++;
        }
    }

    color /= kernelSum;

    fragColor = vec4(
        color,
        texture(uTexture, vTexCoord).a
    );
}
