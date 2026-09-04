#version 150

uniform sampler2D uTexture;
uniform vec2 uResolution;

in vec2 vTexCoord;

out vec4 fragColor;

void sort25(inout float v[25])
{
    for (int i = 1; i < 25; i++)
    {
        float key = v[i];
        int j = i - 1;

        while (j >= 0 && v[j] > key)
        {
            v[j + 1] = v[j];
            j--;
        }

        v[j + 1] = key;
    }
}

void main()
{
    vec2 texel =
        1.0 / uResolution;

    vec3 original =
        texture(
            uTexture,
            vTexCoord
        ).rgb;

    float r[25];
    float g[25];
    float b[25];

    int i = 0;

    for (int y = -2; y <= 2; y++)
    {
        for (int x = -2; x <= 2; x++)
        {
            vec2 offset =
                vec2(x, y) * texel;

            vec2 sampleUV =
                clamp(
                    vTexCoord + offset,
                    vec2(0.0),
                    vec2(1.0)
                );

            vec3 color =
                texture(
                    uTexture,
                    sampleUV
                ).rgb;

            r[i] = color.r;
            g[i] = color.g;
            b[i] = color.b;

            i++;
        }
    }

    sort25(r);
    sort25(g);
    sort25(b);

    vec3 medianColor =
        vec3(
            r[12],
            g[12],
            b[12]
        );

    fragColor =
        vec4(
            medianColor,
            texture(
                uTexture,
                vTexCoord
            ).a
        );
}
