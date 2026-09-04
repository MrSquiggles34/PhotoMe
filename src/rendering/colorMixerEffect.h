#pragma once

#include "shaderEffect.h"

class ColorMixerEffect : public ShaderEffect {

public:
	ColorMixerEffect();

	bool Setup();

	void Apply(ofFbo & source, ofFbo & destination) override;

	void SetHue(float value);
	void SetSaturation(float value);
	void SetLuminance(float value);

	float GetHue() const;
	float GetSaturation() const;
	float GetLuminance() const;

private:
	float hue = 0.0f;
	float saturation = 0.0f;
	float luminance = 0.0f;
};
