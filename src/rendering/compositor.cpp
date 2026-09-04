#include "compositor.h"

#include "../document/layer.h"

Compositor::Compositor() {
}

bool Compositor::Setup(int width, int height) {
	this->width = width;
	this->height = height;

	if (!paintShader.load("shaders/paintComposite.vert", "shaders/paintComposite.frag")) {
		ofLogError() << "Failed to load paint composite shader.";
		return false;
	}

	if (!eraseShader.load("shaders/eraseComposite.vert", "shaders/eraseComposite.frag")) {
		ofLogError() << "Failed to load erase composite shader.";
		return false;
	}

	if (!brushShader.load("shaders/brush.vert", "shaders/brush.frag")) {
		ofLogError() << "Failed to load brush shader.";
		return false;
	}

	ofFbo::Settings settings;
	settings.width = width;
	settings.height = height;
	settings.internalformat = GL_RGBA8;
	settings.useDepth = false;
	settings.useStencil = false;
	settings.textureTarget = GL_TEXTURE_2D;

	brushFbo.allocate(settings);
	compositeFbo.allocate(settings);

	return true;
}

void Compositor::DrawBrush(
	float x,
	float y,
	float radius,
	float hardness,
	const ofColor & color) {
	brushFbo.begin();

	ofClear(0, 0, 0, 0);

	brushShader.begin();

	brushShader.setUniform2f(
		"uResolution",
		brushFbo.getWidth(),
		brushFbo.getHeight());

	brushShader.setUniform2f(
		"uCenter",
		x,
		y);

	brushShader.setUniform1f(
		"uRadius",
		radius);

	brushShader.setUniform1f(
		"uHardness",
		hardness);

	brushShader.setUniform4f(
		"uColor",
		color.r / 255.0f,
		color.g / 255.0f,
		color.b / 255.0f,
		color.a / 255.0f);

	ofDrawRectangle(
		0,
		0,
		brushFbo.getWidth(),
		brushFbo.getHeight());

	brushShader.end();

	brushFbo.end();
}

void Compositor::Composite(Layer & layer, const ofFbo * selectionMask, ofShader & shader) {
	ofFbo & destination = layer.GetFbo();

	// --------------------------------------------------
	// Layer -> compositeFbo
	// --------------------------------------------------

	compositeFbo.begin();

	ofClear(0, 0, 0, 0);
	ofSetColor(255);

	destination.draw(0, 0, compositeFbo.getWidth(), compositeFbo.getHeight());

	compositeFbo.end();

	// --------------------------------------------------
	// compositeFbo -> Layer through shader
	// --------------------------------------------------

	destination.begin();

	ofClear(0, 0, 0, 0);
	ofSetColor(255);

	shader.begin();

	shader.setUniform2f("uResolution", destination.getWidth(), destination.getHeight());

	shader.setUniformTexture("uDestination", compositeFbo.getTexture(), 1);

	shader.setUniformTexture("uBrush", brushFbo.getTexture(), 2);

	bool hasSelection = selectionMask != nullptr;

	shader.setUniform1i("uHasSelection", hasSelection ? 1 : 0);

	if (hasSelection) {
		shader.setUniformTexture("uSelection", selectionMask->getTexture(), 3);
	}

	ofDrawRectangle(0, 0, destination.getWidth(), destination.getHeight());

	shader.end();

	destination.end();
}

void Compositor::Paint(
	Layer & layer,
	float x,
	float y,
	float radius,
	float hardness,
	const ofColor & color,
	const ofFbo * selectionMask) {
	DrawBrush(
		x,
		y,
		radius,
		hardness,
		color);

	Composite(
		layer,
		selectionMask,
		paintShader);
}

void Compositor::Erase(
	Layer & layer,
	float x,
	float y,
	float radius,
	float hardness,
	const ofFbo * selectionMask) {
	DrawBrush(
		x,
		y,
		radius,
		hardness,
		ofColor(255, 255, 255, 255));

	Composite(
		layer,
		selectionMask,
		eraseShader);
}

void Compositor::ApplyShader(Layer& layer, ofShader& shader) {

}
