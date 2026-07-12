#include "layer.h"

Layer::Layer::Layer(int width, int height, LayerID id) : id(id) {
	fbo.allocate(width, height, GL_RGBA8);

	fbo.begin();
	ofClear(0, 0, 0, 0);
	fbo.end();
}

bool Layer::IsVisible() const {
	return visible;
}

// Sets
void Layer::SetImage(const ofImage & image) {
	fbo.begin();
	ofClear(0, 0, 0, 0);
	image.draw(0, 0);
	fbo.end();
}

void Layer::SetVisible(bool visible) {
	this->visible = visible;
}

void Layer::SetOpacity(float opacity) {
	this->opacity = ofClamp(opacity, 0.0f, 1.0f);
}

void Layer::SetName(const string & name) {
	this->name = name;
}

// Gets
ofFbo& Layer::GetFbo() {
	return fbo;
}

const ofFbo & Layer::GetFbo() const {
	return fbo;
}

ofTexture& Layer::GetTexture() {
	return texture;
}

const ofTexture & Layer::GetTexture() const {
	return texture;
}

float Layer::GetOpacity() const {
	return opacity;
}

const string& Layer::GetName() const {
	return name;
}

LayerID Layer::GetID() const {
	return id;
}
