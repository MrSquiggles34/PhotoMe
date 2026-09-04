#version 150

uniform vec2 uResolution;
uniform vec2 uCenter;
uniform float uRadius;
uniform float uHardness;
uniform vec4 uColor;

in vec2 vPosition;

out vec4 outputColor;

void main()
{
    float distanceFromCenter =
        distance(vPosition, uCenter);

    float edgeStart =
        uRadius * uHardness;

    float alpha =
        1.0 - smoothstep(
            edgeStart,
            uRadius,
            distanceFromCenter);

    if (distanceFromCenter > uRadius)
        alpha = 0.0;

    outputColor =
        vec4(
            uColor.rgb,
            uColor.a * alpha);
}
