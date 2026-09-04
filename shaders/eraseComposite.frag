#version 150

uniform sampler2D uDestination;
uniform sampler2D uBrush;
uniform sampler2D uSelection;

uniform bool uHasSelection;

in vec2 vTexCoord;

out vec4 outputColor;

void main()
{
    vec4 destination = texture(uDestination, vTexCoord);
    vec4 brush = texture(uBrush, vTexCoord);

    float selection = 1.0;

    if (uHasSelection)
    {
        selection = texture(uSelection, vTexCoord).a;
    }

    float eraseAmount = brush.a * selection;

    outputColor = vec4(
        destination.rgb,
        destination.a * (1.0 - eraseAmount)
    );
}
