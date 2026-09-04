#version 150

uniform sampler2D uTexture;
uniform vec2 uResolution;

in vec2 vTexCoord;

out vec4 fragColor;


void main()
{
    vec2 texel =
        1.0 / uResolution;

    // --------------------------------------------------
    // Collect 5x5 neighborhood
    // --------------------------------------------------

    vec3 neighbors[25];

    int index = 0;

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

            neighbors[index] =
                texture(
                    uTexture,
                    sampleUV
                ).rgb;

            index++;
        }
    }


    // --------------------------------------------------
    // Initial estimate
    //
    // Same as the CPU version:
    // average of all neighborhood pixels.
    // --------------------------------------------------

    vec3 median = vec3(0.0);

    for (int i = 0; i < 25; i++)
    {
        median += neighbors[i];
    }

    median /= 25.0;


    // --------------------------------------------------
    // Weiszfeld algorithm
    // --------------------------------------------------

    const int maxIterations = 5;
    const float epsilon = 1.0e-5;

    for (int iteration = 0;
         iteration < maxIterations;
         iteration++)
    {
        vec3 numerator = vec3(0.0);
        float denominator = 0.0;

        for (int i = 0; i < 25; i++)
        {
            float distance =
                length(
                    median - neighbors[i]
                );

            // Prevent division by zero.
            distance += 1.0e-6;

            numerator +=
                neighbors[i] / distance;

            denominator +=
                1.0 / distance;
        }

        vec3 newMedian =
            numerator / denominator;

        if (length(newMedian - median) < epsilon)
        {
            median = newMedian;
            break;
        }

        median = newMedian;
    }


    // --------------------------------------------------
    // Output
    // --------------------------------------------------

    float alpha =
        texture(
            uTexture,
            vTexCoord
        ).a;

    fragColor =
        vec4(
            median,
            alpha
        );
}
