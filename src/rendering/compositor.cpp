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

void Compositor::DrawBrush(float x, float y, float radius, const ofColor & color) {

	brushFbo.begin();

	ofClear(0, 0, 0, 0);

	ofSetColor(color);
	ofDrawCircle(x, y, radius);

	ofSetColor(255);

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

void Compositor::Paint(Layer & layer, float x, float y, float radius, const ofColor & color, const ofFbo * selectionMask) {

	DrawBrush( x, y, radius, color);
	Composite(layer, selectionMask, paintShader);
}

void Compositor::Erase(Layer & layer, float x, float y, float radius, const ofFbo * selectionMask) {

	DrawBrush(x, y, radius, ofColor(255, 255, 255, 255));
	Composite(layer, selectionMask, eraseShader);
}
