#include "colorMixerEffect.h"
#include "ofMain.h"

ColorMixerEffect::ColorMixerEffect() {
}

bool ColorMixerEffect::Setup() {

	return Load(
		"shaders/colorMixer.vert",
		"shaders/colorMixer.frag");
}

void ColorMixerEffect::Apply(
	ofFbo & source,
	ofFbo & destination) {
	if (!IsLoaded())
		return;

	ofLogNotice()
		<< "THIS EFFECT: "
		<< static_cast<const void *>(this);

	ofLogNotice()
		<< "SOURCE TEX ID: "
		<< source.getTexture().getTextureData().textureID;

	ofLogNotice()
		<< "DEST TEX ID: "
		<< destination.getTexture().getTextureData().textureID;

	ofPushStyle();
	ofPushMatrix();

	destination.begin();

	ofClear(0, 0, 0, 0);

	ofSetColor(255);

	shader.begin();

	shader.setUniformTexture(
		"uTexture",
		source.getTexture(),
		0);

	shader.setUniform2f(
		"uResolution",
		destination.getWidth(),
		destination.getHeight());

	shader.setUniform1f("uHue", hue);
	shader.setUniform1f("uSaturation", saturation);
	shader.setUniform1f("uLuminance", luminance);

	ofDrawRectangle(
		0,
		0,
		destination.getWidth(),
		destination.getHeight());

	shader.end();

	destination.end();

	ofPopMatrix();
	ofPopStyle();
}

void ColorMixerEffect::SetHue(float value) {
	hue = value;
}

void ColorMixerEffect::SetSaturation(float value) {
	saturation = value;
}

void ColorMixerEffect::SetLuminance(float value) {
	luminance = value;
}

float ColorMixerEffect::GetHue() const {
	return hue;
}

float ColorMixerEffect::GetSaturation() const {
	return saturation;
}

float ColorMixerEffect::GetLuminance() const {
	return luminance;
}
